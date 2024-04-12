#pragma once

#include "Collider.h"

#include <vector>

class CollisionManager {
public:
	void Init();
	void Update();

private:
	void CheckColliderPair(Collider* colliderA, Collider* colliderB);

private:
	float distance;
	Vector3 posA;
	Vector3 posB;

	std::vector<Collider*> colliders_;
public:
	void ClearColliders() { colliders_.clear(); }
	void setCollider(Collider* colliders) { colliders_.push_back(colliders); }
};
