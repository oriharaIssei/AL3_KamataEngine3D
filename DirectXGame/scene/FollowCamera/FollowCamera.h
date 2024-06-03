#pragma once

#include "Input.h"

#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {
public:
	void Init();
	void Update(const Input* input);
private:
	ViewProjection viewProj_;

	const WorldTransform *followTarget_;
public:
	const ViewProjection &getViewProjection()const { return viewProj_; }
	void SetTarget(const WorldTransform *target) { followTarget_ = target; }
};