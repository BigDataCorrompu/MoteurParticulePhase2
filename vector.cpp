#include "vector.h"

vect::vect() {
	// Default constructor: initializes an vector to (0,0,0)
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

vect::vect(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

vect vect::operator/(float scalar) {
	// Scalar division: scales all vector components by the constant
	return { x / scalar, y / scalar, z / scalar };
}



vect vect::operator*(float constante) {
	// Scalar multiplication: scales all vector components by the constant
	return vect(x * constante, y * constante, z * constante);
}

vect vect::operator+(vect & others) {
	// Vector addition: adds corresponding components
	return vect(x + others.x, y + others.y, z + others.z);
}

void vect::operator+=(vect & other) {
	// Compound addition: modifies the current vector by adding another
	x += other.x;
	y += other.y;
	z += other.z;
}

vect vect::operator-(vect & others) {
	// Vector subtraction: subtracts corresponding components
	return vect(x - others.x, y - others.y, z - others.z);
}


float vect::getNorm() {
	// Computes the Euclidean norm of the vector
	return sqrtf(x * x + y * y + z * z);
}

float vect::getNormSquared() {
	// Computes the Euclidean norm suaqred of the vector
	return (x * x + y * y + z * z);
}

vect vect::normalize() {
	// Normalize this vector
	float norm = this->getNorm();
	if (norm == 0.0f) {
		return vect(0.0f, 0.0f, 0.0f); //Returns the zero vector if it cannot be normalized
	}
	return vect(x / norm, y / norm, z / norm);
}

float vect::ProduitScalaire(vect & others) {
	// Dot product: returns the sum of the products of corresponding components
	return this->x * others.x + this->y * others.y + this->z * others.z;
}

vect vect::ProduitVectoriel(vect & others) {
	// Cross product: returns a vector perpendicular to the two input vectors
	vect value;
	value.x = this->y * others.z - this->z * others.y;
	value.y = this->z * others.x - this->x * others.z;
	value.z = this->x * others.y - this->y * others.x;
	return value;
}

vect vect::composantProduct(vect & other) {
	// multiplies each component of the vector with the corresponding component of another vector
	return vect(x * other.x, y * other.y, z * other.z);
}
