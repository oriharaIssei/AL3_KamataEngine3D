#include "Enemy.h"

#include "TextureManager.h"

void Enemy::Init(const Vector3& pos) {
	model_.reset(Model::Create());

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	phase_ = Phase::Approach;

	th_ = TextureManager::Load("cute_cat.png");
}

void Enemy::Update() {
	switch (phase_) {
	case Phase::Approach:
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	default:
		break;
	}
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProj) { model_->Draw(worldTransform_, viewProj, th_); }

void Enemy::ApproachUpdate() {
	//========================================
		//	移動
	worldTransform_.translation_ += {0.0f, 0.0f, -kSpeed_};

	//========================================
	if (worldTransform_.translation_.z <= 1.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	//========================================
	//	移動
	worldTransform_.translation_ += {kSpeed_, kSpeed_, -kSpeed_};
	//========================================
}