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

	//_graphics = make_shared<Graphics>(hwnd);
	_graphics = new Graphics(hwnd);
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());
	_geometry = make_shared<Geometry<VertexTextureData>>();


	CreateGeometry();
	CreateVS();				// VS 로드
	CreateInputLayout();
	CreatePS();				// PS 로드

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	CreateSRV();
	CreateConstantBuffer();
}

void Game::Update()
{
	//Scale Rotation Translation
	_localPosition.x += 0.0005f;

	Matrix matScale = Matrix::CreateScale(_localScale/3);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	Matrix matWorld = matScale * matRotation * matTranslation;
	_transformData.matWorld = matWorld;



	// 매 프레임 게임 로직 업데이트
	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));


	_graphics->GetDeviceContext()->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));

	_graphics->GetDeviceContext()->Unmap(_constantBuffer.Get(), 0);
}

void Game::Render()
{
	_graphics->RenderBegin();

	//TODO
	// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(VertexTextureData);
		uint32 offset = 0;
		
		auto deviceContext = _graphics->GetDeviceContext();

		// IA
		deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetInputLayout(_inputLayout->GetComPtr().Get());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

		// RS
		deviceContext->RSSetState(_rasterizerState.Get());

		// PS
		deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		deviceContext->PSSetShaderResources(1, 1, _shaderResourceView2.GetAddressOf());
		deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());


		// OM
		deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);


		deviceContext->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	_graphics->RenderEnd();
}

// Graphics

// Graphics

void Game::CreateGeometry()
{
	// VertexData
	GeometryHelper::CreateRectangle(_geometry);

	// VertexBuffer
	{
		_vertexBuffer->Create(_geometry->GetVertices());
	}

	// IndexBuffer
	{
		_indexBuffer->Create(_geometry->GetIndices());
	}
}
void Game::CreateInputLayout()
{
	_inputLayout->Create(VertexTextureData::descs, _vsBlob);
}
void Game::CreateVS()
{
	// 쉐이더를 로드해서 _vsBlob에 담고
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	// Blob을 이용해 버퍼포인터, 버퍼사이즈를 이용해 _vertexShader를 생성
	HRESULT hr = _graphics->GetDevice()->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(),
		nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}
void Game::CreatePS()
{
	// 쉐이더를 로드해서 _psBlob에 담고
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	// Blob을 이용해 버퍼포인터, 버퍼사이즈를 이용해 _pixelShader를 생성
	HRESULT hr = _graphics->GetDevice()->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(),
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


	HRESULT hr = _graphics->GetDevice()->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
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


	_graphics->GetDevice()->CreateSamplerState(&desc, _samplerState.GetAddressOf());
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



	_graphics->GetDevice()->CreateBlendState(&desc, _blendState.GetAddressOf());
}
void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"Skeleton.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_graphics->GetDevice().Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
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

	HRESULT hr = _graphics->GetDevice()->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
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
