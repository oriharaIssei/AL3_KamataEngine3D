#pragma once

#include <optional>

#include "Input.h"

#include "BaseCharacter.h"

#include "PartsModel.h"

#include "Vector2.h"
#include <stdint.h>

class Player :public BaseCharacter{
	enum class Behavior{
		kRoot,
		kAttack,
		kDash
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

	void ApplyGlobalVariables();
private:
	Input *input_ = nullptr;
	float speed_ = 0.4f;

	std::optional<Behavior> behaviorRequest_;
	Behavior			    currentBehavior_;

	const ViewProjection *viewProjection_ = nullptr;

	Vector3 lastDir_;
	Vector3 move_;

	/* 浮遊ギミック */
	void InitFloatingGimmick();
	void UpdateFloatingGimmick();

	float floatingParameter_;
	float floatingAmplitude_;

	struct WorkAttack{
		float t_;
		float maxT_;
	};
	WorkAttack workAttack_;

	struct WorkDash{
		float speed_ = 1.8f;

		float t_;
		float maxT_ = 12;
	};
	WorkDash workDash_;

public:
	const WorldTransform &getWorldTransform()const{ return worldTransform_; }
	void setViewProjection(const ViewProjection *viewProj){ viewProjection_ = viewProj; }
};

