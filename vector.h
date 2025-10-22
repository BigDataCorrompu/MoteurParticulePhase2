#pragma once
#include "ofMain.h"

class vect {
public:
	float x, y, z;
	vect();
	vect(float x, float y, float z);

	vect operator/(float constante);
	vect operator+(vect & others);
	vect operator*(float constante);
	vect operator-(vect & others);
	void operator+=(vect & others);

	float ProduitScalaire(vect & others);
	vect ProduitVectoriel(vect & others);

	float getNorm();
	float getNormSquared();

	vect normalize();
	vect composantProduct(vect & other);
};
