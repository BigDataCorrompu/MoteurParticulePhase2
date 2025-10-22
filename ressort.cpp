#include "ressort.h"

ressort::ressort() {

}

ressort::ressort(float newk, float newl) {
	k = newk;
	l = newl;
}

void ressort::setConstanteRaideur(float newk) {
	this->k = newk;
}

void ressort::setL0(float newl) {
	this->l = newl;
}

void ressort::ActiveMaxLength(bool state) {
	UseMaxLength = state;

}

void ressort::setBase(particule *p) {
	if (p != nullptr) {
		base = p;
	}
}

void ressort::SetMaxLength(float length) {
	maxLength = length;
}

void ressort::setPos(vect position) {
	pos = position;
}

vect ressort::getPos() {
	if (base != nullptr) {
		return base->getPosition();
	}
	return pos;
}

void ressort::updateForce(particule * p1, float duration) {
	if (base != nullptr) {
		pos = base->getPosition();
	}
	vect L = p1->getPosition() - pos;
	L.y *= (-1);
	float distance = L.getNorm();

	if (distance == 0) return;

	//Check si le ressort se comporte comme un cable a une certaine distance
	if (distance > maxLength && UseMaxLength) {
		if (base != nullptr) {
			printf("distance : %f\n", distance);
			//On crée une collision artificiel pour modéliser le comportement d'un cable
			p1->CreateCollision(base);
		}
	}

	vect dir = L / distance;
	vect force = dir * (- k * (distance - l)); // Loi de Hooke standard
	p1->addForce(force);
	if (base != nullptr) {
		base->addForce(force * (-1));
	}
	
}
