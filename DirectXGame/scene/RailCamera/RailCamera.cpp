#include "RailCamera.h"

#include "myMath.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

void RailCamera::Init(const std::vector<Vector3> controlPoints) {
	controlPoints_ = controlPoints;

	worldTransform_.translation_ = controlPoints[0];

	viewProjection_.Initialize();
}

void RailCamera::Update() {
	constexpr float dimention = 600;
	if(count_++ > dimention) {
		count_ = 0;
	}

	float t = 1.0f / dimention * count_;
	Vector3 eye = CatmullRomInterpolation(controlPoints_, t);
	float nextT = 1.0f / dimention * ((count_ + 1 >= dimention ? count_ - dimention + 1 : count_ + 1));

	Vector3 target = CatmullRomInterpolation(controlPoints_, nextT);

	Vector3 direction = (target - eye).Normalize();
	worldTransform_.rotation_.y = std::atan2(direction.x, direction.z);
	Vector2 veloXZ = { direction.x,direction.z };
	worldTransform_.rotation_.x = std::atan2(-direction.y, veloXZ.Length());

	worldTransform_.translation_ = eye;

	worldTransform_.matWorld_ = MakeMatrix::Affine(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = worldTransform_.matWorld_.Inverse();
}