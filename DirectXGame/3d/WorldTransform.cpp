#include "WorldTransform.h"

#include <Matrix4x4.h>

void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeMatrix::Affine(scale_, rotation_, translation_);

	if(parent_) {
		matWorld_ = matWorld_ * parent_->matWorld_;
	}
	TransferMatrix();
}
