#include "PlayerBullet.h"

#include "TextureManager.h"
#include <cassert>

void PlayerBullet::Init(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_.reset(model);

	th_ = TextureManager::Load("uvChecker.png");

	velocity_ = velocity;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
}

void PlayerBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	// 移動
	worldTransform_.translation_ += velocity_;
	// worldTransform更新
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, th_); }
