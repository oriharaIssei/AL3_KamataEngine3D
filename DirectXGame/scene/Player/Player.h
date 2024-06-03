#pragma once

#include "Input.h"

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Model.h"

#include "memory"

#include "Vector2.h"
#include <stdint.h>

class Player {
public:
	void Init();
	void Update();
	void Draw(const ViewProjection &viewProj);

private:
	Input *input_ = nullptr;
	float speed_ = 0.4f;

	std::unique_ptr<Model> model_;

	const ViewProjection *viewProjection_ = nullptr;
	WorldTransform worldTransform_;

	Vector3 lastDir_;
	Vector3 move_;
public:
	const WorldTransform &getWorldTransform()const { return worldTransform_; }
	void setViewProjection(const ViewProjection *viewProj) { viewProjection_ = viewProj; }
};

