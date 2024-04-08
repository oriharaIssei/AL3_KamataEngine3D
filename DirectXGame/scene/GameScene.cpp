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

	///==============================================
	/// 初期化
#pragma region "Initialize"
	worldT_.Initialize();
	camera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&camera_->GetViewProjection());

	PrimitiveDrawer::GetInstance()->Initialize();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&camera_->GetViewProjection());

	gh_ = TextureManager::Load("sample.png");
	spritePos_ = {0.0f, 0.0f};
	sprite_.reset(Sprite::Create(gh_, spritePos_));
	model_.reset(Model::Create());

	sh_ = audio_->LoadWave("mokugyo.wav");
	vh_ = audio_->PlayWave(sh_, true, 0.3f);
#pragma endregion
	///==============================================
}

void GameScene::Update() {

	ImGui::Begin("test");
	ImGui::DragFloat2("Sprite Position", &spritePos_.x, 0.1f);
	ImGui::End();

	sprite_->SetPosition(spritePos_);

	camera_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(vh_);
	}
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

	sprite_->Draw();

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

	model_->Draw(worldT_, camera_->GetViewProjection());

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	PrimitiveDrawer::GetInstance()->DrawLine3d({0.0f, 0.0f, 0.0f}, {5.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
	PrimitiveDrawer::GetInstance()->DrawLine3d({0.0f, 0.0f, 0.0f}, {0.0f, 5.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f});
	PrimitiveDrawer::GetInstance()->DrawLine3d({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 1.0f, 1.0f});

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
