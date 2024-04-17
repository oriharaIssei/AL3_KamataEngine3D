#include "SkyDome.h"

void Skydome::Init() {
	model_.reset(Model::CreateFromOBJ("Skydome", true));
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {  }

void Skydome::Draw(const ViewProjection& viewProj) { model_->Draw(worldTransform_, viewProj); }
