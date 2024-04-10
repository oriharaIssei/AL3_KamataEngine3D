#include "EnemyBullet.h"

#include "TextureManager.h"
#include <cassert>

void EnemyBullet::Init(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_.reset(model);

	th_ = TextureManager::Load("shuriken.png");

	velocity_ = velocity;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
}
void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, th_); }
