#include "CollisionManager.h"

void CollisionManager::Init() {}

void CollisionManager::Update() {
	std::vector<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		std::vector<Collider*>::iterator itrB = itrA;
		itrB++; // itrAの次のポインタを指すようにする
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			CheckColliderPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckColliderPair(Collider* colliderA, Collider* colliderB) {
	if ((colliderA->getCollisionAttribute() & colliderB->getCollisionMask()) == 0 || (colliderB->getCollisionAttribute() & colliderA->getCollisionMask()) == 0) {
		return;
	}

	posA = colliderA->getWorldPos();
	posB = colliderB->getWorldPos();
	distance = powf((posA.x - posB.x), 2.0f) + powf((posA.y - posB.y), 2.0f) + powf((posA.z - posB.z), 2.0f);
	if (distance <= powf(colliderA->getRadius() + colliderB->getRadius(), 2.0f)) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}
