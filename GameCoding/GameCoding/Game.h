#pragma once

#include "Graphics.h"
#include "GameObject.h"
#include "Pipeline.h"

class SceneManager;
class InputManager;
class TimeManager;
class ResourceManager;
class RenderManager;

// DirectX11 렌더링을 관리하는 게임 엔진의 최소 단위 클래스
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);		// 초기화 : device, swapchain, RTV, viewport 초기화
	void Update();				// 매 프레임 갱신 로직 (입력, 게임상태 업데이트 등)	
	void Render();				// 매 프레임 렌더링


	shared_ptr<SceneManager> GetSceneManager() { return _scene; }
	shared_ptr<TimeManager> GetTimeManager() { return _time; }
	shared_ptr<InputManager> GetInputManager() { return _input; }
	shared_ptr<ResourceManager> GetResourceManager() { return _resource; }
	shared_ptr<RenderManager> GetRenderManager() { return _render; }


private:
	HWND _hwnd;					// 윈도우 핸들

	shared_ptr<Graphics> _graphics;

private:
	shared_ptr<SceneManager> _scene;
	shared_ptr<TimeManager> _time;
	shared_ptr<InputManager> _input;
	shared_ptr<ResourceManager> _resource;
	shared_ptr<RenderManager> _render;
};

extern unique_ptr<Game> GGame;