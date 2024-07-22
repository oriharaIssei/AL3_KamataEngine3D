#include "HammerDown.h"

#include "GlobalVariables.h"

#include "HammerSwing.h"

#include "Player.h"

#include "Easing.h"

void HammerDown::Init(){
	currentPhase_ = InAttackBehaviorPhase::Anticipation;

	GlobalVariables *variables = GlobalVariables::getInstance();

	variables->addValue<int32_t>("PlayerAttack_HammerDown","anticipationTime_",anticipationTime_);
	variables->addValue<int32_t>("PlayerAttack_HammerDown","chargeTime_",chargeTime_);
	variables->addValue<int32_t>("PlayerAttack_HammerDown","attackTime_",attackTime_);
	variables->addValue<int32_t>("PlayerAttack_HammerDown","recoveryTime_",recoveryTime_);

	variables->addValue<float>("PlayerAttack_HammerDown","startArmRotateX_",startArmRotateX_);
	variables->addValue<float>("PlayerAttack_HammerDown","swingArmRotateX_",swingArmRotateX_);
	variables->addValue<float>("PlayerAttack_HammerDown","chargeArmRotateX_",chargeArmRotateX_);

	variables->addValue<float>("PlayerAttack_HammerDown","startWeaponRotateX_",startWeaponRotateX_);
	variables->addValue<float>("PlayerAttack_HammerDown","swingWeaponRotateX_",swingWeaponRotateX_);
	variables->addValue<float>("PlayerAttack_HammerDown","chargeWeaponRotateX_",chargeWeaponRotateX_);

	phaseT_ = 0;

	comboNext_ = false;

	armRotateX_ = startArmRotateX_;
	host_->setPartsRotate("LeftArm",{armRotateX_,0.0f,0.0f});
	host_->setPartsRotate("RightArm",{armRotateX_,0.0f,0.0f});

	host_->setPartsRotate("Body",{0.0f,0.0f,0.0f});

	host_->setPartsRotate("Weapon",{0.0f,0.0f,0.0f});
	host_->setPartsTranslate("Weapon",{0.0f,7.3f,0.0f});
}

void HammerDown::Update(){
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
		armRotateX_ = EasingLerp(Easing::easeOutQuart,static_cast<float>(phaseT_) / static_cast<float>(chargeTime_),startArmRotateX_,chargeArmRotateX_);
		weaponRotateX_ = EasingLerp(Easing::easeOutQuart,static_cast<float>(phaseT_) / static_cast<float>(chargeTime_),startWeaponRotateX_,chargeWeaponRotateX_);

		if(phaseT_ >= chargeTime_){
			phaseT_ = 0;
			armRotateX_ = chargeArmRotateX_;
			currentPhase_ = InAttackBehaviorPhase::Attack;
			break;
		}

		break;
	}
	case InAttackBehaviorPhase::Attack:{
		armRotateX_ = EasingLerp(Easing::easeOutQuart,static_cast<float>(phaseT_) / static_cast<float>(attackTime_),chargeArmRotateX_,swingArmRotateX_);
		weaponRotateX_ = EasingLerp(Easing::easeOutQuart,static_cast<float>(phaseT_) / static_cast<float>(attackTime_),chargeWeaponRotateX_,swingWeaponRotateX_);

		if(phaseT_ >= attackTime_){
			phaseT_ = 0;
			currentPhase_ = InAttackBehaviorPhase::Recovery;
			break;
		}
		break;
	}
	case InAttackBehaviorPhase::Recovery:{
		if(comboNext_){
			if(phaseT_ >= recoveryTime_ / 2){
				//	player's AttackBehavior Transition
				host_->TransitionAttackBehavior(new HammerSwing(host_));
				return;
			}
			
		} else{
			armRotateX_ = EasingLerp(Easing::easeOutQuart,static_cast<float>(phaseT_) / static_cast<float>(attackTime_),swingArmRotateX_,0.0f);
			if(phaseT_ >= recoveryTime_){
				//	player's AttackBehavior Transition
				host_->TransitionAttackBehavior(nullptr);
				return;
			}
		}
		break;
	}
	default:
		break;
	}

	host_->setPartsRotate("Weapon",{weaponRotateX_,0.0f,0.0f});
	host_->setPartsRotate("LeftArm",{armRotateX_,0.0f,0.0f});
	host_->setPartsRotate("RightArm",{armRotateX_,0.0f,0.0f});

	if(comboNext_){
		return;
	}
	if(input_->TriggerKey(DIK_E)){
		comboNext_ = true;
	}
}