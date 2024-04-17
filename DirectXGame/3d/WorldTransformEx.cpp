#include "WorldTransform.h"

#include "Vector3.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeMatrix::Affine(scale_, rotation_, translation_);
	if(parent_) {
		matWorld_ = matWorld_ * parent_->matWorld_;
	}
	TransferMatrix();
}
