#include "Ground.h"

void Ground::Init() {
	model_.reset(Model::CreateFromOBJ("Ground"));

	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(const ViewProjection &viewProj) {
	model_->Draw(worldTransform_,viewProj);
}
