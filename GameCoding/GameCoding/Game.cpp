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
	CreateVS();				// VS 로드
	CreateInputLayout();
	CreatePS();				// PS 로드

	//CreateRasterizerState();
	CreateSamplerState();
	//CreateBlendState();

	CreateSRV();
	CreateConstantBuffer();
}

void Game::Update()
{
	//Scale Rotation Translation

	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	Matrix matWorld = matScale * matRotation * matTranslation;
	_transformData.matWorld = matWorld;



	// 매 프레임 게임 로직 업데이트
	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 , &subResource);
	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));

	_deviceContext->Unmap(_constantBuffer.Get(), 0);
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
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

		// RS
		_deviceContext->RSSetState(_rasterizerState.Get());

		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		_deviceContext->PSSetShaderResources(1, 1, _shaderResourceView2.GetAddressOf());
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());


		// OM
		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);


		//_deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

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
void Game::CreateGeometry()
{
	// VertexData
	{
		_vertices.resize(4);	
		// 13
		// 02
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
		_vertices[0].uv = Vec2(0.f, 1.f);
		//_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
		_vertices[1].uv = Vec2(0.f, 0.f);
		//_vertices[1].color = Color(1.f, 0.f, 0.f, 1.f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
		_vertices[2].uv = Vec2(1.f, 1.f);
		//_vertices[2].color = Color(1.f, 0.f, 0.f, 1.f);

		_vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
		_vertices[3].uv = Vec2(1.f, 0.f);
		//_vertices[3].color = Color(1.f, 0.f, 0.f, 1.f);
	}

	// VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;				// GPU 전용 버퍼
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// VertexBuffer 용도
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data(); // == &_vertices[0]	// 초기 데이터 전달


		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}

	// Index
	{
		_indices = { 0, 1, 2, 2, 1, 3 };

	}

	// IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;				// GPU 전용 버퍼
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;		// IndexBuffer 용도
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data(); // == &_vertices[0]	// 초기 데이터 전달


		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		CHECK(hr);
	}
}
void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// 의미, 인덱스, 포맷, 입력슬롯, 오프셋, 데이터종류, 인스턴스 데이터 스텝률
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// 12 : Position이 0 ~ 11오프셋에 있고 12부터 Color가 시작
		//{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(),
		_inputLayout.GetAddressOf());
}
void Game::CreateVS()
{
	// 쉐이더를 로드해서 _vsBlob에 담고
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	// Blob을 이용해 버퍼포인터, 버퍼사이즈를 이용해 _vertexShader를 생성
	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(),
		nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}
void Game::CreatePS()
{
	// 쉐이더를 로드해서 _psBlob에 담고
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	// Blob을 이용해 버퍼포인터, 버퍼사이즈를 이용해 _pixelShader를 생성
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(),
		nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}
void Game::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID; // 실선
	desc.CullMode = D3D11_CULL_BACK; // 뒷면 컬링
	desc.FrontCounterClockwise = false; // 시계방향이 앞면
	desc.DepthClipEnable = false; // 깊이버퍼 범위 밖 절단 활성화
	desc.ScissorEnable = false; // 시저링 비활성화
	desc.MultisampleEnable = false; // 멀티샘플링 비활성화
	desc.AntialiasedLineEnable = false; // 앤티앨리어싱 비활성화


	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;	// U축 경계처리
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;	// V축 경계처리
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;	// W축 경계처리
	desc.BorderColor[0] = 1;						// 경계색 : 빨강
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 선형 필터링
	desc.MaxAnisotropy = 16;	// 이방성 필터링 샘플 개수
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;


	_device->CreateSamplerState(&desc, _samplerState.GetAddressOf());
}
void Game::CreateBlendState()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;		// 알파 투 커버리지 비활성화
	desc.IndependentBlendEnable = false;	// 독립 블렌딩 비활성화

	desc.RenderTarget[0].BlendEnable = true; // 블렌딩 비활성화
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // SrcFactor
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // DstFactor
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // 합산
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // RGBA 모두 쓰기



	_device->CreateBlendState(&desc, _blendState.GetAddressOf());
}
void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"Dog.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);


}
void Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_DYNAMIC; 				// CPU_Write + GPU_Read
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 상수버퍼 용도
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU가 쓰기 가능

	HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
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
