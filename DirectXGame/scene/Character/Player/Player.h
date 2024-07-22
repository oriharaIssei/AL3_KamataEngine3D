#pragma once

#include <optional>

#include <array>

#include "Input.h"

#include "BaseCharacter.h"

#include "PartsModel.h"

#include "Vector2.h"
#include <stdint.h>

#include "IAttackBehavior.h"

class Player :
	public BaseCharacter{
	enum class Behavior{
		kRoot,
		kAttack,
		kDash,
		kJump
	};
public:
	void Init()override;
	void Update()override;
	void Draw(const ViewProjection &viewProj)override;
private:
	void BehaviorRootInit();
	void BehaviorRootUpdate();

	void BehaviorAttackInit();
	void BehaviorAttackUpdate();

	void BehaviorDashInit();
	void BehaviorDashUpdate();

	void BehaviorJumpInit();
	void BehaviorJumpUpdate();

private:
	Input *input_ = nullptr;
	float speed_ = 0.4f;

	std::optional<Behavior> behaviorRequest_;
	Behavior			    currentBehavior_;

	const ViewProjection *viewProjection_ = nullptr;

	std::unique_ptr<IAttackBehavior> currentAttackBehavior_;

	Vector3 lastDir_;
	Vector3 move_;

	Vector3 velocity_;

	/* 浮遊ギミック */
	void InitFloatingGimmick();
	void UpdateFloatingGimmick();

	float floatingParameter_;
	float floatingAmplitude_;

	struct WorkDash{
		float speed_ = 1.8f;

		float t_;
		float maxT_ = 12;
	};
	WorkDash workDash_;

	float jumpPowar_ = 5.0f;
public:
	void TransitionAttackBehavior(IAttackBehavior *nextBehavior);

	const Vector3 &getDir()const{ return lastDir_; }

	void setViewProjection(const ViewProjection *viewProj){ viewProjection_ = viewProj; }
};