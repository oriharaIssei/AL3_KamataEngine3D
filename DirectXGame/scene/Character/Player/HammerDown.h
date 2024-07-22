#pragma once
#include "IAttackBehavior.h"

#include <stdint.h>

class HammerDown :
	public IAttackBehavior{
public:
	HammerDown(Player *host) :IAttackBehavior(host){};
	~HammerDown()override{};

	void Init()override;
	void Update()override;
private:
	float armRotateX_;

	float startArmRotateX_;
	float chargeArmRotateX_;
	float swingArmRotateX_;

	float weaponRotateX_;

	float startWeaponRotateX_;
	float chargeWeaponRotateX_;
	float swingWeaponRotateX_;
};