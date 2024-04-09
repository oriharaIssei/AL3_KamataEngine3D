#include "Player.h"

#include "ImGuiManager.h"
#include "Input.h"

#include <cassert>

void Player::Init(Model* model, uint32_t textureHandle) {
	assert(model);
	model_.reset(model);

	th_ = textureHandle;

	worldTransform_.Initialize();
}

void Player::Update() {
	Input* input = Input::GetInstance();

	//===============================================
	// キーボード入力 による 移動
	Vector3 move = {0.0f, 0.0f, 0.0f};
	// x 軸
	if (input->PushKey(DIK_LEFT)) {
		move.x -= kSpeed;
	} else if (input->PushKey(DIK_RIGHT)) {
		move.x += kSpeed;
	}
	// y 軸
	if (input->PushKey(DIK_UP)) {
		move.y += kSpeed;
	} else if (input->PushKey(DIK_DOWN)) {
		move.y -= kSpeed;
	}

	worldTransform_.translation_ += move;

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
	worldTransform_.translation_.x = max(worldTransform_.translation_.x,-30.0f);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, 30.0f);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -16.0f);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, 16.0f);

	//===============================================

	//===============================================
	// worldTransform を計算
	worldTransform_.matWorld_ = MakeMatrix::Affine(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	//===============================================
}

void Player::Draw(const ViewProjection& viewProj) { model_->Draw(worldTransform_, viewProj, th_); }
