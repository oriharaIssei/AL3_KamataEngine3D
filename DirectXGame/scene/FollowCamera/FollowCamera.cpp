#include "FollowCamera.h"

#include "Easing.h"

void FollowCamera::Init() {
	viewProj_.Initialize();
}

void FollowCamera::Update(const Input *input) {
	if(followTarget_) {
		constexpr float rotationSpeed = 0.04f;
		destinationAngleY_ += static_cast<float>(input->PushKey(DIK_LEFT) - input->PushKey(DIK_RIGHT)) * rotationSpeed;
		if(input->TriggerKey(DIK_RSHIFT)) {
			destinationAngleY_ = 0;
		}

		viewProj_.rotation_.y = Lerp(0.1f,viewProj_.rotation_.y,destinationAngleY_);

		Vector3 offset = offsetVector();
		interTarget_ = Lerp(0.1f,followTarget_->translation_,interTarget_);

		viewProj_.translation_ = offset + followTarget_->translation_;
	}
	viewProj_.UpdateMatrix();
	viewProj_.TransferMatrix();
}

void FollowCamera::Reset() {
	if(followTarget_) {
		interTarget_ = followTarget_->translation_;
		viewProj_.rotation_.y = followTarget_->rotation_.y;
	}
	Vector3 offset = offsetVector();
	viewProj_.translation_ = interTarget_ + offset;
}

Vector3 FollowCamera::offsetVector() {
	Vector3 offset = {0.0f,8.0f,-20.0f};
	offset = TransformVector(offset,MakeMatrix::RotateXYZ(viewProj_.rotation_));
	return offset;
}

void FollowCamera::SetTarget(const WorldTransform *target) {
	followTarget_ = target;
	Reset();
}
