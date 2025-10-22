#include "friction.h"

friction::friction() {

}

friction::friction(float k_val) {
	k = k_val;
}

void friction::setFriction(float newValue) {
	k = newValue;
}

void friction::setQuadratic(bool newValue) {
	quadatric = newValue;
}

float friction::getFriction() {
	return k;
}

bool friction::getQuadratic() {
	return quadatric;
}

void friction::updateForce(particule * p1, float duration) {
	if (p1->getMasse() != 0) {
		if (quadatric) {
			vect forces;
			forces.x = std::abs(p1->getVitesse().getNorm()) * p1->getVitesse().x * -k;
			forces.y = std::abs(p1->getVitesse().getNorm()) * p1->getVitesse().y * -k;
			p1->addForce(forces);
		}
		else
		{
			vect forces;
			forces.x = p1->getVitesse().x * -k;
			forces.y = p1->getVitesse().y * -k;
			p1->addForce(forces);
		}	
	}
}
