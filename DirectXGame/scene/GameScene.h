#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "vector"
#include <memory>

#include "Collider.h"
#include "Enemy.h"
#include "Player.h"
#include "RailCamera.h"
#include "Skydome.h"

#include <sstream>

#include "CollisionManager.h"

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

private:
	void BulletsUpdate();
	void EnemiesUpdate();

	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();
private: // メンバ変数
	DirectXCommon *dxCommon_ = nullptr;
	Input *input_ = nullptr;
	Audio *audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<RailCamera> railCamera_;

	bool isDebugCameraActive_ = false;
	ViewProjection viewProj_;

	std::unique_ptr<CollisionManager> collisionManager_;

	std::unique_ptr<Player> player_;

	uint32_t waitCommandTimer_ = 0;
	std::stringstream enemyPopCommands_;
	std::string enemyPopDataPath_;
	std::list<std::unique_ptr<Enemy>> enemies_;
	std::list<std::unique_ptr<EnemyBullet>> enemyBullet_;

	std::unique_ptr<Skydome> skydome_;

	std::vector<Vector3> pointsDrawing;
public:
	void setEnemyBullet(EnemyBullet *bullet) { enemyBullet_.push_back(std::unique_ptr<EnemyBullet>(bullet)); }
};