// Struct.h

#pragma once
#include "Types.h"

// ���� ����ü ����

struct Vertex
{
	Vec3 position;
	//Color color;
	Vec2 uv;
};

struct TransformData
{
	Vec3 offset;
	float dummy;
};