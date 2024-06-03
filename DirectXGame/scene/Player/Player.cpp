#include "Player.h"

#include "TextureManager.h"

void Player::Init() {
	model_.reset(Model::CreateFromOBJ("Player"));

	worldTransform_.Initialize();

	input_ = Input::GetInstance();
}

void Player::Update() {
	move_ = {static_cast<float>(input_->PushKey(DIK_D) - input_->PushKey(DIK_A)),0.0f,static_cast<float>(input_->PushKey(DIK_W) - input_->PushKey(DIK_S))};

	move_ = move_.Normalize() * speed_;
	if(viewProjection_) {
		move_ = TransformVector(move_,MakeMatrix::RotateXYZ(viewProjection_->rotation_));
	}
	if(move_.length() > 0.1f) {
		lastDir_ = move_.Normalize();
	}

	worldTransform_.rotation_.y = atan2(lastDir_.x,lastDir_.z);
	worldTransform_.translation_ += move_;
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection &viewProj) {
	model_->Draw(worldTransform_,viewProj);
}