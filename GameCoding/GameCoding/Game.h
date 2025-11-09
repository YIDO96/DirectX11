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

private:
	void CreateGeometry();				// 기하학적 도형 생성
	void CreateInputLayout();			// 정점 레이아웃 생성
	void CreateVS();
	void CreatePS();

	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();
	void CreateSRV();

	void CreateConstantBuffer();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	HWND _hwnd;					// 윈도우 핸들

	//shared_ptr<Graphics> _graphics;
	Graphics* _graphics;

	//uint32 _width = 0;			// 윈도우 창 크기
	//uint32 _height = 0;			// 윈도우 창 크기

private:
//	// Device & SwapChain
//	ComPtr<ID3D11Device> _device = nullptr;					//	GPU 디바이스 (리로스 생성 추제)
//	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;	// 	디바이스 컨텍스트 (렌더링 명령 전달)
//	ComPtr<IDXGISwapChain> _swapChain = nullptr;			//	spawChain (화면에 보여질 버퍼 교체 관리)
//
//	// RTV 
//	ComPtr<ID3D11RenderTargetView> _renderTargetView;		//	후면 버퍼를 그릴 수 있게 하는 뷰
//
//	// Misc
//	D3D11_VIEWPORT _viewport = { 0 };
//	float _clearColor[4] = { 0.f, 0.f, 0.f, 0.f };

private:
	// Geometry
	vector<Vertex> _vertices;
	//ComPtr<ID3D11Buffer> _vertexBuffer1 = nullptr;
	VertexBuffer* _vertexBuffer;


	vector<uint32> _indices;
	IndexBuffer* _indexBuffer;
	//ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	//ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	InputLayout* _inputLayout;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// RAS
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;

	// SRV
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView2 = nullptr;

	ComPtr<ID3D11SamplerState> _samplerState = nullptr;
	ComPtr<ID3D11BlendState> _blendState = nullptr;

	// [CPU<->RAM]  [GPU<->VRAM]

private:
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;

	Vec3 _localPosition = Vec3(0.f, 0.f, 0.f);
	Vec3 _localRotation = Vec3(0.f, 0.f, 0.f);
	Vec3 _localScale = Vec3(1.f, 1.f, 1.f);
};

