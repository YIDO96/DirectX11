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
	// 매 프레임 게임 로직 업데이트

}

void Game::Render()
{
	RenderBegin();

	//TODO

	RenderEnd();
}

void Game::RenderBegin()
{
	// 1) 출력 병합 단계(OM)에 렌더 타깃을 묶는다. (DSV 없음)
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	// 2) 현재 렌더 타깃을 지정한 색으로 초기화
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	// 3) 레스터라이저 단계(RS)에 사용할 뷰포트 설정
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	// 더블 버퍼링 : 후면 버퍼에 그린 내용을 화면(전면)으로 표시
	// 첫번째 인자 1은 VSync를 1로 맞춰 화면 주사율에 동기화 (티어링 방지)
	// 0으로 하면 즉시 표시 (티어링 발생 가능)
	_swapChain->Present(1, 0);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // ::memset(&desc, 0, sizeof(desc));
	{
		// 출력 버퍼 크기를 윈도 크기에 맞춘다.
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60; // 60Hz
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32bit RGBA (정규화)
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;					// 멀티샘플링 비활성화 (MSAA x)
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더타겟으로 사용
		desc.BufferCount = 1;						// 후면 버퍼 개수 (레거시 Blt 모델 : 1이면 더블 버퍼링)
		desc.OutputWindow = _hwnd;					// 렌더링할 대상 윈도우
		desc.Windowed = TRUE;						// 창모드(True), 전체화면 (False)
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 스왑 후 버퍼 내용 폐기 (레거시 모델)
	}

	// comPtr 팁 : Get()은 원 포인터, GetAddressOf()는 포인터의 주소
	// 매개변수에서 *이면 Get(), **이면 GetAddressOf()
	_device.Get(); // ID3D11Device*를 꺼내오고 싶을때
	_device.GetAddressOf(); // ID3D11Device*의 주소를 꺼내오고 싶을때

	// device와 swapChain을 동시 생성(레거시 경로)
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,						// 디폴트 어댑터
		D3D_DRIVER_TYPE_HARDWARE,		// 하드웨어 디바이스
		nullptr,						// 소프트웨어 래스터라이저(HAL 사용시 nullptr)
		0,								// 플래그(디버그 레이어는 디버그 빌드에서 D3D11_CREATE_DEVICE_DEBUG 권장)
		nullptr,						// 원하는 피처레벨 목록 (nullptr이면 내부 기본 목록)
		0,								// 피처레벨 개수
		D3D11_SDK_VERSION,				// SDK 버전
		&desc,							// 스왑체인 설명
		_swapChain.GetAddressOf(),		// [out] 스왑체인
		_device.GetAddressOf(),			// [out] 디바이스
		nullptr,						// [out] 선택된 피처레벨 (필요시 포인터 전달)
		_deviceContext.GetAddressOf()	// [out] 디바이스 컨텍스트
	);

	CHECK(hr); // 성공 여부 매크로


}

void Game::CreateRenderTargertView()
{
	HRESULT hr;

	// 후면 버퍼(텍스처) 임시 포인터
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// swapChain에서 후면 버퍼에 해당하는 리소스를 ID3D11Textrue2D로 가져오고
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// 가져온 후면버퍼를 묘사하는 RenderTargetView로 만들어줬고
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
