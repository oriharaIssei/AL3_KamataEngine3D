#pragma once

#include "Model.h"

#include "Vector3.h"
#include "WorldTransform.h"

class Player;
class EnemyBullet {
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">出現位置</param>
	/// <param name="velocity">速度Vector</param>
	void Init(Model* model, const Vector3& pos,Player* player);

	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_;
	Vector3 velocity_;

	static const int32_t kLifeTime_ = 60 * 5;
	int32_t deathTimer_ = kLifeTime_;
	bool isDead_ = false;

	uint32_t th_;

	Player* player_p;

public:
	bool IsDead() const { return isDead_; }
};