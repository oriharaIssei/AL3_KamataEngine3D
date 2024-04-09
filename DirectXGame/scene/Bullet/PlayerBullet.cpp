#include "PlayerBullet.h"

#include "TextureManager.h"
#include <cassert>

void PlayerBullet::Init(Model* model, const Vector3& pos) {
	assert(model);
	model_.reset(model);

	th_ = TextureManager::Load("uvChecker.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
}

void PlayerBullet::Update() { worldTransform_.UpdateMatrix(); }

void PlayerBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, th_); }
