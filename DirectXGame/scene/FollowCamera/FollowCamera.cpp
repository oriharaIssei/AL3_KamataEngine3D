#include "FollowCamera.h"

void FollowCamera::Init() {
	viewProj_.Initialize();

}

void FollowCamera::Update(const Input *input) {
	if(followTarget_) {
		Vector3 offset = {0.0f,8.0f,-20.0f};

		constexpr float rotationSpeed = 0.04f;
		viewProj_.rotation_.y += static_cast<float>(input->PushKey(DIK_LEFT) - input->PushKey(DIK_RIGHT)) * rotationSpeed;

		offset = TransformVector(offset,MakeMatrix::RotateXYZ(viewProj_.rotation_));

		viewProj_.translation_ = offset + followTarget_->translation_;
	}
	viewProj_.UpdateMatrix();
	viewProj_.TransferMatrix();
}
