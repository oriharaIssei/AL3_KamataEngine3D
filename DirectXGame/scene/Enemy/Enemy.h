#pragma once

#include "Model.h"

#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>

class Enemy {
public:
	void Init(const Vector3& pos);
	void Update();
	void Draw(const ViewProjection& viewProj);

private:
	WorldTransform worldTransform_;
	const float kSpeed_ = 0.2f;

	std::unique_ptr<Model> model_ = nullptr;

	uint32_t th_;
};
