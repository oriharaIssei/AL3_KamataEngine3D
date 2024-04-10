#include "Enemy.h"

#include "TextureManager.h"
#include <ImGuiManager.h>

void (Enemy::*Enemy::updatesTable[])() = {&Enemy::ApproachUpdate, &Enemy::LeaveUpdate};

void Enemy::Init(const Vector3& pos) {
	model_.reset(Model::Create());

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	phase_ = Phase::Approach;

	th_ = TextureManager::Load("cute_cat.png");
}

void Enemy::Update() {
	(this->*updatesTable[static_cast<size_t>(phase_)])();

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::Text("Position :\nX %.2f \n Y %.2f \n Z %.2f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::Text("Current Phase : %s", phase_ == Phase::Approach ? "Approach" : "Leave");
	ImGui::End();
#endif // _DEBUG
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