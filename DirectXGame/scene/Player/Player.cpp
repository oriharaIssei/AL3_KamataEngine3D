#include "Player.h"

#include "cassert"

void Player::Init(Model* model,uint32_t textureHandle) {
	assert(model);
	model_.reset(model);

	th_ = textureHandle;

	worldTransform_.Initialize();
}

void Player::Update() {}

void Player::Draw(const ViewProjection& viewProj) { model_->Draw(worldTransform_, viewProj, th_); }
