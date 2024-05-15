#include "Enemy.h"

#include "GameScene.h"
#include "Player.h"

#include "TextureManager.h"
#include <ImGuiManager.h>

#pragma region "Enemy"

const uint32_t Enemy::kFierInterval = 60;

void Enemy::Init(const Vector3 &pos, Player *player, GameScene *gameScene) {
	isDead_ = false;

	gameScene_ptr = gameScene;
	SetPlayer(player);

	model_.reset(Model::Create());

	state_.reset(new EnemyStateApproach(this));

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	InitOnApproach();

	th_ = TextureManager::Load("cute_cat.png");

	setCollisionAttribute(kCollisionAttributeEnemy);
	setCollisionMask(~kCollisionAttributeEnemy);
}

void Enemy::Update() {
	//===============================================
	// 古い要素の削除
	timedCalls_.remove_if([](std::unique_ptr<TimedCall<void, void>> &caller) { return caller->getIsOccured(); });
	//===============================================

	//===============================================
	// 現在のステートで更新処理
	if(state_ != nullptr) {
		state_->Update();
	}
	//===============================================

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection &viewProj) {
	model_->Draw(worldTransform_, viewProj, th_);
}

void Enemy::ChangeState(BaseEnemyState *nextState) { state_.reset(nextState); }

void Enemy::InitOnApproach() {
	Fire();

	std::unique_ptr<TimedCall<void, void>> timedCaller = std::make_unique<TimedCall<void, void>>(std::bind(&Enemy::InitOnApproach, this), Enemy::kFierInterval);

	timedCalls_.push_back(std::move(timedCaller));
}

void Enemy::Fire() {
	EnemyBullet* bullet = new EnemyBullet();
	bullet->Init(Model::Create(), this->getWorldPos(), player_ptr);
	gameScene_ptr->setEnemyBullet(bullet);
}

void Enemy::UpdateTimedCalls() {
	for(auto &caller : timedCalls_) {
		caller->Update();
	}
}

Vector3 Enemy::getWorldPos() const {
	Vector3 pos = Transform({ 0.0f,0.0f,0.0f }, worldTransform_.matWorld_);
	return pos;
}

#pragma endregion

#pragma region "Enemy States"

void EnemyStateApproach::Update() {
	//========================================
	//	移動
	host_->setPos(velocity + host_->getTranslation());
	//========================================

	host_->UpdateTimedCalls();

	if(host_->getWorldPos().z <= -2.0f) {
		host_->ClearTimedCalls();
		// host は同じ
		host_->ChangeState(new EnemyStateLeave(host_));
	}
}

void EnemyStateLeave::Update() {
	//========================================
	//	移動
	host_->setPos(velocity + host_->getTranslation());
	//========================================
}
#pragma endregion
