#include "Player.h"

#include <TextureManager.h>

void Player::Init() {
	model_.reset(Model::Create());
	textureHandle_ = TextureManager::Load("./Resources/uvChecker.png");

	worldTransform_.Initialize();
}

void Player::Update() {
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection &viewProj) {
	model_->Draw(worldTransform_, viewProj, textureHandle_);
}