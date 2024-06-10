#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "memory"

#include "Model.h"

struct PartsModel {
	void Init(Model *parts) {
		model.reset(parts);
		worldTransform.Initialize();
	}
	std::unique_ptr<Model> model;

	WorldTransform worldTransform;

	void Draw(const ViewProjection &viewProj) {
		model->Draw(worldTransform,viewProj);
	}
};