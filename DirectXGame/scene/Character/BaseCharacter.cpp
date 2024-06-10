#include "BaseCharacter.h"

void BaseCharacter::Draw(const ViewProjection &viewProj) {
	for(auto &model : partsModels_) {
		model.second->Draw(viewProj);
	}
}
