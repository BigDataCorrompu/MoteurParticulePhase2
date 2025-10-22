#pragma once
#include "baseObject.h"
#include <vector>
#include "particule.h"
#include "vector.h"
#include "ressort.h"

enum direction {
	left = 0,
	right = 1,
	up = 2,
	down = 3
};

class Blob : public baseObject {
public:
	Blob();
	Blob(particule*, ParticuleForceRegistry*);
	void Draw();
	void SetParticuleMaster(particule*);
	std::vector<particule*> GetParticuleArray();
	void MoveBlob(direction);
	particule * getHeadBlob();
	std::vector<Blob> SplitBlob();
	bool inside(particule);
	bool checkCollision(particule);
	void addParticule(particule *);
	void setRegisterForce(ParticuleForceRegistry *);
	void clear();
	bool splitBlob(ParticuleForceRegistry * fg, ressort * f, Blob* newBlob);
	bool mergeBlob(ParticuleForceRegistry * fg, Blob* othersBlob, ressort *r);
	int getNumparticule();
	ressort * getRessort();
	void setRessort(ressort *);
	void UpdatePositionResort();
	
private:
	std::vector<particule*> particuleArray;
	particule * blobHead = nullptr;
	ParticuleForceRegistry* registreForces = nullptr;
	ressort * r = nullptr;
	ofColor color = (255.0,0.0,0.0);

public:

};
