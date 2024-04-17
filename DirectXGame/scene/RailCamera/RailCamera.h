#pragma once

#include "Model.h"

#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "memory"
#include "vector"

class RailCamera {
public:
	void Init(const std::vector<Vector3> controlPoints);
	void Update();
private:
	std::unique_ptr<Model> sphere_;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	Vector3 moveVal_;
	Vector3 radian_;

	size_t count_ = 0;
	std::vector<Vector3> controlPoints_;

public:
	const WorldTransform &getWorldTransform()const { return worldTransform_; }
	const ViewProjection &getViewProjection() const { return viewProjection_; }
};
