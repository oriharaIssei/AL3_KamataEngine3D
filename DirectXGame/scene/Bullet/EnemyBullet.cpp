#include "EnemyBullet.h"

#include "Player.h"

#include "Lerp.h"
#include "TextureManager.h"
#include <cassert>

void EnemyBullet::Init(Model* model, const Vector3& pos, Player* player) {
	player_p = player;

	velocity_ = (player_p->getWorldPos() - pos).Normalize() * 0.01f;

	assert(model);
	model_.reset(model);

	th_ = TextureManager::Load("shuriken.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	setCollisionAttribute(kCollisionAttributeEnemy);
	setCollisionMask(~kCollisionAttributeEnemy);
}
void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;

	velocity_ = Slerp(0.02f, velocity_.Normalize(), Vector3(player_p->getWorldPos() - worldTransform_.translation_).Normalize()) * 0.1f;

	// xz 平面内での velocity_ の角度を計算
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	// y 軸周りに回転させる
	Vector3 velocityZ = Transform(velocity_, MakeMatrix::RotateY(-worldTransform_.rotation_.y));

	// 回転後の velocity_ を使って zy 平面内での角度を計算
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, th_); }

void EnemyBullet::OnCollision() { isDead_ = true; }
