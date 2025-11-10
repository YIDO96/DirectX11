#include "pch.h"
#include "BlendState.h"

BlendState::BlendState(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

BlendState::~BlendState()
{
}

void BlendState::Create(D3D11_RENDER_TARGET_BLEND_DESC blendDesc, float factor)
{
	_blendFactor = factor;

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;		// 알파 투 커버리지 비활성화
	desc.IndependentBlendEnable = false;	// 독립 블렌딩 비활성화

	desc.RenderTarget[0] = blendDesc;

	//desc.RenderTarget[0].BlendEnable = true; // 블렌딩 비활성화
	//desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // SrcFactor
	//desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // DstFactor
	//desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // 합산
	//desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	//desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // RGBA 모두 쓰기

	_device->CreateBlendState(&desc, _blendState.GetAddressOf());
}