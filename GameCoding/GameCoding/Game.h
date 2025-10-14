#pragma once


// DirectX11 렌더링을 관리하는 게임 엔진으 최소 단위 클래스
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);		// 초기화 : device, swapchain, RTV, viewport 초기화
	void Update();				// 매 프레임 갱신 로직 (입력, 게임상태 업데이트 등)	
	void Render();				// 매 프레임 렌더링

private:
	void RenderBegin();			// 렌더 타겟 바인딩, 클리어, 뷰포트 설정
	void RenderEnd();			// 화편 표시 (Present)


private:
	void CreateDeviceAndSwapChain();		// D3D11 Device, DeviceContext ,SwapChain 생성
	void CreateRenderTargertView();			// 백버퍼로부터 RTV 생성
	void SetViewport();						// 뷰포트 설정

private:
	void CreateGeometry();				// 기하학적 도형 생성
	void CreateInputLayout();			// 정점 레이아웃 생성
	void CreateVS();
	void CreatePS();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	HWND _hwnd;					// 윈도우 핸들
	uint32 _width = 0;			// 윈도우 창 크기
	uint32 _height = 0;			// 윈도우 창 크기

private:
	// Device & SwapChain
	ComPtr<ID3D11Device> _device = nullptr;					//	GPU 디바이스 (리로스 생성 추제)
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;	// 	디바이스 컨텍스트 (렌더링 명령 전달)
	ComPtr<IDXGISwapChain> _swapChain = nullptr;			//	spawChain (화면에 보여질 버퍼 교체 관리)

	// RTV 
	ComPtr<ID3D11RenderTargetView> _renderTargetView;		//	후면 버퍼를 그릴 수 있게 하는 뷰

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.f, 0.f, 0.f, 0.f };

private:
	// Geometry
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;


	// [CPU<->RAM]  [GPU<->VRAM]

};

