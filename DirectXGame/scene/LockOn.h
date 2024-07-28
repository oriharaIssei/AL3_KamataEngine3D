#pragma once

#include <memory>

#include <vector>

#include "ViewProjection.h"

#include "Sprite.h"

#include "Enemy/Enemy.h"

#include <numbers>

#include <Input.h>

class LockOn{
public:
	void Init();
	void Update(const std::vector<std::unique_ptr<Enemy>> &enemies_,
				const ViewProjection &viewProj);
	void Draw();

private:
	Input *input_;

	const Enemy *target = nullptr;

	std::unique_ptr<Sprite> sprite_;

	float minDistance_;
	float maxDistance_;
	float angleRange_ = std::numbers::pi_v<float>;

	bool isLocked_;
public:
	Vector3 getTargetPos()const{ return target->getWorldTransform().matWorld_[3]; }
	bool ExistTarget()const{ return target?true:false; }
};