#include "Player.h"

#include "ImGuiManager.h"
#include "TextureManager.h"
#include <WinApp.h>

#include <cassert>

Player::~Player() {
	for(auto &bullet : bullets_) {
		bullet.release();
	}
}

void Player::Init(const Vector3 &pos, Model *model, uint32_t textureHandle) {
	input_ = Input::GetInstance();
	assert(model);
	model_.reset(model);

	modelTh_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };

	setCollisionAttribute(kCollisionAttributePlayer);
	setCollisionMask(~kCollisionAttributePlayer);

	worldTransform3DReticle_.Initialize();
	reticleTh_ = TextureManager::Load("reticle.png");
	reticle_.reset(Sprite::Create(reticleTh_, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }));
	reticle_->SetSize({ 40.0f,40.0f });
}

void Player::Update() {
	//===============================================
	// 古い弾の削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet> &bullet) { return bullet->IsDead() ? true : false; });
	//===============================================

	//===============================================
	// キーボード入力 による 移動

	// 旋回
	Rotate();

	// 移動
	Vector3 move = { 0.0f, 0.0f, 0.0f };
	// x 軸
	if(input_->PushKey(DIK_LEFT)) {
		move.x -= kSpeed_;
	} else if(input_->PushKey(DIK_RIGHT)) {
		move.x += kSpeed_;
	}
	// y 軸
	if(input_->PushKey(DIK_UP)) {
		move.y += kSpeed_;
	} else if(input_->PushKey(DIK_DOWN)) {
		move.y -= kSpeed_;
	}

	worldTransform_.translation_ += move;

	//===============================================

	//===============================================
	// キャラクター攻撃処理
	Attack();
	//===============================================

	//===============================================
	// 移動の制限
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -6.0f);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, 6.0f);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -4.0f);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, 4.0f);

	//===============================================

	//===============================================
	// 3D Reticle 更新
	constexpr float kDistancePlayerTo3DReticle = 50.0f;
	Vector3 offset = { 0.0f,0.0f,1.0f };
	offset = Transform(offset, MakeMatrix::Affine(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_));
	offset = offset.Normalize() * kDistancePlayerTo3DReticle;
	worldTransform3DReticle_.translation_ = offset + worldTransform_.translation_;

	//===============================================

	//===============================================
	// worldTransform を更新
	worldTransform_.UpdateMatrix();
	worldTransform3DReticle_.UpdateMatrix();
	//===============================================

	//===============================================
	// imgui による座標の表示
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::Text("Position :\nX %.2f \n Y %.2f \n Z %.2f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();
#endif // _DEBUG
	//===============================================
}

void Player::Draw(const ViewProjection &viewProj) {
	model_->Draw(worldTransform_, viewProj, modelTh_);
	// 弾があれば描画
	for(auto &bullet : bullets_) {
		bullet->Draw(viewProj);
	}
}

void Player::DrawUI(const ViewProjection &viewProj) {
	//===============================================
	// Reticle を スクリーン座標で
	Vector3 reticlePos = Transform({ 0.0f,0.0f,0.0f }, worldTransform3DReticle_.matWorld_);
	reticlePos = Transform(reticlePos,
						   viewProj.matView
						   * viewProj.matProjection
						   * MakeMatrix::ViewPort(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0.0f, 1.0f)
	);
	reticle_->SetPosition({ reticlePos.x,reticlePos.y });
	//===============================================
	reticle_->Draw();
}

void Player::Rotate() {
	if(input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed_;
	} else if(input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed_;
	}
}

void Player::Attack() {
	if(input_->TriggerKey(DIK_W)) {
		bullets_.push_back(std::make_unique<PlayerBullet>());
		// 速度 と player の 向き を合わせる(回転させる)
		Vector3 velocity = Transform({ 0.0f,0.0f,0.0f }, worldTransform3DReticle_.matWorld_) - getWorldPos();;
		velocity = velocity.Normalize() * kBuletSpeed_;
		bullets_.back()->Init(Model::Create(), Transform({ 0.0f,0.0f,0.0f }, worldTransform_.matWorld_), velocity);
	}

	for(auto &bullet : bullets_) {
		bullet->Update();
	}
}

Vector3 Player::getWorldPos() const {
	Vector3 pos = Transform({ 0.0f,0.0f,0.0f }, worldTransform_.matWorld_);
	return pos;
}
