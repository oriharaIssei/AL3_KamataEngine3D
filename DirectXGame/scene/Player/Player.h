#pragma once

#include "Input.h"

#include "Bullet/PlayerBullet.h"
#include "Model.h"

#include "WorldTransform.h"

#include <list>
#include <memory>

class Player {
public:
	Player() = default;
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// /// <param name="textureHandle">テクスチャハンドル</param>
	void Init(Model* model, uint32_t textureHandle);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProj);

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
	Input* input_ = nullptr;

	WorldTransform worldTransform_;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	std::unique_ptr<Model> model_;

	const float kSpeed = 0.2f;
	// 旋回スピード
	const float kRotSpeed_ = 0.02f;

	uint32_t th_;
};
