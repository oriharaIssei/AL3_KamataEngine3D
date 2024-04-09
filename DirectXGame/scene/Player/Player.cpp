#include "Player.h"

#include "ImGuiManager.h"
#include <cassert>

Player::~Player() {
	for (auto& bullet : bullets_) {
		bullet.release();
	}
}

void Player::Init(Model* model, uint32_t textureHandle) {
	input_ = Input::GetInstance();
	assert(model);
	model_.reset(model);

	th_ = textureHandle;

	worldTransform_.Initialize();
}

void Player::Update() {
	//===============================================
	// 古い弾の削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead() ? true : false; });
	//===============================================

	//===============================================
	// キーボード入力 による 移動

	// 旋回
	Rotate();

	// 移動
	Vector3 move = {0.0f, 0.0f, 0.0f};
	// x 軸
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kSpeed;
	}
	// y 軸
	if (input_->PushKey(DIK_UP)) {
		move.y += kSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kSpeed;
	}

	worldTransform_.translation_ += move;

	//===============================================

	//===============================================
	// キャラクター攻撃処理
	Attack();
	//===============================================

	//===============================================
	// imgui による座標の表示
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::Text("Position :\nX %.2f \n Y %.2f \n Z %.2f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();
#endif // _DEBUG
	//===============================================

	//===============================================
	// 移動の制限
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -30.0f);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, 30.0f);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -16.0f);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, 16.0f);

	//===============================================

	//===============================================
	// worldTransform を更新
	worldTransform_.UpdateMatrix();
	//===============================================
}

void Player::Draw(const ViewProjection& viewProj) {
	model_->Draw(worldTransform_, viewProj, th_);
	// 弾があれば描画
	for (auto& bullet : bullets_) {
		bullet->Draw(viewProj);
	}
}

void Player::Rotate() {
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed_;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed_;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_W)) {
		bullets_.push_back(std::make_unique<PlayerBullet>());
		// 速度 と player の 向き を合わせる(回転させる)
		Vector3 velocity = TransformNormal({0.0f, 0.0f, kBuletSpeed_}, worldTransform_.matWorld_);
		bullets_.back()->Init(Model::Create(), worldTransform_.translation_, velocity);
	}

	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}
