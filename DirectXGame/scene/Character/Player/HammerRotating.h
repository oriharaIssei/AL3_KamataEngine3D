#pragma once
#include "IAttackBehavior.h"

#include "Vector3.h"

#include "stdint.h"

class HammerRotating :
	public IAttackBehavior{
public:
	HammerRotating(Player *host) :IAttackBehavior(host){};
	~HammerRotating()override{};
	void Init() override;
	void Update() override;
private:
	Vector3 hostDir_;

	float moveSpeed_;
	float swingRotateSpeed_;

	int32_t maxAttackTime_;

	Vector3 rightArmRotate_;
	Vector3 weaponRotate_;
	Vector3 bodyRotate_;

	Vector3 swingedRightArmRotate_;
	Vector3 swingedWeaponRotate_;
	Vector3 swingedBodyRotate_;

	Vector3 startRightArmRotate_;
	Vector3 startWeaponRotate_;

	Vector3 chargeRightArmRotate_;
	Vector3 chargeWeaponRotate_;
};