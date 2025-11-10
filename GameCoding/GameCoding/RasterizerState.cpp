#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::Create()
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