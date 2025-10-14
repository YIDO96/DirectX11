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

	CreateGeometry();
	CreateVS();				// VS �ε�
	CreateInputLayout();
	CreatePS();				// PS �ε�
}

void Game::Update()
{
	// �� ������ ���� ���� ������Ʈ

}

void Game::Render()
{
	RenderBegin();

	//TODO
	// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(),
			&stride, &offset);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);


		// OM

		_deviceContext->Draw(_vertices.size(), 0);
	}

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

void Game::CreateGeometry()
{
	// VertexData
	{
		_vertices.resize(3);

		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
		_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);

		_vertices[1].position = Vec3(0.f, 0.5f, 0.f);
		_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
		_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);
	}

	// VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;				// GPU ���� ����
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// VertexBuffer �뵵
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data(); // == &_vertices[0]	// �ʱ� ������ ����


		_device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
	}
}

void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// �ǹ�, �ε���, ����, �Է½���, ������, ����������, �ν��Ͻ� ������ ���ܷ�
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// 12 : Position�� 0 ~ 11�����¿� �ְ� 12���� Color�� ����
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(),
		_inputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	// ���̴��� �ε��ؼ� _vsBlob�� ���
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	// Blob�� �̿��� ����������, ���ۻ���� �̿��� _vertexShader�� ����
	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(),
		nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	// ���̴��� �ε��ؼ� _psBlob�� ���
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	// Blob�� �̿��� ����������, ���ۻ���� �̿��� _pixelShader�� ����
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(),
		nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, 
	const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr);

	CHECK(hr);
}
