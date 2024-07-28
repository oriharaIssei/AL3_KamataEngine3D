#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"

#include <memory>

#include "WorldTransform.h"
#include "ViewProjection.h"

#include "Model.h"
#include "Sprite.h"

#include "Ground.h"
#include "Skydome.h"

#include "Player.h"
#include "Enemy/Enemy.h"

#include "FollowCamera.h"

#include "LockOn.h"

#include "Collision/Manager/CollisionManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon *dxCommon_ = nullptr;
	Input *input_ = nullptr;
	Audio *audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	bool isDebug;

	ViewProjection viewProj_;

	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<FollowCamera> followCamera;

	std::unique_ptr<LockOn> lockOn_;

	std::unique_ptr<CollisionManager> collisionManager_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<Enemy>> enemies_;
};