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
class Player;

class PlayerAttackCommand {
public:
	PlayerAttackCommand(Player *hostPtr):host(hostPtr) {};
	virtual ~PlayerAttackCommand() {};

	virtual void Update(const ViewProjection &viewProj) = 0;
protected:
	Player *host;
};

class NormalAttack :public PlayerAttackCommand {
public:
	NormalAttack(Player *hostPtr):PlayerAttackCommand(hostPtr) {};
	~NormalAttack()override {};
	void Update(const ViewProjection &viewProj)override;
};
class MultiLockon :public PlayerAttackCommand {
public:
	MultiLockon(Player *hostPtr):PlayerAttackCommand(hostPtr) {};
	~MultiLockon()override { lockOnEnemyList_.clear(); }
	void Update(const ViewProjection &viewProj)override;
private:
	std::list<Enemy *> lockOnEnemyList_;
};

class Player :public Collider {
	friend NormalAttack;
	friend MultiLockon;
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
	void DrawUI();

	void OnCollision()override {};

private:
	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Shot();

	void SingleLockOn(const ViewProjection &viewProj);
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
	Vector3 reticlePos_;
	Vector2 mousePos_;
	Vector4 reticleColor_;
	uint32_t reticleTh_;

	std::unique_ptr<PlayerAttackCommand> attackCommand_ = nullptr;
public:
	void setCameraTransform(const WorldTransform *transform) {
		worldTransform_.parent_ = transform;
	}

	Vector3 getWorldPos() const override;

	const std::list<std::unique_ptr<PlayerBullet>> &getBullets() const { return bullets_; }

	const void setEnemyList(std::list<std::unique_ptr<Enemy>> *enemyList) { enemyList_ = enemyList; };
	const std::list<std::unique_ptr<Enemy>> &getEnemyList()const { return *enemyList_; }

	const WorldTransform &getWorldTransform()const { return worldTransform_; }
};