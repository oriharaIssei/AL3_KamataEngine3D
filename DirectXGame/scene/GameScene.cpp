#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProj_);

	camera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	viewProj_.Initialize();

	player_ = std::make_unique<Player>();
	player_->Init(Model::Create(), TextureManager::Load("sample.png"));

	enemy_ = std::make_unique<Enemy>();
	enemy_->Init({20.0f, 2.0f, 40.0f}, player_.get());
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		// 結果を反転
		isDebugCameraActive_ ^= true;
	}

	if (isDebugCameraActive_) {
		// カメラの更新
		camera_->Update();

		// 情報の受け渡し
		viewProj_.matView = camera_->GetViewProjection().matView;
		viewProj_.matProjection = camera_->GetViewProjection().matProjection;

		// 転送
		viewProj_.TransferMatrix();
	} else {
		viewProj_.UpdateMatrix();
	}

#endif

	player_->Update();

	if (input_->TriggerKey(DIK_2)) {
		enemy_->Init({20.0f, 0.0f, 40.0f}, player_.get());
	}
	enemy_->Update();

	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProj_);
	enemy_->Draw(viewProj_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	float distance;
	Vector3 posA;
	Vector3 posB;

	constexpr float radius = 4.0f;

	///========================================
	/// 自機 と 敵弾
	///========================================
	posA = player_->getWorldPos();
	for (auto& enemyBullet : enemy_->getBullets()) {
		posB = enemyBullet->getWorldPos();
		distance = powf((posA.x - posB.x), 2.0f) + powf((posA.y - posB.y), 2.0f) + powf((posA.z - posB.z), 2.0f);
		if (distance <= powf(radius, 2.0f)) {
			player_->OnCollision();
			enemyBullet->OnCollision();
		}
	}

	///========================================
	/// 自弾 と 敵
	///========================================
	posA = enemy_->getWorldPos();
	for (auto& playerBullet : player_->getBullets()) {
		posB = playerBullet->getWorldPos();
		distance = powf((posA.x - posB.x), 2.0f) + powf((posA.y - posB.y), 2.0f) + powf((posA.z - posB.z), 2.0f);
		if (distance <= powf(radius, 2.0f)) {
			enemy_->OnCollision();
			playerBullet->OnCollision();
		}
	}

	///========================================
	/// 自弾 と 敵弾
	///========================================

	for (auto& playerBullet : player_->getBullets()) {
		posA = playerBullet->getWorldPos();
		for (auto& enemyBullet : enemy_->getBullets()) {
			posB = enemyBullet->getWorldPos();
			distance = powf((posA.x - posB.x), 2.0f) + powf((posA.y - posB.y), 2.0f) + powf((posA.z - posB.z), 2.0f);
			if (distance <= powf(radius, 2.0f)) {
				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}
}
