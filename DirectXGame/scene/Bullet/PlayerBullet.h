#pragma once

#include "Model.h"

#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>

class PlayerBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">出現位置</param>
	/// <param name="velocity>速度</param>
	void Init(Model* model,const Vector3& pos,const Vector3& velocity);
	
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
public:
	bool IsDead() const { return isDead_; }
};
