#include "Enemy.h"

#include "TextureManager.h"
#include <ImGuiManager.h>

#pragma region "Enemy"
void Enemy::Init(const Vector3& pos) {
	model_.reset(Model::Create());

	state_.reset(new EnemyStateApproach(this));

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	th_ = TextureManager::Load("cute_cat.png");
}

void Enemy::Update() {
	// 現在のステートで更新処理
	if (state_ != nullptr) {
		state_->Update();
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProj) { model_->Draw(worldTransform_, viewProj, th_); }

void Enemy::ChangeState(BaseEnemyState* nextState) { state_.reset(nextState); }

#pragma endregion

#pragma region "Enemy States"

void EnemyStateApproach::Update() {
	//========================================
	//	移動
	host_->setPos(velocity + host_->getPos());

	//========================================
	if (host_->getPos().z <= 1.0f) {
		// host は同じ
		host_->ChangeState(new EnemyStateLeave(host_));
	}
}

void EnemyStateLeave::Update() {
	//========================================
	//	移動
	host_->setPos(velocity + host_->getPos());
}
#pragma endregion
