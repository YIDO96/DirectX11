#pragma once


#include "Types.h"
#include "Values.h"
#include "Struct.h"

// STL
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <list>
#include <map>
#include <unordered_map>
#include <memory>
using namespace std;

// WIN
#include <Windows.h>
#include <assert.h>

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif


#define CHECK(p)	assert(SUCCEEDED(p))

#define		GAME	GGame
#define		SCENE		GAME->GetSceneManager()
#define		TIME		GAME->GetTimeManager()
#define		INPUT		GAME->GetInputManager()
#define		RESOURCES	GAME->GetResourceManager()
#define		RENDER		GAME->GetRenderManager()


// Engine
#include "Graphics.h"
// Engine - Pipelines - InputAssembler
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
// Engine - Pipelines - Geometry
#include "Geometry.h"
#include "GeometryHelper.h"
#include "VertexData.h"
// Engine - Pipelines - Shaders
#include "ShaderBase.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "BlendState.h"
// Engine - Pipeline
#include "Pipeline.h"

// Engine - Game
#include "Transform.h"