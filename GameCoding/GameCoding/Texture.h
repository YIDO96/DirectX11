#pragma once
#include "ResourceBase.h"


class Texture : public ResourceBase
{
	using Super = ResourceBase;

public:
	Texture(ComPtr<ID3D11Device> device);
	~Texture();

	ComPtr<ID3D11ShaderResourceView> GetComPtr() const { return _shaderResourceView; }
	Vec2 GetSize() { return _size; }

	void Create(const wstring& path);

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

	Vec2 _size = { 0.f, 0.f };
};