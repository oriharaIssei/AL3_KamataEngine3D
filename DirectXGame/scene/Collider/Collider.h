#pragma once

#include "Vector3.h"

class Collider {
public:
	Collider() = default;
	virtual ~Collider(){};

	virtual void OnCollision() = 0;

protected:
	float radius_ = 4.0f;

public:
	const float& getRadius() const { return radius_; }
	void setRadius(float radius) { radius_ = radius; }

	virtual const Vector3& getWorldPos()const = 0;
};
