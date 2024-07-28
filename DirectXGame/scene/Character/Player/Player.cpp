#include "Player.h"

#include "IAttackBehavior.h"

#include "HammerDown.h"
#include "HammerRotating.h"
#include "HammerSwing.h"

#include "imgui.h"

#include "TextureManager.h"

#include "GlobalVariables.h"
#include "LockOn.h"

#include "Easing.h"
#include "MyMath.h"
#include <numbers>

#include "Collision/Manager/CollisionTypeIdDef.h"

constexpr float delTime = (1.0f / 60.0f);

void Player::Init(){
	BaseCharacter::Init();

	collider_->Init({0.0f,4.0f,0.0f},6.0f,[this]([[maybe_unused]] Collider *collider){behaviorRequest_ = Behavior::kJump; });
	collider_->setTransformParent(&worldTransform_);
	collider_->setTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayer));

	collider_->Init({0.0f,4.0f,0.0f},6.0f,[this]([[maybe_unused]] Collider *collider){
		if(collider->getTypeID() != static_cast<uint32_t>(CollisionTypeDef::kEnemy)){
			return;
		}
		behaviorRequest_ = Behavior::kJump; });
	collider_->setTransformParent(&worldTransform_);
	collider_->setTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayer));

#pragma region"Parts Initialize"
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

	partsModels_["Body"]->worldTransform.parent_ = &worldTransform_;
	partsModels_["Weapon"]->worldTransform.parent_ = &worldTransform_;
	partsModels_["Head"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;

	partsModels_["RightArm"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;
	partsModels_["LeftArm"]->worldTransform.parent_ = &partsModels_["Body"]->worldTransform;

	partsModels_["Body"]->worldTransform.translation_.y = 2.2f;
	partsModels_["RightArm"]->worldTransform.translation_ = {2.0f,4.9f,0.0f};
	partsModels_["LeftArm"]->worldTransform.translation_ = {-2.0f,4.9f,0.0f};

	partsModels_["Weapon"]->worldTransform.translation_.y = 7.3f;
#pragma endregion

	weaponCollider_ = std::make_unique<Collider>();

	weaponCollider_->Init({0.0f,7.0f,0.0f},8.0f,[this](Collider *collider){
		if(collider->getTypeID() != static_cast<uint32_t>(CollisionTypeDef::kEnemy)){
			return;
		}
		std::unique_ptr<HitEffect> effect = std::make_unique<HitEffect>();
		effect->Init();
		effect->transform_.translation_ = collider->getPosition();
		effect->transform_.UpdateMatrix();
		hitEffects_.emplace_back(std::move(effect));
	});
	weaponCollider_->setTransformParent(&partsModels_["Weapon"]->worldTransform);
	weaponCollider_->setTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon));


	input_ = Input::GetInstance();

	InitFloatingGimmick();

	currentBehavior_ = Behavior::kRoot;
	BehaviorRootInit();

	GlobalVariables *glovalVariables = GlobalVariables::getInstance();
	glovalVariables->CreateGroup("Player");
	glovalVariables->addValue("Player","speed",speed_);
	glovalVariables->addValue("Player","dash speed",workDash_.speed_);
	glovalVariables->addValue("Player","JumpPowar",jumpPowar_);
}

void Player::Update(){
	if(behaviorRequest_){
		currentBehavior_ = behaviorRequest_.value();
		behaviorRequest_ = std::nullopt;

		switch(currentBehavior_){
		case Behavior::kRoot:
			BehaviorRootInit();
			break;
		case Behavior::kAttack:
			BehaviorAttackInit();
			break;
		case Behavior::kDash:
			BehaviorDashInit();
			break;
		case Behavior::kJump:
			BehaviorJumpInit();
			break;
		default:
			break;
		}
	}

	ImGui::Begin("PlayerParts");
	for(auto &part : partsModels_){
		std::string labelString = part.first;
		if(ImGui::TreeNode(labelString.c_str())){
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

	switch(currentBehavior_){
	case Behavior::kRoot:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	case Behavior::kJump:
		BehaviorJumpUpdate();
		break;
	default:
		break;
	}

	worldTransform_.UpdateMatrix();

	collider_->Update();

	for(auto &part : partsModels_){
		part.second->worldTransform.UpdateMatrix();
	}
	weaponCollider_->Update();

	for(auto effectItr = hitEffects_.begin(); effectItr != hitEffects_.end();){
		HitEffect *effect = effectItr->get();
		effect->Update();
		if(effect->getIsAlive()){
			++effectItr; continue;
		}
		effectItr = hitEffects_.erase(effectItr);
	}
}

void Player::Draw(const ViewProjection &viewProj){
	BaseCharacter::Draw(viewProj);

	for(auto &effect : hitEffects_){
		effect->particleModel_->Draw(effect->transform_,viewProj,effect->color_.get());
	}
}

void Player::BehaviorRootInit(){
	partsModels_["Body"]->worldTransform.translation_.y = 2.2f;
	partsModels_["Weapon"]->worldTransform.rotation_ = {0.0f,0.0f,0.0f};
	partsModels_["RightArm"]->worldTransform.translation_ = {2.0f,4.9f,0.0f};
	partsModels_["LeftArm"]->worldTransform.translation_ = {-2.0f,4.9f,0.0f};
}

void Player::BehaviorRootUpdate(){
	if(input_->TriggerKey(DIK_E)){
		behaviorRequest_ = Behavior::kAttack;
	} else if(input_->TriggerKey(DIK_LSHIFT)){
		behaviorRequest_ = Behavior::kDash;
	} else if(input_->TriggerKey(DIK_SPACE)){
		behaviorRequest_ = Behavior::kJump;
	}

	move_ = {static_cast<float>(input_->PushKey(DIK_D) - input_->PushKey(DIK_A)),0.0f,static_cast<float>(input_->PushKey(DIK_W) - input_->PushKey(DIK_S))};

	if(viewProjection_){
		move_ = TransformVector(move_,MakeMatrix::RotateXYZ(viewProjection_->rotation_));
	}

	velocity_ = move_.Normalize() * speed_;

	worldTransform_.translation_ += velocity_;
	if(lockOn_ && lockOn_->ExistTarget()){
		lastDir_ = lockOn_->getTargetPos() - Vector3(worldTransform_.matWorld_[3]);
	} else{
		if(move_.length() > 0.1f){
			lastDir_ = move_.Normalize();
		}
	}
	worldTransform_.rotation_.y = lerpShortAngle(worldTransform_.rotation_.y,atan2(lastDir_.x,lastDir_.z),0.1f);

	UpdateFloatingGimmick();
}

void Player::BehaviorAttackInit(){
	currentAttackBehavior_ = std::make_unique<HammerDown>(this);
	currentAttackBehavior_->Init();
}

void Player::BehaviorAttackUpdate(){
	currentAttackBehavior_->Update();
}

void Player::BehaviorDashInit(){
	// ※ lastDir_.yは何も入っていない ===========================
	workDash_.t_ = 0;
}

void Player::BehaviorDashUpdate(){
	worldTransform_.translation_ += lastDir_.Normalize() * workDash_.speed_;
	++workDash_.t_;

	if(workDash_.t_ >= workDash_.maxT_){
		behaviorRequest_ = Behavior::kRoot;
		return;
	}
}

void Player::BehaviorJumpInit(){
	partsModels_["Body"]->worldTransform.translation_.y = 0;

	partsModels_["RightArm"]->worldTransform.rotation_.x = 0;
	partsModels_["LeftArm"]->worldTransform.rotation_.x = 0;

	velocity_.y = jumpPowar_;
}

void Player::BehaviorJumpUpdate(){
	const float gravityAccel = -0.05f;
	velocity_.y += gravityAccel;
	worldTransform_.translation_ += velocity_;
	if(worldTransform_.translation_.y <= 0){
		worldTransform_.translation_.y = 0;

		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::InitFloatingGimmick(){
	floatingParameter_ = 0.0f;
	floatingAmplitude_ = 0.6f;
}

void Player::UpdateFloatingGimmick(){
	const uint16_t roopFrame = 58;
	const float updatePerParameter = 2 * std::numbers::pi_v<float> / (float)roopFrame;

	floatingParameter_ += updatePerParameter;
	floatingParameter_ = std::fmod(floatingParameter_,std::numbers::pi_v<float>);

	const float sin = std::sin(floatingParameter_);
	partsModels_["Body"]->worldTransform.translation_.y = sin * floatingAmplitude_;

	partsModels_["Body"]->worldTransform.rotation_.y = -sin * 2.0f * (floatingAmplitude_);
}

void Player::TransitionAttackBehavior(IAttackBehavior *nextBehavior){
	if(nextBehavior){
		currentAttackBehavior_.reset(nextBehavior);
		currentAttackBehavior_->Init();
		return;
	}
	currentAttackBehavior_.release();
	currentAttackBehavior_ = nullptr;
	behaviorRequest_ = Behavior::kRoot;
}

void Player::HitEffect::Init(){
	particleModel_.reset(Model::CreateSphere());
	transform_.Initialize();

	scale_ = maxScale_;
	timer_ = fullTime_;

	color_ = std::make_unique<ObjectColor>();
	color_->Initialize();
}

void Player::HitEffect::Update(){
	timer_ -= delTime;
	float t = timer_ / fullTime_;

	color_->SetColor({1.0f,1.0f,1.0f,Lerp(1 - t,1.0f,0.0f)});
	color_->TransferMatrix();

	scale_ = Lerp(t,0.0f,maxScale_);

	transform_.scale_ = {scale_,scale_,scale_};
	transform_.UpdateMatrix();
}