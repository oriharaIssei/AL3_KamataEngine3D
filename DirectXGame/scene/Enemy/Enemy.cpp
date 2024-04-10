#include "Enemy.h"

#include "TextureManager.h"

void Enemy::Init(const Vector3& pos) {
	model_.reset(Model::Create());

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	th_ = TextureManager::Load("cute_cat.png");
}

void Enemy::Update() {
	//========================================
	//	移動
	worldTransform_.translation_ -= {0.0f, 0.0f, kSpeed_};

	worldTransform_.UpdateMatrix();
	//========================================
}

void Enemy::Draw(const ViewProjection& viewProj) { model_->Draw(worldTransform_, viewProj, th_); }
