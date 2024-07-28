#pragma once

#include "Collision/Collider.h"

#include "Model.h"

#include "ViewProjection.h"

#include "memory"

#include <list>

class CollisionManager{
public:
	void Init();
	void Update();
	void Draw(const ViewProjection &viewProj);
private:
	bool CheckCollisionPair(const Collider *a,const Collider *b);
private:
	std::list<Collider *> colliders_;
	std::unique_ptr<Model> model_;
	bool isDrawColliders_ = false;
public:
	void AddPushBackCollider(Collider *collider){ colliders_.push_back(collider); }
	void Reset(){ colliders_.clear(); }
};