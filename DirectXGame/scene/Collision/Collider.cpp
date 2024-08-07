#include "Collider.h"

void Collider::Init(const Vector3 &pos,float radius,std::function<void(Collider *)> onCollisionFunc){
	transform_.Initialize();
	transform_.translation_ = pos;
	transform_.scale_ = {radius / 2.0f,radius / 2.0f,radius / 2.0f};
	transform_.UpdateMatrix();

	radius_ = radius;

	onCollision_ = onCollisionFunc;
}

void Collider::Update(){
	transform_.UpdateMatrix();

	if(invincibleTime_ <= 0.0f){
		return;
	}
	invincibleTime_ -= 1.0f / 60.0f;
}

void Collider::Draw(Model *model,const ViewProjection &viewProj){
	model->Draw(transform_,viewProj);
}