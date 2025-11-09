#pragma once
class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device>GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }


private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargertView();
	void SetViewport();

private:
	HWND _hwnd;					// 윈도우 핸들

	// Device & SwapChain
	ComPtr<ID3D11Device> _device = nullptr;					//	GPU 디바이스 (리로스 생성 추제)
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;	// 	디바이스 컨텍스트 (렌더링 명령 전달)
	ComPtr<IDXGISwapChain> _swapChain = nullptr;			//	spawChain (화면에 보여질 버퍼 교체 관리)

	// RTV 
	ComPtr<ID3D11RenderTargetView> _renderTargetView;		//	후면 버퍼를 그릴 수 있게 하는 뷰

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
};

