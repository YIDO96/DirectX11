#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective,		// 원근 투형 (3d, 원근법에 따라 크기 차이)
	Orthographic,		// 직교 투영 (2d, 화면에 들어온 모든 오브젝트는 같은 크기)
};

class Camera : public Component
{
	using Super = Component;

public:
	Camera();
	~Camera();

	virtual void Update() override;

	void SetProjectionType(ProjectionType type) { _type = type; }
	ProjectionType GetProjetctionType() { return _type; }

	void UpdateMatrix();

private:
	ProjectionType _type = ProjectionType::Orthographic;


public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

