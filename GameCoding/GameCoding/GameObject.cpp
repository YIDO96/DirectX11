#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: _device(device)
{
	// 바로 사각형으로 만들어두기
	_geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometry);

	// 그 도형을 GPU에게 버퍼를 만들어줘
	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_vertexBuffer->Create(_geometry->GetVertices());

	_indexBuffer = make_shared<IndexBuffer>(_device);
	_indexBuffer->Create(_geometry->GetIndices());

	// Layout과 Shader 생성
	_vertexShader = make_shared<VertexShader>(_device);
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");						// CreateVS

	// GPU와 쉐이더랑 연결
	_inputLayout = make_shared<InputLayout>(_device);
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());	// CreateInputLayout

	// PixelShader 생성
	_pixelShader = make_shared<PixelShader>(_device);
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");						// CreatePS

	// RasterizerState 생성
	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();													// CreateRasterizerState

	// BlendState 생성
	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();														// CreateBlendState

	// ConstantBuffer 생성
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_device, deviceContext);
	_constantBuffer->Create();

	// Texture 생성
	_texture1 = make_shared<Texture>(_device);
	_texture1->Create(L"Skeleton.png");											// CreateSRV)

	// SamplerState 생성
	_samplerState = make_shared<SamplerState>(_device);	
	_samplerState->Create();													// CreateSamplerState
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	//Scale Rotation Translation
	_localPosition.x += 0.0005f;

	Matrix matScale = Matrix::CreateScale(_localScale / 3);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	Matrix matWorld = matScale * matRotation * matTranslation;
	_transformData.matWorld = matWorld;


	_constantBuffer->CopyData(_transformData);
}

void GameObject::Render(shared_ptr<Pipeline> pipeline)
{
	PipelineInfo info;
	info.inputLayout = _inputLayout;
	info.vertexShader = _vertexShader;
	info.pixelShader = _pixelShader;
	info.rasterizerState = _rasterizerState;
	info.blendState = _blendState;

	pipeline->UpdatePipeline(info);

	// IA
	pipeline->SetVertexBuffer(_vertexBuffer);
	pipeline->SetIndexBuffer(_indexBuffer);
	// VS
	pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
	pipeline->SetTexture(0, SS_PixelShader, _texture1);
	pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
	pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
}