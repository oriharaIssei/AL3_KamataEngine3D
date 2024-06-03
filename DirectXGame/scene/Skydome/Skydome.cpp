#include "Skydome.h"

void Skydome::Init() {
	model_.reset(Model::CreateFromOBJ("Skydome"));
	worldTransform_.Initialize();
	worldTransform_.scale_ = {2.0f,2.0f,2.0f};
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(const ViewProjection &viewProj) {
	model_->Draw(worldTransform_,viewProj);
}
