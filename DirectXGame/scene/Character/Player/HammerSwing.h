#pragma once

#include "IAttackBehavior.h"

#include "Vector3.h"

class HammerSwing :
	public IAttackBehavior{
public:
	HammerSwing(Player *host) :IAttackBehavior(host){};
	~HammerSwing()override{};
	void Init() override;
	void Update() override;
private:
	Vector3 leftArmRotate_;
	Vector3 rightArmRotate_;
	Vector3 weaponRotate_;

	Vector3 startLeftArmRotate_;
	Vector3 startRightArmRotate_;
	Vector3 startWeaponRotate_;

	Vector3 swingLeftArmRotate_;
	Vector3 swingRightArmRotate_;
	Vector3 swingWeaponRotate_;
};