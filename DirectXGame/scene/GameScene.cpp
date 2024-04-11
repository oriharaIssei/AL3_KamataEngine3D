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
	std::list<Collider*> colliders;
	colliders.push_back(player_.get());
	colliders.push_back(enemy_.get());
	for (auto& playerBullet : player_->getBullets()) {
		colliders.push_back(playerBullet.get());
	}
	for (auto& enemyBullet : enemy_->getBullets()) {
		colliders.push_back(enemyBullet.get());
	}

	std::list<Collider*>::iterator itrA = colliders.begin();
	for (; itrA != colliders.end(); ++itrA) {
		Collider* colliderA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++; // itrAの次のポインタを指すようにする
		for (; itrB != colliders.end(); ++itrB) {
			Collider* colliderB = *itrB;

			CheckColliderPair(colliderA, colliderB);
		}
	}
}

void GameScene::CheckColliderPair(Collider* colliderA, Collider* colliderB) {
	if ((colliderA->getCollisionAttribute() & colliderB->getCollisionMask()) == 0 || (colliderB->getCollisionAttribute() & colliderA->getCollisionMask()) == 0) {
		return;
	}

	float distance;
	Vector3 posA;
	Vector3 posB;

	posA = colliderA->getWorldPos();
	posB = colliderB->getWorldPos();
	distance = powf((posA.x - posB.x), 2.0f) + powf((posA.y - posB.y), 2.0f) + powf((posA.z - posB.z), 2.0f);
	if (distance <= powf(colliderA->getRadius() + colliderB->getRadius(), 2.0f)) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}
