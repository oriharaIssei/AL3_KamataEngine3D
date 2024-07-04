#include "Enemy.h"

#include "imgui.h"

#include "numbers"
#include "stdint.h"

void Enemy::Init() {
	partsModels_["Body"].reset(new PartsModel());
	partsModels_["Left_Arm"].reset(new PartsModel());
	partsModels_["Right_Arm"].reset(new PartsModel());

	partsModels_["Body"]->Init(Model::CreateFromOBJ("Enemy_Body"));
	partsModels_["Left_Arm"]->Init(Model::CreateFromOBJ("Enemy_Arm"));
	partsModels_["Right_Arm"]->Init(Model::CreateFromOBJ("Enemy_Arm"));

	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

	for(auto &part : partsModels_) {
		part.second->worldTransform.parent_ = &worldTransform_;
	}

	partsModels_["Left_Arm"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;
	partsModels_["Right_Arm"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;

	partsModels_["Left_Arm"]->worldTransform.scale_ = {2.0f,2.0f,2.0f};
	partsModels_["Left_Arm"]->worldTransform.translation_ = {-1.4f,0.9f,0.0f};
	partsModels_["Left_Arm"]->worldTransform.rotation_ = {0.0f,0.0f,0.5f};
	partsModels_["Right_Arm"]->worldTransform.scale_ = {2.0f,2.0f,2.0f};
	partsModels_["Right_Arm"]->worldTransform.translation_ = {1.4f,0.9f,0.0f};
	partsModels_["Right_Arm"]->worldTransform.rotation_ = {0.0f,0.0f,-0.5f};
}

void Enemy::Update() {
	ImGui::Begin("EnemyParts");
	for(auto &part : partsModels_) {
		if(ImGui::TreeNode(part.first.c_str())) {
			ImGui::TreePop();
			ImGui::DragFloat3("Scale",&part.second->worldTransform.scale_.x,0.1f);
			ImGui::DragFloat3("Rotate",&part.second->worldTransform.rotation_.x,0.1f);
			ImGui::DragFloat3("Translation",&part.second->worldTransform.translation_.x,0.1f);
		}
	}

	ImGui::End();

	WalkMotion();

	for(auto &part : partsModels_) {
		part.second->worldTransform.UpdateMatrix();
	}
}

void Enemy::Draw(const ViewProjection &viewProj) {
	BaseCharacter::Draw(viewProj);
}

void Enemy::WalkMotion() {
	const uint16_t roopFrame = 40;
	const float updatePerParameter = 2 * std::numbers::pi_v<float> / (float)roopFrame;
	const float amplitude = 2.0f;

	walkParameter_ += updatePerParameter;
	walkParameter_ = std::fmod(walkParameter_,std::numbers::pi_v<float>);

	const float cos = std::cos(updatePerParameter);

	partsModels_["Left_Arm"]->worldTransform.rotation_.x = cos * amplitude;
	partsModels_["Right_Arm"]->worldTransform.rotation_.x = cos * amplitude;
}