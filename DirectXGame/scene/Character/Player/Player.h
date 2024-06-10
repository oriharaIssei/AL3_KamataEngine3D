#pragma once

#include "Input.h"

#include "BaseCharacter.h"

#include "Vector2.h"
#include <stdint.h>

class Player :public BaseCharacter {
public:
	void Init()override;
	void Update()override;
	void Draw(const ViewProjection &viewProj)override;

private:
	Input *input_ = nullptr;
	float speed_ = 0.4f;

	const ViewProjection *viewProjection_ = nullptr;

	Vector3 lastDir_;
	Vector3 move_;

	/* 浮遊ギミック */
	void InitFloatingGimmick();
	void UpdateFloatingGimmick();

	float floatingParameter_;
	float floatingAmplitude_;
public:
	const WorldTransform &getWorldTransform()const { return worldTransform_; }
	void setViewProjection(const ViewProjection *viewProj) { viewProjection_ = viewProj; }
};

