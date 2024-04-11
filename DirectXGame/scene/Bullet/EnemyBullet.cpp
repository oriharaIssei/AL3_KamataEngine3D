#include "EnemyBullet.h"

#include "TextureManager.h"
#include <cassert>

void EnemyBullet::Init(Model* model, const Vector3& pos, const Vector3& velocity) {
	velocity_ = velocity;

	assert(model);
	model_.reset(model);

	th_ = TextureManager::Load("shuriken.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	/*	解 1*/
	// xz 平面内での velocity_ の角度を計算
	worldTransform_.rotation_.y = std::atan2(velocity_.x,velocity_.z);

	// y 軸周りに回転させる
	Vector3 velocityZ = Transform(velocity_, MakeMatrix::RotateY(-worldTransform_.rotation_.y));

	// 回転後の velocity_ を使って zy 平面内での角度を計算
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	
}
void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, th_); }
