#pragma once

#include <optional>

#include "Input.h"

#include <string>

class Player;
enum class InAttackBehaviorPhase{
	Anticipation,
	Charge,
	Attack,
	Recovery
};
class IAttackBehavior{
public:
	IAttackBehavior(Player *host) :host_(host){ input_ = Input::GetInstance(); };
	virtual ~IAttackBehavior(){};

	virtual void Init() = 0;
	virtual void Update() = 0;
protected:
	Input *input_;
	Player *host_ = nullptr;

	InAttackBehaviorPhase currentPhase_;

	int32_t anticipationTime_;
	int32_t chargeTime_;
	int32_t attackTime_;
	int32_t recoveryTime_;

	int32_t phaseT_;

	bool comboNext_;
};