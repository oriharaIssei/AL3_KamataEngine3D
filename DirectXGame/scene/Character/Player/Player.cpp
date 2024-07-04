#include "Player.h"

#include "imgui.h"

#include "TextureManager.h"

#include "Easing.h"
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
	partsModels_["Weapon"].reset(new PartsModel());
	partsModels_["Weapon"]->Init(Model::CreateFromOBJ("Player_Weapon"));

	worldTransform_.Initialize();

	partsModels_["Body"]->worldTransform.parent_ = &worldTransform_;
	partsModels_["Weapon"]->worldTransform.parent_ = &worldTransform_;
	partsModels_["Head"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;

	partsModels_["RightArm"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;
	partsModels_["LeftArm"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;

	partsModels_["Body"]->worldTransform.translation_.y = 2.2f;
	partsModels_["RightArm"]->worldTransform.translation_ = {2.0f,4.9f,0.0f};
	partsModels_["LeftArm"]->worldTransform.translation_ = {-2.0f,4.9f,0.0f};

	input_ = Input::GetInstance();

	InitFloatingGimmick();

	maxT_ = 36;

	currentBehavior_ = Behavior::kRoot;
	BehaviorRootInit();
}

void Player::Update() {
	if(behaviorRequest_) {
		currentBehavior_ = behaviorRequest_.value();
		behaviorRequest_ = std::nullopt;

		switch(currentBehavior_) {
		case Player::Behavior::kRoot:
		BehaviorRootInit();
		break;
		case Player::Behavior::kAttack:
		BehaviorAttackInit();
		break;
		default:
		break;
		}
	}

	ImGui::Begin("PlayerParts");
	for(auto &part : partsModels_) {
		std::string labelString = part.first;
		if(ImGui::TreeNode(labelString.c_str())) {
			ImGui::TreePop();
			labelString += " Scale";
			ImGui::DragFloat3(labelString.c_str(),&part.second->worldTransform.scale_.x,0.1f);
			labelString = part.first + " Rotate";
			ImGui::DragFloat3(labelString.c_str(),&part.second->worldTransform.rotation_.x,0.1f);
			labelString = part.first + " Translation";
			ImGui::DragFloat3(labelString.c_str(),&part.second->worldTransform.translation_.x,0.1f);
		}
	}
	ImGui::End();

	switch(currentBehavior_) {
	case Behavior::kRoot:
	BehaviorRootUpdate();
	if(input_->TriggerKey(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kAttack;
	}
	break;
	case Behavior::kAttack:
	BehaviorAttackUpdate();
	break;
	default:
	break;
	}

	worldTransform_.UpdateMatrix();

	for(auto &part : partsModels_) {
		part.second->worldTransform.UpdateMatrix();
	}
}

void Player::Draw(const ViewProjection &viewProj) {
	BaseCharacter::Draw(viewProj);
}

void Player::BehaviorRootUpdate() {
	move_ = {static_cast<float>(input_->PushKey(DIK_D) - input_->PushKey(DIK_A)),0.0f,static_cast<float>(input_->PushKey(DIK_W) - input_->PushKey(DIK_S))};

	move_ = move_.Normalize() * speed_;
	if(viewProjection_) {
		move_ = TransformVector(move_,MakeMatrix::RotateXYZ(viewProjection_->rotation_));
	}
	if(move_.length() > 0.1f) {
		lastDir_ = move_.Normalize();
	}

	UpdateFloatingGimmick();

	worldTransform_.translation_ += move_;

	worldTransform_.rotation_.y = lerpShortAngle(worldTransform_.rotation_.y,atan2(lastDir_.x,lastDir_.z),0.1f);
}

void Player::BehaviorRootInit() {
	partsModels_["Body"]->worldTransform.translation_.y = 2.2f;
	partsModels_["Weapon"]->worldTransform.translation_.y = 7.3f;
	partsModels_["RightArm"]->worldTransform.translation_ = {2.0f,4.9f,0.0f};
	partsModels_["LeftArm"]->worldTransform.translation_ = {-2.0f,4.9f,0.0f};
}

void Player::BehaviorAttackUpdate() {
	++t_;
	if(t_ == 12) {
		worldTransform_.translation_ += TransformNormal({0,0,3.0f},MakeMatrix::RotateXYZ(worldTransform_.rotation_));

	} else if(t_ == maxT_) {
		partsModels_["Weapon"]->worldTransform.rotation_.x = 0;
		behaviorRequest_ = Behavior::kRoot;
	}

	float easedArmRotate = EasingLerp<float>(Easing::easeOutQuart,(t_ / maxT_),-2,0);

	partsModels_["LeftArm"]->worldTransform.rotation_.x = easedArmRotate;
	partsModels_["RightArm"]->worldTransform.rotation_.x = easedArmRotate;

	partsModels_["Weapon"]->worldTransform.rotation_.x = EasingLerp<float>(Easing::easeOutQuart,t_ / maxT_,0,2);
}

void Player::BehaviorAttackInit() {
	t_ = 0;
	partsModels_["LeftArm"]->worldTransform.rotation_.x = -2.0f;
	partsModels_["RightArm"]->worldTransform.rotation_.x = -2.0f;

	partsModels_["Body"]->worldTransform.rotation_ = {0,0,0};
	partsModels_["Weapon"]->worldTransform.rotation_.x = 0;
	partsModels_["Weapon"]->worldTransform.translation_.y = 7.3f;
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