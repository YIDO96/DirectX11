#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	// TODO
	CreateDeviceAndSwapChain();
	CreateRenderTargertView();
	SetViewport();
}

void Game::Update()
{
	// �� ������ ���� ���� ������Ʈ

}

void Game::Render()
{
	RenderBegin();

	//TODO

	RenderEnd();
}

void Game::RenderBegin()
{
	// 1) ��� ���� �ܰ�(OM)�� ���� Ÿ���� ���´�. (DSV ����)
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	// 2) ���� ���� Ÿ���� ������ ������ �ʱ�ȭ
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	// 3) �����Ͷ����� �ܰ�(RS)�� ����� ����Ʈ ����
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	// ���� ���۸� : �ĸ� ���ۿ� �׸� ������ ȭ��(����)���� ǥ��
	// ù��° ���� 1�� VSync�� 1�� ���� ȭ�� �ֻ����� ����ȭ (Ƽ� ����)
	// 0���� �ϸ� ��� ǥ�� (Ƽ� �߻� ����)
	_swapChain->Present(1, 0);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // ::memset(&desc, 0, sizeof(desc));
	{
		// ��� ���� ũ�⸦ ���� ũ�⿡ �����.
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60; // 60Hz
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32bit RGBA (����ȭ)
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;					// ��Ƽ���ø� ��Ȱ��ȭ (MSAA x)
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ����Ÿ������ ���
		desc.BufferCount = 1;						// �ĸ� ���� ���� (���Ž� Blt �� : 1�̸� ���� ���۸�)
		desc.OutputWindow = _hwnd;					// �������� ��� ������
		desc.Windowed = TRUE;						// â���(True), ��üȭ�� (False)
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ���� �� ���� ���� ��� (���Ž� ��)
	}

	// comPtr �� : Get()�� �� ������, GetAddressOf()�� �������� �ּ�
	// �Ű��������� *�̸� Get(), **�̸� GetAddressOf()
	_device.Get(); // ID3D11Device*�� �������� ������
	_device.GetAddressOf(); // ID3D11Device*�� �ּҸ� �������� ������

	// device�� swapChain�� ���� ����(���Ž� ���)
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,						// ����Ʈ �����
		D3D_DRIVER_TYPE_HARDWARE,		// �ϵ���� ����̽�
		nullptr,						// ����Ʈ���� �����Ͷ�����(HAL ���� nullptr)
		0,								// �÷���(����� ���̾�� ����� ���忡�� D3D11_CREATE_DEVICE_DEBUG ����)
		nullptr,						// ���ϴ� ��ó���� ��� (nullptr�̸� ���� �⺻ ���)
		0,								// ��ó���� ����
		D3D11_SDK_VERSION,				// SDK ����
		&desc,							// ����ü�� ����
		_swapChain.GetAddressOf(),		// [out] ����ü��
		_device.GetAddressOf(),			// [out] ����̽�
		nullptr,						// [out] ���õ� ��ó���� (�ʿ�� ������ ����)
		_deviceContext.GetAddressOf()	// [out] ����̽� ���ؽ�Ʈ
	);

	CHECK(hr); // ���� ���� ��ũ��


}

void Game::CreateRenderTargertView()
{
	HRESULT hr;

	// �ĸ� ����(�ؽ�ó) �ӽ� ������
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// swapChain���� �ĸ� ���ۿ� �ش��ϴ� ���ҽ��� ID3D11Textrue2D�� ��������
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// ������ �ĸ���۸� �����ϴ� RenderTargetView�� ��������
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}
