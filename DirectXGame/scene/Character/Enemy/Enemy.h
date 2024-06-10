#pragma once

#include "BaseCharacter.h"


class Enemy :
	public BaseCharacter {
public:
	void Init()override;
	void Update()override;
	void Draw(const ViewProjection &viewProj)override;
private:
	void WalkMotion();
	float walkParameter_;
};