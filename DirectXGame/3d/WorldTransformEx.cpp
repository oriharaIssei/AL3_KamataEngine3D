#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeMatrix::Affine(scale_, rotation_, translation_);
	TransferMatrix();
}