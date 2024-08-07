#include "HammerRotating.h"

#include "Player.h"

#include "GlobalVariables.h"

#include "Easing.h"

void HammerRotating::Init(){
	currentPhase_ = InAttackBehaviorPhase::Anticipation;

	hostDir_ = host_->getDir().Normalize();

	GlobalVariables *variables = GlobalVariables::getInstance();
	variables->addValue("PlayerAttack_HammerRotating","moveSpeed_",moveSpeed_);

	rightArmRotate_ = host_->getPartsTransform("RightArm").rotation_;
	weaponRotate_ = host_->getPartsTransform("Weapon").rotation_;
	bodyRotate_ = host_->getPartsTransform("Body").rotation_;

	variables->addValue<int32_t>("PlayerAttack_HammerRotating","anticipationTime_",anticipationTime_);
	variables->addValue<int32_t>("PlayerAttack_HammerRotating","chargeTime_",chargeTime_);
	variables->addValue<int32_t>("PlayerAttack_HammerRotating","attackTime_",attackTime_);
	variables->addValue<int32_t>("PlayerAttack_HammerRotating","recoveryTime_",recoveryTime_);

	variables->addValue("PlayerAttack_HammerRotating","chargeRightArmRotate_",chargeRightArmRotate_);
	variables->addValue("PlayerAttack_HammerRotating","chargeWeaponRotate_",chargeWeaponRotate_);

	variables->addValue("PlayerAttack_HammerRotating","startRightArmRotate_",startRightArmRotate_);
	variables->addValue("PlayerAttack_HammerRotating","startWeaponRotate_",startWeaponRotate_);

	variables->addValue("PlayerAttack_HammerRotating","moveSpeed_",moveSpeed_);
	variables->addValue("PlayerAttack_HammerRotating","swingRotateSpeed_",swingRotateSpeed_);
}

void HammerRotating::Update(){
	++phaseT_;
	switch(currentPhase_){
	case InAttackBehaviorPhase::Anticipation:{
		if(phaseT_ >= anticipationTime_){
			phaseT_ = 0;
			currentPhase_ = InAttackBehaviorPhase::Charge;
			break;
		}
		break;
	}
	case InAttackBehaviorPhase::Charge:{
		rightArmRotate_ = EasingLerp(Easing::easeOutQuart,static_cast<float>(phaseT_) / static_cast<float>(chargeTime_),startRightArmRotate_,chargeRightArmRotate_);
		weaponRotate_ = EasingLerp(Easing::easeOutQuart,static_cast<float>(phaseT_) / static_cast<float>(chargeTime_),startWeaponRotate_,chargeWeaponRotate_);
		host_->setPartsRotate("Weapon",weaponRotate_);
		host_->setPartsRotate("RightArm",rightArmRotate_);
		if(phaseT_ >= chargeTime_){
			phaseT_ = 0;
			rightArmRotate_ = chargeRightArmRotate_;
			weaponRotate_ = chargeWeaponRotate_;
			currentPhase_ = InAttackBehaviorPhase::Attack;
			break;
		}
		break;
	}
	case InAttackBehaviorPhase::Attack:{
		XINPUT_STATE gamePadState;
		bodyRotate_.y += swingRotateSpeed_;
		weaponRotate_.y += swingRotateSpeed_;

		host_->setPartsRotate("Body",bodyRotate_);
		host_->setPartsRotate("Weapon",weaponRotate_);

		host_->addTranslate(hostDir_ * moveSpeed_);

		if(phaseT_ >= attackTime_){
			phaseT_ = 0;
			input_->GetJoystickState(0,gamePadState);
			if(gamePadState.Gamepad.wButtons & XINPUT_GAMEPAD_Y){
				if(phaseT_ <= maxAttackTime_){
					break;
				}
			}
			currentPhase_ = InAttackBehaviorPhase::Recovery;

			swingedRightArmRotate_ = host_->getPartsTransform("RightArm").rotation_;
			swingedWeaponRotate_ = host_->getPartsTransform("Weapon").rotation_;
			swingedBodyRotate_ = host_->getPartsTransform("Body").rotation_;

			break;
		}
		break;
	}
	case InAttackBehaviorPhase::Recovery:{
		float t = static_cast<float>(phaseT_) / static_cast<float>(recoveryTime_);
		rightArmRotate_ = EasingLerp(Easing::easeOutQuart,t,swingedRightArmRotate_,{0.0f,0.0f,0.0f});
		weaponRotate_ = EasingLerp(Easing::easeOutQuart,t,swingedWeaponRotate_,{0.0f,0.0f,0.0f});
		bodyRotate_ = EasingLerp(Easing::easeOutQuart,t,swingedBodyRotate_,{0.0f,0.0f,0.0f});

		host_->setPartsRotate("Body",bodyRotate_);
		host_->setPartsRotate("Weapon",weaponRotate_);
		host_->addPartsRotate("RightArm",rightArmRotate_);

		if(phaseT_ >= recoveryTime_){
			host_->setPartsRotate("Body",{0.0f,0.0f,0.0f});
			host_->setPartsRotate("Weapon",{0.0f,0.0f,0.0f});
			host_->setPartsRotate("RightArm",{0.0f,0.0f,0.0f});

			//	player's AttackBehavior Transition
			host_->TransitionAttackBehavior(nullptr);
			return;
		}
		break;
	}
	default:
		break;
	}

	if(comboNext_){
		return;
	}
}
