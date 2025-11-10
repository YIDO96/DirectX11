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

	_graphics = make_shared<Graphics>(hwnd);

	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());

	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());
	_geometry = make_shared<Geometry<VertexTextureData>>();
	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), _graphics->GetDeviceContext());

	_texture1 = make_shared<Texture>(_graphics->GetDevice());

	_rasterizerState = make_shared<RasterizerState>(_graphics->GetDevice());
	_samplerState = make_shared<SamplerState>(_graphics->GetDevice());
	_blendState = make_shared<BlendState>(_graphics->GetDevice());




	{ // Create Geometry
		// VertexData
		GeometryHelper::CreateRectangle(_geometry);
		// VertexBuffer
		_vertexBuffer->Create(_geometry->GetVertices());
		// IndexBuffer
		_indexBuffer->Create(_geometry->GetIndices());
	}

	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");						// CreateVS
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());	// CreateInputLayout
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");						// CreatePS

	_rasterizerState->Create();													// CreateRasterizerState
	_samplerState->Create();													// CreateSamplerState
	_blendState->Create();														// CreateBlendState

	_texture1->Create(L"Skeleton.png");											// CreateSRV)

	_constantBuffer->Create();													// CreateConstantBuffer
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


	_constantBuffer->CopyData(_transformData);
}

void Game::Render()
{
	_graphics->RenderBegin();


	// IA - VS - RS - PS - OM
	{
		PipelineInfo info;
		info.inputLayout = _inputLayout;
		info.vertexShader = _vertexShader;
		info.pixelShader = _pixelShader;
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;

		_pipeline->UpdatePipeline(info);


		auto deviceContext = _graphics->GetDeviceContext();

		// IA
		_pipeline->SetVertexBuffer(_vertexBuffer);
		_pipeline->SetIndexBuffer(_indexBuffer);
		// VS
		_pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
		_pipeline->SetTexture(0, SS_PixelShader, _texture1);
		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
		_pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	_graphics->RenderEnd();
}