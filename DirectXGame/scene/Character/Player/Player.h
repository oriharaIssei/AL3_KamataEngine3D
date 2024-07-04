#pragma once

#include <optional>

#include "Input.h"

#include "BaseCharacter.h"

#include "PartsModel.h"

#include "Vector2.h"
#include <stdint.h>

class Player :public BaseCharacter {
	enum class Behavior {
		kRoot,
		kAttack
	};
public:
	void Init()override;
	void Update()override;
	void Draw(const ViewProjection &viewProj)override;
private:
	void BehaviorRootUpdate();
	void BehaviorRootInit();
	void BehaviorAttackUpdate();
	void BehaviorAttackInit();
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

	/* BehaviorAttack用 */
	float t_;
	float maxT_;

public:
	const WorldTransform &getWorldTransform()const { return worldTransform_; }
	void setViewProjection(const ViewProjection *viewProj) { viewProjection_ = viewProj; }
};

