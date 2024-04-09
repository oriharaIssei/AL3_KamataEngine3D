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
	std::unique_ptr<Model> model_;
	uint32_t th_;
	WorldTransform worldTransform_;
};
