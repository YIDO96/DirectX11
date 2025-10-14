#pragma once


// DirectX11 �������� �����ϴ� ���� ������ �ּ� ���� Ŭ����
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);		// �ʱ�ȭ : device, swapchain, RTV, viewport �ʱ�ȭ
	void Update();				// �� ������ ���� ���� (�Է�, ���ӻ��� ������Ʈ ��)	
	void Render();				// �� ������ ������

private:
	void RenderBegin();			// ���� Ÿ�� ���ε�, Ŭ����, ����Ʈ ����
	void RenderEnd();			// ȭ�� ǥ�� (Present)


private:
	void CreateDeviceAndSwapChain();		// D3D11 Device, DeviceContext ,SwapChain ����
	void CreateRenderTargertView();			// ����۷κ��� RTV ����
	void SetViewport();						// ����Ʈ ����

private:
	HWND _hwnd;					// ������ �ڵ�
	uint32 _width = 0;			// ������ â ũ��
	uint32 _height = 0;			// ������ â ũ��

private:
	// Device & SwapChain
	ComPtr<ID3D11Device> _device = nullptr;					//	GPU ����̽� (���ν� ���� ����)
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;	// 	����̽� ���ؽ�Ʈ (������ ��� ����)
	ComPtr<IDXGISwapChain> _swapChain = nullptr;			//	spawChain (ȭ�鿡 ������ ���� ��ü ����)

	// RTV 
	ComPtr<ID3D11RenderTargetView> _renderTargetView;		//	�ĸ� ���۸� �׸� �� �ְ� �ϴ� ��

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.f, 0.f, 0.f, 0.f };
};

