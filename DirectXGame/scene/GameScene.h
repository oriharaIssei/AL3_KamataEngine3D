#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "memory"

#include "Model.h"
#include "Sprite.h"

#include "Player.h"

#include "FollowCamera.h"

#include "Ground.h"
#include "Skydome.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

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

	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<FollowCamera> followCamera;

	ViewProjection viewProj_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Player> player_;
};