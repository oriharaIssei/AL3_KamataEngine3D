#pragma once

#include "Model.h"

#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <stdint.h>

#include <list>
#include <memory>

#include "EnemyBullet.h"

#include "TimedCall.h"

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
	Vector3 velocity = {0.5f, 0.3f, 0.0f};
};

class Player;
class Enemy {
public:
	void Init(const Vector3& pos, Player* player);
	void Update();
	void Draw(const ViewProjection& viewProj);

	void ChangeState(BaseEnemyState* nextState);

	void InitOnApproach();
	void Fire();

	void UpdateTimedCalls();
	void ClearTimedCalls() { timedCalls_.clear(); }
	
	void OnCollision(){};

public:
	static const uint32_t kFierInterval;

private:
	WorldTransform worldTransform_;
	const float kSpeed_ = 0.2f;
	const float kBulletSpeed_ = 0.4f;

	std::unique_ptr<Model> model_ = nullptr;

	std::unique_ptr<BaseEnemyState> state_;
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	std::list<std::unique_ptr<TimedCall<void, void>>> timedCalls_;

	int32_t fireTimer_ = 0;

	uint32_t th_;

	Player* player_ptr;

public:
	const Vector3& getWorldPos() const { return worldTransform_.translation_; }
	void setPos(const Vector3& pos) { worldTransform_.translation_ = pos; }

	const std::list<std::unique_ptr<EnemyBullet>>& getBullets() const { return bullets_; }

	void SetPlayer(Player* player) { player_ptr = player; }
};
