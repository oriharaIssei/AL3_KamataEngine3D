#pragma once

#include "Input.h"

#include "Collider.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "Sprite.h"

#include "WorldTransform.h"
#include <Vector2.h>

#include <list>
#include <memory>

class Enemy;
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
	void Update(const ViewProjection &viewProj);
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection &viewProj);
	void DrawUI(const ViewProjection &viewProj);

	void OnCollision()override {};

	void LockOn();
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

	std::list<std::unique_ptr<Enemy>> *enemyList_;

	WorldTransform worldTransform_;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	std::unique_ptr<Model> model_;

	const float kSpeed_ = 0.2f;
	// 旋回スピード
	const float kRotSpeed_ = 0.02f;
	// 弾の速度
	const float kBuletSpeed_ = 1.0f;

	uint32_t modelTh_;

	WorldTransform worldTransform3DReticle_;
	std::unique_ptr<Sprite> reticle_;
	Vector2 mousePos_;
	uint32_t reticleTh_;
public:
	void setCameraTransform(const WorldTransform *transform) {
		worldTransform_.parent_ = transform;
	}
	Vector3 getWorldPos() const override;
	const std::list<std::unique_ptr<PlayerBullet>> &getBullets() const { return bullets_; }
	const void setEnemyList(std::list<std::unique_ptr<Enemy>> *enemyList) { enemyList_ = enemyList; };
};

class PlayerAttackCommand {
public:
	virtual void Update() = 0;
protected:
	Player *host;
};

class NormalAttack :public PlayerAttackCommand {
public:
	void Update()override;
};
class MultiLockon :public PlayerAttackCommand {
public:
	void Update()override;
};