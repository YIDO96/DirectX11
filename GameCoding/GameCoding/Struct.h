// Struct.h

#pragma once
#include "Types.h"

// 공용 구조체 정의

//struct Vertex
//{
//	Vec3 position;
//	//Color color;
//	Vec2 uv;
//};

struct CameraData
{
	Matrix matView = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
};

struct TransformData
{
	Matrix matWorld = Matrix::Identity;
};