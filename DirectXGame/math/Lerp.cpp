#include "Lerp.h"

#include <algorithm>
#include <cmath>

float Lerp(const float& t, const float& start, const float& end) { return ((1.0f - t) * start) + (t * end); }

Vector2 Lerp(const float& t, const Vector2& start, const Vector2& end) { return Vector2(Lerp(t, start.x, end.y), Lerp(t, start.y, end.y)); }

Vector3 Lerp(const float& t, const Vector3& start, const Vector3& end) { return Vector3(Lerp(t, start.x, end.y), Lerp(t, start.y, end.y), Lerp(t, start.z, end.z)); }

Vector3 Slerp(const float& t, const Vector3& start, const Vector3& end) {
	// cosθを計算
	float dot = start.dot(end);
	
	if (dot > 0.9995 || dot < -0.9995) {
		return Lerp(t, start, end).Normalize();
	}

	// θを計算し、sinθで割ることで補間を正規化
	float slerpedTheta = acos(dot) * t;
	Vector3 relativeVec = (end - start * dot).Normalize();

	return start * cosf(slerpedTheta) + relativeVec * sinf(slerpedTheta);
}