#include "Player.h"

#include "imgui.h"

#include "TextureManager.h"

#include "MyMath.h"
#include "numbers"

void Player::Init() {
	body_.model.reset(Model::CreateFromOBJ("Player_Body"));
	body_.worldTransform.Initialize();
	head_.model.reset(Model::CreateFromOBJ("Player_Head"));
	head_.worldTransform.Initialize();
	rightArm_.model.reset(Model::CreateFromOBJ("Player_RightArm"));
	rightArm_.worldTransform.Initialize();
	leftArm_.model.reset(Model::CreateFromOBJ("Player_leftArm"));
	leftArm_.worldTransform.Initialize();

	worldTransform_.Initialize();

	head_.worldTransform.parent_ = &body_.worldTransform;
	body_.worldTransform.parent_ = &worldTransform_;
	rightArm_.worldTransform.parent_ = &body_.worldTransform;
	leftArm_.worldTransform.parent_ = &body_.worldTransform;

	body_.worldTransform.translation_.y = 2.2f;
	rightArm_.worldTransform.rotation_ = {-1.0f,0.4f,1.1f};
	rightArm_.worldTransform.translation_ = {2.0f,4.5f,-0.7f};
	leftArm_.worldTransform.rotation_ = {0.0f,-1.0f,1.2f};
	leftArm_.worldTransform.translation_ = {-2.0f,4.9f,0.0f};

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
	if(ImGui::TreeNode("Head")) {
		ImGui::TreePop();
		ImGui::DragFloat3("Scale",&head_.worldTransform.scale_.x,0.1f);
		ImGui::DragFloat3("Rotate",&head_.worldTransform.rotation_.x,0.1f);
		ImGui::DragFloat3("Translation",&head_.worldTransform.translation_.x,0.1f);
	}
	if(ImGui::TreeNode("Body")) {
		ImGui::TreePop();
		ImGui::DragFloat3("Scale",&body_.worldTransform.scale_.x,0.1f);
		ImGui::DragFloat3("Rotate",&body_.worldTransform.rotation_.x,0.1f);
		ImGui::DragFloat3("Translation",&body_.worldTransform.translation_.x,0.1f);
	}
	if(ImGui::TreeNode("LeftArm")) {
		ImGui::TreePop();
		ImGui::DragFloat3("Scale",&leftArm_.worldTransform.scale_.x,0.1f);
		ImGui::DragFloat3("Rotate",&leftArm_.worldTransform.rotation_.x,0.1f);
		ImGui::DragFloat3("Translation",&leftArm_.worldTransform.translation_.x,0.1f);
	}
	if(ImGui::TreeNode("rightArm")) {
		ImGui::TreePop();
		ImGui::DragFloat3("Scale",&rightArm_.worldTransform.scale_.x,0.1f);
		ImGui::DragFloat3("Rotate",&rightArm_.worldTransform.rotation_.x,0.1f);
		ImGui::DragFloat3("Translation",&rightArm_.worldTransform.translation_.x,0.1f);
	}
	ImGui::End();

	UpdateFloatingGimmick();

	worldTransform_.rotation_.y = lerpShortAngle(worldTransform_.rotation_.y,atan2(lastDir_.x,lastDir_.z),0.1f);
	worldTransform_.translation_ += move_;

	worldTransform_.UpdateMatrix();
	body_.worldTransform.UpdateMatrix();
	head_.worldTransform.UpdateMatrix();
	leftArm_.worldTransform.UpdateMatrix();
	rightArm_.worldTransform.UpdateMatrix();
}

void Player::Draw(const ViewProjection &viewProj) {
	body_.Draw(viewProj);
	head_.Draw(viewProj);
	rightArm_.Draw(viewProj);
	leftArm_.Draw(viewProj);
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
	body_.worldTransform.translation_.y = sin * floatingAmplitude_;

	body_.worldTransform.rotation_.y = -sin * 2.0f * (floatingAmplitude_);
}