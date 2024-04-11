#pragma once

#include "Vector2.h"
#include "Vector3.h"

float Lerp(const float& t, const float& start, const float& end);
Vector2 Lerp(const float& t, const Vector2& start, const Vector2& end);
Vector3 Lerp(const float& t, const Vector3& start, const Vector3& end);

Vector3 Slerp(const float& t, const Vector3& start, const Vector3& end);
