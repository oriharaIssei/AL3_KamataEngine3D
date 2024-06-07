#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "memory"

#include "Model.h"

struct ModelSet {
	std::unique_ptr<Model> model;

	WorldTransform worldTransform;

	void Draw(const ViewProjection &viewProj) {
		model->Draw(worldTransform,viewProj);
	}
};