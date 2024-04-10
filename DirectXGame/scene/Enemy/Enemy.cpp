#include "Enemy.h"

#include "TextureManager.h"
#include <ImGuiManager.h>

#pragma region "Enemy"

const uint32_t Enemy::kFierInterval = 60;

void Enemy::Init(const Vector3& pos) {
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
	timedCalls_.remove_if([](std::unique_ptr<TimedCall<void,void>>& caller) { return caller->getIsOccured(); });
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

	std::unique_ptr<TimedCall<void,void>> timedCaller = std::make_unique<TimedCall<void,void>>(std::bind(&Enemy::InitOnApproach,this),Enemy::kFierInterval);

	timedCalls_.push_back(std::move(timedCaller));
}

void Enemy::Fire() {
	bullets_.push_back(std::make_unique<EnemyBullet>());
	// 速度 と 向き を合わせる(回転させる)
	Vector3 velocity = TransformNormal({0.0f, 0.0f, -0.3f}, worldTransform_.matWorld_);
	bullets_.back()->Init(Model::Create(), worldTransform_.translation_, velocity);
}

void Enemy::UpdateTimedCalls() {
	for (auto& caller:timedCalls_) {
		caller->Update();
	}
}



#pragma endregion

#pragma region "Enemy States"

void EnemyStateApproach::Update() {
	//========================================
	//	移動
	host_->setPos(velocity + host_->getPos());
	//========================================

	host_->UpdateTimedCalls();

	if (host_->getPos().z <= 1.0f) {
		host_->ClearTimedCalls();
		// host は同じ
		host_->ChangeState(new EnemyStateLeave(host_));
	}
}

void EnemyStateLeave::Update() {
	//========================================
	//	移動
	host_->setPos(velocity + host_->getPos());
	//========================================
}
#pragma endregion
