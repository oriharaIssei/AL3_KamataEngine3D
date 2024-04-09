#pragma once

#include "Model.h"
#include <memory>

#include "WorldTransform.h"

class Player {
public:
	Player() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// /// <param name="textureHandle">テクスチャハンドル</param>
	void Init(Model* model, uint32_t textureHandle);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProj);

private:
	WorldTransform worldTransform_;

	std::unique_ptr<Model> model_;
	const float kSpeed = 0.2f;
	uint32_t th_;
};
