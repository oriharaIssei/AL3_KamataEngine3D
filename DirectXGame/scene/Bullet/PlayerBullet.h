#pragma once

#include "Model.h"

#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>

class PlayerBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">出現位置</param>
	void Init(Model* model,const Vector3& pos);
	
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	std::unique_ptr<Model> model_;
	WorldTransform worldTransform_;
	uint32_t th_;
};
