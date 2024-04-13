#pragma once

#include "Model.h"

#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>

class Skydome {
public:
	void Init();
	void Update();
	void Draw(const ViewProjection& viewProj);

private:
	WorldTransform worldTransform_;

	std::unique_ptr<Model> model_ = nullptr;
};
