#include "Blob.h"

Blob::Blob() {
	particuleArray.clear();
	//Résever la mémoire du tableau pour 256 éléments dans le tableau
	particuleArray.reserve(256);
}

Blob::Blob(particule* particuleHead, ParticuleForceRegistry* force) {
	particuleArray.clear();
	//Résever la mémoire du tableau pour 256 éléments dans le tableau
	particuleArray.reserve(256);
	particuleArray.push_back(particuleHead);
	registreForces = force;
}

void Blob::SetParticuleMaster(particule* p) {
	if (particuleArray.empty() != true) {
		particuleArray.clear();
	}
	particuleArray.push_back(p);
	blobHead = p;
}

std::vector<particule*> Blob::GetParticuleArray() {
	return particuleArray;
}

void Blob::MoveBlob(direction direction) {
	switch (direction) {
	case left:
		blobHead->setVitesse({ blobHead->getVitesse().x - 10, blobHead->getVitesse().y, blobHead->getVitesse().z });
		break;
	case right:
		blobHead->setVitesse({ blobHead->getVitesse() .x + 10, blobHead->getVitesse().y, blobHead->getVitesse().z });
		break;
	case up:
		blobHead->setVitesse({ blobHead->getVitesse().x, blobHead->getVitesse() .y + 10, blobHead->getVitesse().z });
		break;
	case down:
		blobHead->setVitesse({ blobHead->getVitesse().x, blobHead->getVitesse() .y - 10, blobHead->getVitesse().z });
		break;
	default:
		break;
	}
	return;
}

particule * Blob::getHeadBlob() {
	return blobHead;
}

std::vector<Blob> Blob::SplitBlob() {
	return std::vector<Blob>();
}

bool Blob::inside(particule p) {
	for (auto& part : particuleArray) {
		if (p.GetID() == part->GetID()) {
			return true;
		}
	}
	return false;
}

bool Blob::checkCollision(particule p) {
	for (auto& part : particuleArray) {
		if (part->checkCollision(p)) {
			return true;
		}
	}
	return false;
}

void Blob::addParticule(particule * p) {
	particuleArray.push_back(p);
}

void Blob::setRegisterForce(ParticuleForceRegistry *rf) {
	registreForces = rf;
}

bool Blob::splitBlob(ParticuleForceRegistry* fg, ressort *f, Blob* newBlob) {
	//on supprime seulement la particule du tableau et on retire le ressort qui le lie au blob
	if (particuleArray.size() <= 3 && particuleArray.size() > 1) {
		particule * p = particuleArray[particuleArray.size() -1];
		fg->remove(p, getRessort());
		particuleArray.erase(particuleArray.begin() + particuleArray.size() - 1);
		return false;
	}
	if (particuleArray.size() > 3) {
		int numParticule = particuleArray.size() / 2;
		int index = -1;
		// On coupe en deux le blob dans le sens de la verticalité pour évité qu'ils ne se regroupent dans un seul blob juste après
		for (int j = 0; j < numParticule; j++) {
			float posX = (float)ofGetWidth();
			for (int i = 0; i < particuleArray.size(); i++) {
				if (particuleArray[i]->getPosition().x < posX) {
					posX = particuleArray[i]->getPosition().x;
					index = i;
				}
			}
			//On ajoute l'élèment le plus à gauche dans le nouveau blob et le supprime de l'ancien
			//Si c'est le premier, on l'ajoute en tant que headBlob
			if (j == 0 && index > -1) {
				newBlob->SetParticuleMaster(particuleArray[index]);
			} else if (index > -1){
				newBlob->addParticule(particuleArray[index]);
			}
			particuleArray.erase(particuleArray.begin() + index);
		}

		//On ajoute le pointeur des forces dans le nouveaux blob et on modifie les anciens ressort de l'ancien headBlob
		// vers le nouveaux pour les particules qui ont changé de headBlob
		newBlob->setRegisterForce(fg);
		f->setBase(newBlob->getHeadBlob());
		f->ActiveMaxLength(true);
		f->SetMaxLength(150);
		for (auto& force : fg->m_Register) {
			// On vérifie si la particule est dans le nouveau blob
			if (newBlob->inside(*force.p)) {
				//On vérifie ensuite si la force est de type ressort
				if (dynamic_cast<ressort *>(force.fg)) {
					//On vérifie que le ressort est bien appliqué à une autre particule et non un point fixe
					if (dynamic_cast<ressort *>(force.fg)->base != nullptr) {
						//On vérifie si la forces est appliqué vers l'ancien headBlob on la transfert vers le nouveau
						if (dynamic_cast<ressort *>(force.fg)->base->GetID() == blobHead->GetID()) {
							force.fg = f;
						}
					}
				}
			}
		}
		// On supprime le blob qui a un ressort
		fg->remove(newBlob->getHeadBlob(), f);
		return true;
	}
	return false;
}

bool Blob::mergeBlob(ParticuleForceRegistry * fg, Blob* othersBlob, ressort* r) {

	//On mets a jour les forces vers le headBlob du playerBlob
	for (int i = 0; i < fg->m_Register.size(); i++) {
		if (dynamic_cast<ressort *>(fg->m_Register[i].fg)) {
			if (dynamic_cast<ressort *>(fg->m_Register[i].fg)->base == othersBlob->getHeadBlob()) {
				fg->m_Register[i].fg = getRessort();
			}
		}
	}
	//On ajoute également le headBlob de l'autre blob qui était pas dans les forces:
	fg->add(othersBlob->particuleArray[0], r);

	// on mets toutes les particules dans le blob player
	for (int i = 0; i < othersBlob->particuleArray.size(); i++) {
		addParticule(othersBlob->particuleArray[i]);
	}
	othersBlob->particuleArray.clear();
	
	return true;
}

int Blob::getNumparticule() {
	return particuleArray.size();
}

ressort * Blob::getRessort() {
	if (r != nullptr) {
		return r;
	}
	return nullptr;
}

void Blob::setRessort(ressort *ressort) {
	if (ressort != nullptr) {
		r = ressort;
	}
}

void Blob::clear() {
	particuleArray.clear();
	blobHead = nullptr;
	registreForces = nullptr;
}

void Blob::UpdatePositionResort() {
	for (auto& force : registreForces->m_Register) {
		if (inside(*force.p) && force.p->GetID() != blobHead->GetID()) {
			ressort* r = (ressort*)force.fg;
			r->setPos(blobHead->getPosition());

		}
	}
}


