#include "Enemy.h"
#include "Player.h"

#include "TextureManager.h"
#include <ImGuiManager.h>

#pragma region "Enemy"

const uint32_t Enemy::kFierInterval = 60;

void Enemy::Init(const Vector3& pos, Player* player) {
	SetPlayer(player);

	model_.reset(Model::Create());

	state_.reset(new EnemyStateApproach(this));

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	InitOnApproach();

	th_ = TextureManager::Load("cute_cat.png");
}

void Enemy::Update() {
	//===============================================
	// 古い要素の削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead() ? true : false; });
	timedCalls_.remove_if([](std::unique_ptr<TimedCall<void, void>>& caller) { return caller->getIsOccured(); });
	//===============================================

	//===============================================
	// 現在のステートで更新処理
	if (state_ != nullptr) {
		state_->Update();
	}
	//===============================================

	//===============================================
	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
	//===============================================

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProj) {
	model_->Draw(worldTransform_, viewProj, th_);
	for (auto& bullet : bullets_) {
		bullet->Draw(viewProj);
	}
}

void Enemy::ChangeState(BaseEnemyState* nextState) { state_.reset(nextState); }

void Enemy::InitOnApproach() {
	Fire();

	std::unique_ptr<TimedCall<void, void>> timedCaller = std::make_unique<TimedCall<void, void>>(std::bind(&Enemy::InitOnApproach, this), Enemy::kFierInterval);

	timedCalls_.push_back(std::move(timedCaller));
}

void Enemy::Fire() {
	bullets_.push_back(std::make_unique<EnemyBullet>());
	// player のいる方向に撃つ
	bullets_.back()->Init(Model::Create(), worldTransform_.translation_, player_ptr);
}

void Enemy::UpdateTimedCalls() {
	for (auto& caller : timedCalls_) {
		caller->Update();
	}
}

#pragma endregion

#pragma region "Enemy States"

void EnemyStateApproach::Update() {
	//========================================
	//	移動
	host_->setPos(velocity + host_->getWorldPos());
	//========================================

	host_->UpdateTimedCalls();

	if (host_->getWorldPos().z <= 1.0f) {
		host_->ClearTimedCalls();
		// host は同じ
		host_->ChangeState(new EnemyStateLeave(host_));
	}
}

void EnemyStateLeave::Update() {
	//========================================
	//	移動
	host_->setPos(velocity + host_->getWorldPos());
	//========================================
}
#pragma endregion
