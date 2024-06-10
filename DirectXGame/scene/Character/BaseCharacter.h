#pragma once

#include <map>
#include <memory>

#include <string>

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "PartsModel.h"

class BaseCharacter {
public:
	virtual ~BaseCharacter() {};
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw(const ViewProjection &viewProj);
protected:
	/// <summary>
	/// Character の パーツ
	/// </summary>
	std::map<std::string,std::unique_ptr<PartsModel>> partsModels_;

	WorldTransform worldTransform_;
};