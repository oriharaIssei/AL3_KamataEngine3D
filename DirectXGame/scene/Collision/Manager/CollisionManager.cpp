#include "CollisionManager.h"

#include "lib/GlobalVariables/GlobalVariables.h"

void CollisionManager::Init(){
	model_.reset(Model::CreateSphere());

	GlobalVariables::getInstance()->addValue("CollisionManager","isDrawColliders_",isDrawColliders_);
}

void CollisionManager::Draw(const ViewProjection& viewProj){
	if(!isDrawColliders_){
		return;
	}
	for(auto& collider : colliders_){
		collider->Draw(model_.get(),viewProj);
	}
}

void CollisionManager::Update(){
	for(auto aItr = colliders_.begin(); aItr != colliders_.end();){
		Collider* a = *aItr;

		std::list<Collider*>::iterator bItr = aItr;
		++bItr;// aItr の次
		for(; bItr != colliders_.end(); ++bItr){
			Collider* b = *bItr;

			if(CheckCollisionPair(a,b)){
				a->OnCollision(b);
				b->OnCollision(a);
			}
		}
	}
}

bool CollisionManager::CheckCollisionPair(const Collider* a,const Collider* b){
	if((a->getPosition() - b->getPosition()).lenghtSq() <= a->getRadius() + b->getRadius() * (a->getRadius() + b->getRadius())){
		return true;
	}
	return false;
}
