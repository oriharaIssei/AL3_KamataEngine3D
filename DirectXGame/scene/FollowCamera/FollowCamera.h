#pragma once

#include "Input.h"

#include "ViewProjection.h"
#include "WorldTransform.h"

class LockOn;
class FollowCamera{
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

	const LockOn *lockOn_;

	const WorldTransform *followTarget_;
public:
	void SetLockOn(const LockOn *lockOn){ lockOn_ = lockOn; }
	const ViewProjection &getViewProjection()const{ return viewProj_; }
	void SetTarget(const WorldTransform *target);
};