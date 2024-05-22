#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Model.h"

#include "memory"

#include <stdint.h>

class Player {
public:
	void Init();
	void Update();
	void Draw(const ViewProjection &viewProj);

private:
	WorldTransform worldTransform_;

	uint32_t textureHandle_;

	std::unique_ptr<Model> model_;
};

