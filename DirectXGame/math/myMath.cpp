#include "myMath.h"

#include <algorithm>
#include <assert.h>
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

	return start * std::cosf(slerpedTheta) + relativeVec * sinf(slerpedTheta);
}

Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	constexpr float half = 0.5f;

	float t2 = t * t;
	float t3 = t2 * t;

	Vector3 e3 = -p0 + 3.0f * p1 - 3.0f * p2 + p3;
	Vector3 e2 = 2 * p0 - 5 * p1 + 4 * p2 - p3;
	Vector3 e1 = -p0 + p2;
	Vector3 e0 = 2 * p1;

	return half * (e3 * t3 + e2 * t2 + e1 * t + e0);
}

Vector3 CatmullRomInterpolation(const std::vector<Vector3>& points, float t) {
	assert(points.size() >= 4 && "制御点は 4点以上必要です");
	size_t division = points.size() - 1;
	float areaWidth = 1.0f / division;

	float t_2 = std::fmod(t, areaWidth) * division;
	t_2 = std::clamp(t_2, 0.0f, 1.0f);

	size_t index = static_cast<size_t>(t / areaWidth);
	index = std::min(index, division - 1); 

	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	if (index == 0) {
		index0 = index1;
	}
	if (index3 >= points.size()) {
		index3 = index2;
	}

	Vector3 p0 = points[index0];
	Vector3 p1 = points[index1];
	Vector3 p2 = points[index2];
	Vector3 p3 = points[index3];

	return CatmullRomInterpolation(p0,p1,p2,p3,t_2);
}