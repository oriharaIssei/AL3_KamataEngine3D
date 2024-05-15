#include "GameScene.h"

#include <cassert>
#include <fstream>

#include "myMath.h"

#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProj_);

	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);

	viewProj_.Initialize();

	player_ = std::make_unique<Player>();
	player_->Init({ 0.0f, 0.0f, 15.0f }, Model::Create(), TextureManager::Load("sample.png"));

	enemies_.push_back(std::make_unique<Enemy>());
	enemies_.back()->Init({ 20.0f, 2.0f, 40.0f }, player_.get(), this);

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	const size_t segmentCount = 100;
	std::vector<Vector3> controlPoints = {
		{0, 0, -40},
		{0, 0, -40},
		{0, 0, -40},
		{0, 0, -40},
		{0, 0, -40}
	};
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Init(controlPoints);
	for(size_t i = 0; i < segmentCount; ++i) {
		float t = 1.0f / segmentCount * i;
		pointsDrawing.push_back(CatmullRomInterpolation(controlPoints, t));
	}

	player_->setCameraTransform(&railCamera_->getWorldTransform());

	waitCommandTimer_ = (uint32_t)0;
	enemyPopDataPath_ = "./csvData/enemyPopCommands.csv";
	LoadEnemyPopData();
}

void GameScene::Update() {
#ifdef _DEBUG
	if(input_->TriggerKey(DIK_SPACE)) {
		// 結果を反転
		isDebugCameraActive_ ^= true;
	}

	if(isDebugCameraActive_) {
		// カメラの更新
		debugCamera_->Update();

		// 情報の受け渡し
		viewProj_.matView = debugCamera_->GetViewProjection().matView;
		viewProj_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// 転送
		viewProj_.TransferMatrix();
	} else {
		railCamera_->Update();
		viewProj_.matProjection = railCamera_->getViewProjection().matProjection;
		viewProj_.matView = railCamera_->getViewProjection().matView;
	}
#endif
#ifndef _DEBUG
	railCamera_->Update();
	viewProj_.matProjection = railCamera_->getViewProjection().matProjection;
	viewProj_.matView = railCamera_->getViewProjection().matView;
#endif // !_DEBUG

	// 転送
	viewProj_.TransferMatrix();

	player_->Update(viewProj_);
	EnemiesUpdate();
	BulletsUpdate();

	collisionManager_->setCollider(player_.get());
	for(auto &playerBullet : player_->getBullets()) {
		collisionManager_->setCollider(playerBullet.get());
	}

	collisionManager_->Update();
	collisionManager_->ClearColliders();

	skydome_->Update();
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList *commandList = dxCommon_->GetCommandList();

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

	skydome_->Draw(viewProj_);
	player_->Draw(viewProj_);
	for(auto &enemy : enemies_) {
		enemy->Draw(viewProj_);
	}
	for(auto &bullet : enemyBullet_) {
		bullet->Draw(viewProj_);
	}

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProj_);
	for(size_t i = 1; i < pointsDrawing.size(); i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing[i - 1], pointsDrawing[i], { 1.0f, 0.0f, 0.0f, 1.0f });
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI(viewProj_);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::BulletsUpdate() {
	enemyBullet_.remove_if([](std::unique_ptr<EnemyBullet> &bullet) { return bullet->IsDead() ? true : false; });
	for(auto &bullet : enemyBullet_) {
		bullet->Update();
		collisionManager_->setCollider(bullet.get());
	}
}

void GameScene::EnemiesUpdate() {
	enemies_.remove_if([](std::unique_ptr<Enemy> &enemy) {
		return enemy->IsDead() ? true : false;
	});

	UpdateEnemyPopCommands();
	if(input_->TriggerKey(DIK_2)) {
		// 仮のリスポーン設定
		enemies_.push_back(std::make_unique<Enemy>());
		enemies_.back()->Init({ 20.0f, 0.0f, 40.0f }, player_.get(), this);
	}

	for(auto &enemy : enemies_) {
		enemy->Update();
		collisionManager_->setCollider(enemy.get());
	}
}

void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open(enemyPopDataPath_);
	assert(file.is_open());

	// file の 文字列 の 全てを コピー
	enemyPopCommands_ << file.rdbuf();
}

void GameScene::UpdateEnemyPopCommands() {
	if(waitCommandTimer_-- > 0) {
		return;
	}

	std::string line;

	while(getline(enemyPopCommands_, line)) {
		std::istringstream line_stram(line);

		std::string word;

		getline(line_stram, word, ',');

		if(word.find("//") == 0) {
			continue;
		} else if(word.find("POP") == 0) {
			getline(line_stram, word, ',');
			float x = (float)std::atoi(word.c_str());
			getline(line_stram, word, ',');
			float y = (float)std::atoi(word.c_str());
			getline(line_stram, word, ',');
			float z = (float)std::atoi(word.c_str());

			enemies_.push_back(std::make_unique<Enemy>());
			enemies_.back()->Init({ x,y,z }, player_.get(), this);

		} else if(word.find("WAIT") == 0) {
			getline(line_stram, word, ',');
			waitCommandTimer_ = (uint32_t)std::atoi(word.c_str());
			break;/* - 中断 - */
		}
	}
}
