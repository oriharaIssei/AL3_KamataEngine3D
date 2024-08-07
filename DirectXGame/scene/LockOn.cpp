#include "LockOn.h"

#include <list>

#include <Vector3.h>

#include <cmath>

#include "TextureManager.h"
#include "WinApp.h"

#include "GlobalVariables.h"

void LockOn::Init(){
	isLocked_ = false;
	sprite_.reset(Sprite::Create(TextureManager::Load("reticle.png"),{0.0f,0.0f}));
	sprite_->SetSize({36.0f,36.0f});

	target = nullptr;

	input_ = Input::GetInstance();

	GlobalVariables *variables = GlobalVariables::getInstance();
	variables->addValue("LockOn","minDistance_",minDistance_);
	variables->addValue("LockOn","maxDistance_",maxDistance_);
	variables->addValue("LockOn","angleRange_",angleRange_);
}

void LockOn::Update(const std::vector<std::unique_ptr<Enemy>> &enemies_,
					const ViewProjection &viewProj){
	XINPUT_STATE gamePadState;

	if(!input_->GetJoystickState(0,gamePadState)){
		return;
	}
	if(gamePadState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER){
		isLocked_ = !isLocked_;
	}
	if(!isLocked_){
		target = nullptr;
		return;
	}
	std::list <std::pair <float,const Enemy *>>targets_;

	const Vector3 offset = {0.0f,3.0f,0.0f};
	for(auto &enemy : enemies_){
		Vector3 worldPos = TransformVector(offset,enemy->getWorldTransform().matWorld_);
		Vector3 viewPos = TransformVector(worldPos,viewProj.matView);
		if(minDistance_ <= viewPos.z && viewPos.z <= maxDistance_){
			float arcTangent = std::atan2(
				std::sqrtf(viewPos.x * viewPos.x + viewPos.y * viewPos.y),
				viewPos.z
			);
			if(std::abs(arcTangent) < angleRange_){
				targets_.emplace_back(std::make_pair(viewPos.z,enemy.get()));
			}
		}
	}

	if(targets_.empty()){
		isLocked_ = false;
		target = nullptr;
		return;
	}

	targets_.sort([](auto &a,auto &b){return a.second < b.second; });
	target = targets_.front().second;

	Vector3 reticlePos_ = target->getTransform_().matWorld_[3];
	reticlePos_ = TransformVector(reticlePos_,
								  viewProj.matView
								* viewProj.matProjection
								* MakeMatrix::ViewPort(0,0,WinApp::kWindowWidth,WinApp::kWindowHeight,0.0f,1.0f)
	);
	sprite_->SetPosition({reticlePos_.x,reticlePos_.y});
}

void LockOn::Draw(){
	if(!isLocked_){
		return;
	}
	sprite_->Draw();
}