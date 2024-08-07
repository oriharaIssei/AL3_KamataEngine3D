#pragma once

#include <unordered_map>
#include <memory>

#include <string>

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Collision/Collider.h"

#include "PartsModel.h"

class BaseCharacter{
public:
	virtual ~BaseCharacter(){};
	virtual void Init();
	virtual void Update() = 0;
	virtual void Draw(const ViewProjection &viewProj);
protected:
	/// <summary>
	/// Character の パーツ
	/// </summary>
	std::unordered_map<std::string,std::unique_ptr<PartsModel>> partsModels_;

	std::unique_ptr<Collider> collider_;

	WorldTransform worldTransform_;

public:
	Collider *getCollider()const{ return collider_.get(); }

	const WorldTransform &getTransform_()const{ return worldTransform_; }
	const WorldTransform &getWorldTransform()const{ return worldTransform_; }
	void setTransform(const Transform &transform){
		worldTransform_.scale_ = transform.scale;
		worldTransform_.rotation_ = transform.rotate;
		worldTransform_.translation_ = transform.translate;
	}
	void addTranslate(const Vector3 &add){
		worldTransform_.translation_ += add;
	}

	void setScale(const Vector3 &scale){
		worldTransform_.scale_ = scale;
	}
	void setRotate(const Vector3 &rotate){
		worldTransform_.rotation_ = rotate;
	}
	void setTranslate(const Vector3 &translate){
		worldTransform_.translation_ = translate;
	}

	const WorldTransform &getPartsTransform(const std::string &partsName){ return partsModels_[partsName]->worldTransform; }
	void setPartsTransform(const std::string &partsName,const Transform &transform){
		partsModels_[partsName]->worldTransform.scale_ = transform.scale;
		partsModels_[partsName]->worldTransform.rotation_ = transform.rotate;
		partsModels_[partsName]->worldTransform.translation_ = transform.translate;
	}
	void setPartsScale(const std::string &partsName,const Vector3 &scale){
		partsModels_[partsName]->worldTransform.scale_ = scale;
	}
	void setPartsRotate(const std::string &partsName,const Vector3 &rotate){
		partsModels_[partsName]->worldTransform.rotation_ = rotate;
	}
	void setPartsTranslate(const std::string &partsName,const Vector3 &translate){
		partsModels_[partsName]->worldTransform.translation_ = translate;
	}

	void addPartsTransform(const std::string &partsName,const Transform &transform){
		partsModels_[partsName]->worldTransform.scale_ += transform.scale;
		partsModels_[partsName]->worldTransform.rotation_ += transform.rotate;
		partsModels_[partsName]->worldTransform.translation_ += transform.translate;
	}
	void addPartsScale(const std::string &partsName,const Vector3 &scale){
		partsModels_[partsName]->worldTransform.scale_ += scale;
	}
	void addPartsRotate(const std::string &partsName,const Vector3 &rotate){
		partsModels_[partsName]->worldTransform.rotation_ += rotate;
	}
	void addPartsTranslate(const std::string &partsName,const Vector3 &translate){
		partsModels_[partsName]->worldTransform.translation_ += translate;
	}
};