#pragma once

#include "Input.h"

#include "Collider.h"
#include "Model.h"
#include "PlayerBullet.h"

#include "WorldTransform.h"

#include <list>
#include <memory>

class Player :public Collider {
public:
	Player() = default;
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// /// <param name="textureHandle">テクスチャハンドル</param>
	void Init(const Vector3 &pos, Model *model, uint32_t textureHandle);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection &viewProj);

	void OnCollision()override {};

private:
	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private:
	Input *input_ = nullptr;

	WorldTransform worldTransform_;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	std::unique_ptr<Model> model_;

	const float kSpeed_ = 0.2f;
	// 旋回スピード
	const float kRotSpeed_ = 0.02f;
	// 弾の速度
	const float kBuletSpeed_ = 1.0f;

	uint32_t th_;
public:
	void setCameraTransform(const WorldTransform *transform) {worldTransform_.parent_ = transform; }
	const Vector3 &getWorldPos() const override { return worldTransform_.translation_; }
	const std::list<std::unique_ptr<PlayerBullet>> &getBullets() const { return bullets_; }
};
