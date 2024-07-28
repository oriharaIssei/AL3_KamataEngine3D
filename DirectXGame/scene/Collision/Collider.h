#pragma once

#include <functional>

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Model.h"

#include "Vector3.h"
#include <stdint.h>

class Collider{
public:
	Collider() = default;
	void Init(const Vector3 &pos,float radius,std::function<void(Collider *a)> onCollisionFunc);
	void Update();
	void Draw(Model *model,const ViewProjection &viewProj);
	void OnCollision(Collider *collider){ onCollision_(collider); }
private:
	std::function<void(Collider *a)> onCollision_;

	float radius_;
	WorldTransform transform_;

	uint32_t typeID_ = 0u;
public:
	Vector3 getPosition()const{ return transform_.matWorld_[3]; }
	float getRadius()const{ return radius_; }
	void setTransformParent(WorldTransform *parent){ transform_.parent_ = parent; transform_.UpdateMatrix(); }

	uint32_t getTypeID()const{ return typeID_; }
	void setTypeID(uint32_t typeID){ typeID_ = typeID; }
};