#pragma once

#include <ViewProjection.h>
#include <WorldTransform.h>

#include <memory>

#include <Model.h>

class Skydome {
public:
	void Init();
	void Draw(const ViewProjection &viewProj);
private:
	std::unique_ptr<Model> model_;
	WorldTransform worldTransform_;
};