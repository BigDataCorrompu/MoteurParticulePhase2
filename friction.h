#pragma once
#include "ofMain.h"
#include "particule.h"

class friction : public ParticuleForceGenerator {
public:
	float k;	//Constante de frottement
	bool quadatric = false;

	friction();
	friction(float);

	void setFriction(float);
	void setQuadratic(bool);
	float getFriction();
	bool getQuadratic();

	void updateForce(particule *, float) override;
};
