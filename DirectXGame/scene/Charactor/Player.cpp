#include "Player.h"

#include "PartsModel.h"
#include "ViewProjection.h"

#include "Vector3.h"

void Player::Init(){
}

void Player::Update(){
}

void Player::Draw(const ViewProjection& viewProjection){
	body_->Draw(viewProjection);
	hand_->Draw(viewProjection);
}