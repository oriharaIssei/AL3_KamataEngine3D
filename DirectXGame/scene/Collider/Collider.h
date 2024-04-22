#pragma once

#include "stdint.h"
#include "Vector3.h"

// 衝突属性
const uint32_t kCollisionAttributePlayer = 0b1;
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;

class Collider {
public:
	Collider() = default;
	virtual ~Collider(){};

	virtual void OnCollision() = 0;

protected:
	float radius_ = 4.0f;

	// 自分の衝突属性
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突する対象の属性
	uint32_t collisionMask_ = 0xffffffff;

public:
	const float& getRadius() const { return radius_; }
	void setRadius(float radius) { radius_ = radius; }

	const uint32_t& getCollisionAttribute() const { return collisionAttribute_; }
	void setCollisionAttribute(const uint32_t& collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	const uint32_t& getCollisionMask() const { return collisionMask_; }
	void setCollisionMask(const uint32_t& collisionMask) { collisionMask_ = collisionMask; }

	virtual Vector3 getWorldPos() const = 0;
};
