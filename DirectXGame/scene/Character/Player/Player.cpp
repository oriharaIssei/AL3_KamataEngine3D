#include "Player.h"

#include "imgui.h"

#include "TextureManager.h"

#include "MyMath.h"
#include "numbers"

void Player::Init() {
	partsModels_["Body"].reset(new PartsModel());
	partsModels_["Body"]->Init(Model::CreateFromOBJ("Player_Body"));
	partsModels_["Head"].reset(new PartsModel());
	partsModels_["Head"]->Init(Model::CreateFromOBJ("Player_Head"));
	partsModels_["RightArm"].reset(new PartsModel());
	partsModels_["RightArm"]->Init(Model::CreateFromOBJ("Player_RightArm"));
	partsModels_["LeftArm"].reset(new PartsModel());
	partsModels_["LeftArm"]->Init(Model::CreateFromOBJ("Player_leftArm"));

	worldTransform_.Initialize();

	partsModels_["Body"]->worldTransform.parent_ = &worldTransform_;
	partsModels_["Head"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;
	partsModels_["RightArm"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;
	partsModels_["LeftArm"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;

	partsModels_["Body"]->worldTransform.translation_.y = 2.2f;
	partsModels_["RightArm"]->worldTransform.rotation_ = {-1.0f,0.4f,1.1f};
	partsModels_["RightArm"]->worldTransform.translation_ = {2.0f,4.5f,-0.7f};
	partsModels_["LeftArm"]->worldTransform.rotation_ = {0.0f,-1.0f,1.2f};
	partsModels_["LeftArm"]->worldTransform.translation_ = {-2.0f,4.9f,0.0f};

	input_ = Input::GetInstance();

	InitFloatingGimmick();
}

void Player::Update() {
	move_ = {static_cast<float>(input_->PushKey(DIK_D) - input_->PushKey(DIK_A)),0.0f,static_cast<float>(input_->PushKey(DIK_W) - input_->PushKey(DIK_S))};

	move_ = move_.Normalize() * speed_;
	if(viewProjection_) {
		move_ = TransformVector(move_,MakeMatrix::RotateXYZ(viewProjection_->rotation_));
	}
	if(move_.length() > 0.1f) {
		lastDir_ = move_.Normalize();
	}

	ImGui::Begin("PlayerParts");
	for(auto &part : partsModels_) {
		if(ImGui::TreeNode(part.first.c_str())) {
			ImGui::TreePop();
			ImGui::DragFloat3("Scale",&part.second->worldTransform.scale_.x,0.1f);
			ImGui::DragFloat3("Rotate",&part.second->worldTransform.rotation_.x,0.1f);
			ImGui::DragFloat3("Translation",&part.second->worldTransform.translation_.x,0.1f);
		}
	}
	
	ImGui::End();

	UpdateFloatingGimmick();

	worldTransform_.rotation_.y = lerpShortAngle(worldTransform_.rotation_.y,atan2(lastDir_.x,lastDir_.z),0.1f);
	worldTransform_.translation_ += move_;

	worldTransform_.UpdateMatrix();

	for(auto &part : partsModels_) {
		part.second->worldTransform.UpdateMatrix();
	}
}

void Player::Draw(const ViewProjection &viewProj) {
	BaseCharacter::Draw(viewProj);
}

void Player::InitFloatingGimmick() {
	floatingParameter_ = 0.0f;
	floatingAmplitude_ = 0.6f;
}

void Player::UpdateFloatingGimmick() {
	const uint16_t roopFrame = 58;
	const float updatePerParameter = 2 * std::numbers::pi_v<float> / (float)roopFrame;

	floatingParameter_ += updatePerParameter;
	floatingParameter_ = std::fmod(floatingParameter_,std::numbers::pi_v<float>);

	const float sin = std::sin(floatingParameter_);
	partsModels_["Body"]->worldTransform.translation_.y = sin * floatingAmplitude_;

	partsModels_["Body"]->worldTransform.rotation_.y = -sin * 2.0f * (floatingAmplitude_);
}