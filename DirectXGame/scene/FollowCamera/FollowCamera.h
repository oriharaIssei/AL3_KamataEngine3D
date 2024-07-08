#pragma once

#include "Input.h"

#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {
public:
	void Init();
	void Update(const Input *input);
private:
	void Reset();
	Vector3 offsetVector();
private:
	ViewProjection viewProj_;
	float destinationAngleY_;
	Vector3 interTarget_;

	const WorldTransform *followTarget_;
public:
	const ViewProjection &getViewProjection()const { return viewProj_; }
	void SetTarget(const WorldTransform *target);
};