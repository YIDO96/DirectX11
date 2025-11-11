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
	HWND _hwnd;					// 윈도우 핸들

	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;

	shared_ptr<GameObject> _monster;
	shared_ptr<GameObject> _camera;
};

