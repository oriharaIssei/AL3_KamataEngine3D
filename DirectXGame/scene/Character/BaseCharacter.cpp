#include "BaseCharacter.h"

void BaseCharacter::Init(){
	worldTransform_.Initialize();
	collider_ = std::make_unique<Collider>();
}

void BaseCharacter::Draw(const ViewProjection &viewProj){
	for(auto &model : partsModels_){
		model.second->Draw(viewProj);
	}
}
