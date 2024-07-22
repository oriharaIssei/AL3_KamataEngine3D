#include "HammerSwing.h"

#include "HammerRotating.h"

#include "GlobalVariables.h"

#include "Player.h"

#include "Easing.h"

void HammerSwing::Init(){
	currentPhase_ = InAttackBehaviorPhase::Anticipation;

	GlobalVariables *variables = GlobalVariables::getInstance();

	variables->addValue<int32_t>("PlayerAttack_HammerSwing","anticipationTime_",anticipationTime_);
	variables->addValue<int32_t>("PlayerAttack_HammerSwing","chargeTime_",chargeTime_);
	variables->addValue<int32_t>("PlayerAttack_HammerSwing","attackTime_",attackTime_);
	variables->addValue<int32_t>("PlayerAttack_HammerSwing","recoveryTime_",recoveryTime_);

	variables->addValue<Vector3>("PlayerAttack_HammerSwing","swingLeftArmRotate_",swingLeftArmRotate_);
	variables->addValue<Vector3>("PlayerAttack_HammerSwing","swingRightArmRotate_",swingRightArmRotate_);
	variables->addValue<Vector3>("PlayerAttack_HammerSwing","swingWeaponRotate_",swingWeaponRotate_);

	startLeftArmRotate_ = host_->getPartsTransform("LeftArm").rotation_;
	startRightArmRotate_ = host_->getPartsTransform("RightArm").rotation_;
	startWeaponRotate_ = host_->getPartsTransform("Weapon").rotation_;

	leftArmRotate_ = startLeftArmRotate_;
	rightArmRotate_ = startRightArmRotate_;
	weaponRotate_ = startWeaponRotate_;

	comboNext_ = false;

	host_->setPartsRotate("LeftArm",leftArmRotate_);
	host_->setPartsRotate("RightArm",rightArmRotate_);
}

void HammerSwing::Update(){
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
		if(phaseT_ >= chargeTime_){
			phaseT_ = 0;
			currentPhase_ = InAttackBehaviorPhase::Attack;
			break;
		}
		break;
	}
	case InAttackBehaviorPhase::Attack:{
		float t = static_cast<float>(phaseT_) / static_cast<float>(attackTime_);
		leftArmRotate_ = EasingLerp(Easing::easeOutCubic,t,startLeftArmRotate_,swingLeftArmRotate_);
		rightArmRotate_ = EasingLerp(Easing::easeOutCubic,t,startRightArmRotate_,swingRightArmRotate_);
		weaponRotate_ = EasingLerp(Easing::easeOutCubic,t,startWeaponRotate_,swingWeaponRotate_);

		if(phaseT_ >= attackTime_){
			phaseT_ = 0;
			currentPhase_ = InAttackBehaviorPhase::Recovery;
			break;
		}
		break;
	}
	case InAttackBehaviorPhase::Recovery:{

		if(comboNext_){
			if(phaseT_ >= recoveryTime_){
				host_->TransitionAttackBehavior(new HammerRotating(host_));
				return;
			}
		} else{
			float t = static_cast<float>(phaseT_) / static_cast<float>(attackTime_);
			leftArmRotate_ = EasingLerp(Easing::easeOutCubic,1.0f - t,swingLeftArmRotate_,{0.0f,0.0f,0.0f});
			rightArmRotate_ = EasingLerp(Easing::easeOutCubic,1.0f - t,swingRightArmRotate_,{0.0f,0.0f,0.0f});
			weaponRotate_ = EasingLerp(Easing::easeOutCubic,1.0f - t,swingWeaponRotate_,{0.0f,0.0f,0.0f});

			if(phaseT_ >= recoveryTime_){
				host_->TransitionAttackBehavior(nullptr);
				return;
			}

		}

		break;
	}
	default:
		break;
	}
	host_->setPartsRotate("Weapon",weaponRotate_);
	host_->setPartsRotate("LeftArm",leftArmRotate_);
	host_->setPartsRotate("RightArm",rightArmRotate_);

	if(comboNext_){
		return;
	}
	if(input_->TriggerKey(DIK_E)){
		comboNext_ = true;
	}
}