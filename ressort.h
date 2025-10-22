#pragma once
#include "ofMain.h"
#include "particule.h"

class ressort : public ParticuleForceGenerator {
public:
	float k;	//Constante  de raideur du ressort
	float l;	// Longueur L0 du ressort au repos
	vect pos;	// Position de l'autre point du ressort
	float maxLength = 0;
	bool UseMaxLength = false;
	particule * base = nullptr;

	ressort();
	ressort(float, float);
	void setConstanteRaideur(float);
	void setL0(float);
	void ActiveMaxLength(bool);
	void setBase(particule *);
	void SetMaxLength(float);
	void setPos(vect);
	vect getPos();

	void updateForce(particule *, float) override;
};
