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

	const WorldTransform &getWorldTransform()const{ return worldTransform_; }
	void setTransform(const Transform &transform){
		worldTransform_.scale_ = transform.scale;
		worldTransform_.rotation_ = transform.rotate;
		worldTransform_.translation_ = transform.translate;
	}
	void addTranslate(const Vector3 &add){
		worldTransform_.translation_ += add;
	}

	void setScale(const Vector3 &scale){
		worldTransform_.scale_ = scale;
	}
	void setRotate(const Vector3 &rotate){
		worldTransform_.rotation_ = rotate;
	}
	void setTranslate(const Vector3 &translate){
		worldTransform_.translation_ = translate;
	}

	const WorldTransform &getPartsTransform(const std::string &partsName){ return partsModels_[partsName]->worldTransform; }
	void setPartsTransform(const std::string &partsName,const Transform &transform){
		partsModels_[partsName]->worldTransform.scale_ = transform.scale;
		partsModels_[partsName]->worldTransform.rotation_ = transform.rotate;
		partsModels_[partsName]->worldTransform.translation_ = transform.translate;
	}
	void setPartsScale(const std::string &partsName,const Vector3 &scale){
		partsModels_[partsName]->worldTransform.scale_ = scale;
	}
	void setPartsRotate(const std::string &partsName,const Vector3 &rotate){
		partsModels_[partsName]->worldTransform.rotation_ = rotate;
	}
	void setPartsTranslate(const std::string &partsName,const Vector3 &translate){
		partsModels_[partsName]->worldTransform.translation_ = translate;
	}

	void addPartsTransform(const std::string &partsName,const Transform &transform){
		partsModels_[partsName]->worldTransform.scale_ += transform.scale;
		partsModels_[partsName]->worldTransform.rotation_ += transform.rotate;
		partsModels_[partsName]->worldTransform.translation_ += transform.translate;
	}
	void addPartsScale(const std::string &partsName,const Vector3 &scale){
		partsModels_[partsName]->worldTransform.scale_ += scale;
	}
	void addPartsRotate(const std::string &partsName,const Vector3 &rotate){
		partsModels_[partsName]->worldTransform.rotation_ += rotate;
	}
	void addPartsTranslate(const std::string &partsName,const Vector3 &translate){
		partsModels_[partsName]->worldTransform.translation_ += translate;
	}

	void setViewProjection(const ViewProjection *viewProj){ viewProjection_ = viewProj; }
};