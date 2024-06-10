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

	debugCamera = std::make_unique<DebugCamera>(WinApp::kWindowWidth,WinApp::kWindowHeight);
	followCamera = std::make_unique<FollowCamera>();
	followCamera->Init();

	viewProj_.Initialize();

	player_ = std::make_unique<Player>();
	player_->Init();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Init();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();
	ground_ = std::make_unique<Ground>();
	ground_->Init();

	followCamera->SetTarget(&player_->getWorldTransform());
	player_->setViewProjection(&followCamera->getViewProjection());

	isDebug = false;
}

void GameScene::Update() {
#ifdef _DEBUG
	if(input_->TriggerKey(DIK_LCONTROL)) {
		isDebug = !isDebug;
	}
	if(isDebug) {
		debugCamera->Update();
		viewProj_.matView = debugCamera->GetViewProjection().matView;
		viewProj_.matProjection = debugCamera->GetViewProjection().matProjection;
	}
#endif // _DEBUG
	player_->Update();
	
	enemy_->Update();
	
	if(!isDebug) {
		followCamera->Update(input_);
		viewProj_.matView = followCamera->getViewProjection().matView;
		viewProj_.matProjection = followCamera->getViewProjection().matProjection;
	}

	viewProj_.TransferMatrix();
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
	ground_->Draw(viewProj_);

	enemy_->Draw(viewProj_);
	player_->Draw(viewProj_);

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
