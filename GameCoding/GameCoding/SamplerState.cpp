#include "pch.h"
#include "SamplerState.h"

SamplerState::SamplerState(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

SamplerState::~SamplerState()
{
}

void SamplerState::Create()
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