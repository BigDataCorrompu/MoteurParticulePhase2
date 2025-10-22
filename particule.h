#pragma once
#include "baseObject.h"
#include "ofMain.h"
#include "vector.h"

static int idNumber = 0;

class particule : baseObject {
private:
	vect position, vitesse, acceleration;
	vect accumForce;
	float damping, gravity, masse;
	std::vector<vect> trace;
	int iD;
	float size;
	ofColor color = (255, 255, 255);

public:
	particule();
	particule(vect pos);
	particule(vect pos, vect vit);
	particule(vect pos, vect vit, vect acc);
	vect getPosition();
	vect getVitesse();
	vect getAccel();
	void setColor(ofColor);
	float GetSize();

	int GetID();

	bool checkCollision(particule others);
	void SolveCollision(particule *others);

	void CreateCollision(particule * others);

	void setMasse(float m);
	void setVitesse(vect vit);
	void setGravity(float g);
	void setDamping(float f);

	float getMasse();
	float getGravity();
	float getDamping();

	std::vector<vect> getTraceArray();
	void addTracePosition(vect pos);
	void drawTrace();
	void drawTrace(ofColor color);

	void computeForce();
	void computeAcceleration();
	void addForce(vect);
	void clearForce();
	vect getAccumForce();

	void setPosition(vect pos);
	void updatePosition(float time);
	void EulerIntegration(float time, float width, float height);
	void VerletIntegration(float time);
	void draw();

	float getDistance(particule others);
};

class ParticuleForceGenerator{
public:
	virtual void updateForce(particule *, float);
};

class ParticuleForceRegistry {
public:
	struct ParticuleForceRegister {
		particule * p;
		ParticuleForceGenerator * fg;
	};
	typedef std::vector<ParticuleForceRegister> Registre;

	Registre m_Register;

	void add(particule * p, ParticuleForceGenerator * fg);
	void remove(particule * p, ParticuleForceGenerator * fg);
	void erase();
	bool inside(particule * p);
	void clear();
	std::vector<ParticuleForceRegister> GetForceRegister();
	void updateForces(float duration);
};

class particuleGravity : public ParticuleForceGenerator {
public:
	vect gravity;
	particuleGravity();
	particuleGravity(vect);
	void updateForce(particule *, float) override;
};
