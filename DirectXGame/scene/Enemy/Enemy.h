#pragma once

#include "Model.h"

#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>

enum class Phase { Approach, Leave };
class Enemy;
class BaseEnemyState {
public:
	BaseEnemyState(Enemy* host) : host_(host){};
	virtual ~BaseEnemyState(){};

	virtual void Update() = 0;

protected:
	Enemy* host_;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	EnemyStateApproach(Enemy* host) : BaseEnemyState(host){};
	~EnemyStateApproach() override{};

	void Update() override;

private:
	Vector3 velocity = {0.0f, 0.0f, -0.2f};
};

class EnemyStateLeave : public BaseEnemyState {
public:
	EnemyStateLeave(Enemy* host) : BaseEnemyState(host){};
	~EnemyStateLeave() override{};

	void Update() override;

private:
	Vector3 velocity = {-0.5f, 0.5f, 0.0f};
};

class Enemy {
public:
	void Init(const Vector3& pos);
	void Update();
	void Draw(const ViewProjection& viewProj);

	void ChangeState(BaseEnemyState* nextState);

private:
	WorldTransform worldTransform_;
	const float kSpeed_ = 0.2f;

	std::unique_ptr<Model> model_ = nullptr;

	std::unique_ptr<BaseEnemyState> state_;

	uint32_t th_;

public:
	const Vector3& getPos() const { return worldTransform_.translation_; }
	void setPos(const Vector3& pos) { worldTransform_.translation_ = pos; }
};

