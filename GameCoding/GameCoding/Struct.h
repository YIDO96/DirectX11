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

struct TransformData
{
	//Vec3 offset;
	//float dummy;

	// Matrix::Identity = 항등행렬
	Matrix matWorld = Matrix::Identity;
	Matrix matView = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
};