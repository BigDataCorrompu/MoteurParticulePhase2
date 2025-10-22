#include "particule.h"
#include <numbers>
#include "ressort.h"

#define e 0.8

particule::particule() {
}

particule::particule(vect pos) {
	position = pos;
	vitesse = {0,0,0};
	gravity = 0.0f;
	masse = 1.0f;
	acceleration = { 0, 0, 0 };
	damping = 0.0f;
	size = 5;
	iD = idNumber++;
	trace.push_back(pos);
	
}

particule::particule(vect pos, vect vit) {
	position = pos;
	vitesse = vit;
	gravity = 0.0f;
	masse = 1.0f;
	acceleration = { 0, 0, 0 };
	damping = 0.0f;
	size = 5;
	iD = idNumber++;
	trace.push_back(pos);
}

particule::particule(vect pos, vect vit, vect acc) {
	position = pos;
	vitesse = vit;
	damping = 0;
	masse = 1.0f;
	gravity = 0;
	acceleration = acc;
	size = 5.0f;
	iD = idNumber++;
	trace.push_back(pos);
}

vect particule::getPosition() { return position; }
void particule::setPosition(vect pos) { position = pos; }
vect particule::getAccel() { return acceleration; }

void particule::setColor(ofColor newColor) {
	color = newColor;
}

float particule::GetSize() {
	return size;
}

vect particule::getVitesse() { return vitesse; }

void particule::updatePosition(float time) {
	vect deplacement(vitesse * time);
	position = position + deplacement;
}

void particule::draw() {
	ofSetColor(color);
	ofDrawCircle(glm::vec3(position.x, position.y, position.z), 5);
	ofSetColor(255, 255, 255);
}

float particule::getDistance(particule others) {
	return std::sqrtf(pow(others.getPosition().x - position.x, 2) + pow(others.getPosition().y - position.y, 2) + pow(others.getPosition().z - position.z, 2));
}

/// <summary>
/// Function that computes accelerations based on the forces applied to the particle
/// </summary>

//void particule::computeForce() {
//	if (masse != 0) {
//		// Computing the friction force: friction_force = -f * v^2
//		acceleration.x = std::abs(vitesse.getNorm()) * vitesse.x * -damping / masse;
//		acceleration.y = std::abs(vitesse.getNorm()) * vitesse.y * -damping / masse;
//	}
//	// Adding the friction force (Positive since the reference frame is inverted)
//	acceleration.y += gravity;
//}

void particule::computeAcceleration() {
	if (masse != 0) {
		acceleration = accumForce / masse;
	}
}

void particule::addForce(vect force) {
	accumForce += force;
}

void particule::clearForce() {
	accumForce = { 0, 0, 0 };
}

vect particule::getAccumForce() {
	return accumForce;
}

int particule::GetID() {
	return iD;
}

bool particule::checkCollision(particule others) {
	float distance = std::sqrtf(std::pow(others.getPosition().x - position.x, 2) + std::pow(others.getPosition().y - position.y, 2) + std::pow(others.getPosition().z - position.z, 2));
	if (distance < size + others.GetSize()) {
		return true;
	}
	return false;
}

void particule::SolveCollision(particule *others) {
	if (masse == 0 || others->getMasse() == 0) {
		//pas de prise en charge de collision quand la masse est nulle
		return;
	}
	float rapportMasse = masse / (masse + others->getMasse());
	

	//On calcule la distance entre les 2 objets
	float distance = std::sqrtf(std::pow(others->getPosition().x - position.x, 2) + std::pow(others->getPosition().y - position.y, 2) + std::pow(others->getPosition().z - position.z, 2));
	float penetration = size + others->GetSize() - distance;
	// On déplace les 2 particule en fonctionde la proportion de leur masse
	vect dir = { others->getPosition().x - position.x, others->getPosition().y - position.y, others->getPosition().z - position.z };
	dir = dir.normalize();

	vect vit_relative = others->getVitesse() - vitesse;
	if (vit_relative.ProduitScalaire(dir) > 0) {
		return;
	}

	//Calcule de l'impulsion
	float k = (e + 1) * vit_relative.ProduitScalaire(dir) / (1 / masse + 1 / others->getMasse());

	if (penetration > 0) {
		penetration /= 2;
		position = { position.x - penetration * rapportMasse * dir.x, position.y + penetration * rapportMasse * dir.y, position.z - penetration * rapportMasse * dir.z };
		others->setPosition({ others->getPosition().x + penetration * (1 - rapportMasse) * dir.x, others->getPosition().y - penetration * (1 - rapportMasse) * dir.y, others->getPosition().z + penetration * (1 - rapportMasse) * dir.z });
	}
	
	vitesse = { vitesse.x + (k / masse * dir.x), vitesse.y + (k / masse * dir.y), vitesse.z + (k / masse * dir.z) };
	others->setVitesse({ others->getVitesse().x - (k / others->getMasse() * dir.x), others->getVitesse().y - (k / others->getMasse() * dir.y), others->getVitesse().z - (k / others->getMasse() * dir.z) });
}

void particule::CreateCollision(particule * others) {
	vect dir = { others->getPosition().x - position.x, others->getPosition().y - position.y, others->getPosition().z - position.z };
	dir = dir.normalize();
	dir.y *= (-1);

	vect vit_relative = (others->getVitesse() - vitesse);
	float vit = vit_relative.ProduitScalaire(dir);

	float j = -vit / (1 / masse + 1 / others->getMasse());

	vect temp = dir * (j / masse);
	vitesse = vitesse - temp;
	temp = dir * (j / others->getMasse());
	others->setVitesse(others->getVitesse() + temp);
	return;
}



/// <summary>
///
/// </summary>
/// <param name="time"></param>
void particule::EulerIntegration(float time, float width, float height) {
	vect deltaVitesse = acceleration * time;
	vitesse = vitesse + deltaVitesse;
	vect deplacement = vitesse * time;

	/* ######## FOR DEBUG ########
	* 
	printf("acc (y) = %f\n", acceleration.y);
	printf("vit (y) = %f\n\n", vitesse.y);
	printf("deplacement (y) = %f\n\n", deplacement.y);

	printf("acc (x) = %f\n", acceleration.x);
	printf("vit (x) = %f\n", vitesse.x);
	printf("deplacement (x) = %f\n\n", deplacement.x);

	printf("angle : %f\n", std::atanf(vitesse.y / vitesse.x) * 180 / std::numbers::pi);

	printf("------------------------\n");
	*/

	// Invert the vertical velocity component, since the rendering coordinate system is inverted
	// 0 --> top of the screen
	deplacement.y *= (-1);

	// New position = previous position + movement
	if ((position + deplacement).x < 0) {
		position.x = 0;
		vitesse.x *= (-1/2);
	} else if ((position + deplacement).x > width) {
		position.x = width;
		vitesse.x *= (-1/2);
	}

	if ((position + deplacement).y < 0) {
		position.y = 0;
		vitesse.y *= (-1/2);
	} else if ((position + deplacement).y > height) {
		position.y = height;
		vitesse.y *= (-1/2);
	}
	position = position + deplacement;
}

/// <summary>
/// In development
/// </summary>
/// <param name="time"></param>
void particule::VerletIntegration(float time) {
	vect previous_pos = position;
	vect temp = acceleration * time * time;
	position = position * 2 - previous_pos + temp;
}

/// <summary>
/// Function to modify the mass of the particle
/// </summary>
///
/// <param name="m">masse de la particule</param>
void particule::setMasse(float m) {
	masse = m;
}

void particule::setVitesse(vect vit) {
	vitesse = vit;
}

/// <summary>
/// Function to modify the gravity applied to the particle
/// </summary>
/// <param name="g"></param>
void particule::setGravity(float g) {
	gravity = g;
}

void particule::setDamping(float f) {
	damping = f;
}

float particule::getMasse() {
	return masse;
}

float particule::getGravity() {
	return gravity;
}

float particule::getDamping() {
	return damping;
}

/// <summary>
/// Function which return an array with all the positions all the previous positions of the particle
/// </summary>
/// <returns>
/// Vector<Vect> storing the past positions
/// </returns>
std::vector<vect> particule::getTraceArray() {
	return trace;
}
void particule::addTracePosition(vect pos) {
	trace.push_back(pos);
}

/// <summary>
/// Function that plots a line through all the past positions
/// </summary>
void particule::drawTrace() {
	if (trace.size() >= 2) {
		for (int i = 1; i < trace.size(); i++) {
			ofDrawLine(glm::vec3(trace[i - 1].x, trace[i - 1].y, trace[i - 1].z), glm::vec3(trace[i].x, trace[i].y, trace[i].z));
		}
	}
}

/// <summary>
/// Allows drawing the trail of an object by specifying the trail color
/// </summary>
/// <param name="color">
/// Trail color in the format: ofColor(R,G,B)
/// </param>
void particule::drawTrace(ofColor color) {
	ofSetColor(color);
	drawTrace();
	ofSetColor(ofColor(255, 255, 255));
}

void ParticuleForceGenerator::updateForce(particule * p, float duration) {
	
}

void ParticuleForceRegistry::add(particule * p, ParticuleForceGenerator * fg) {
	ParticuleForceRegister newRegister = { p, fg };
	m_Register.push_back(newRegister);
}

void ParticuleForceRegistry::remove(particule * p, ParticuleForceGenerator * fg) {
	for (int i = 0; i < m_Register.size(); i++) {
		if (m_Register[i].p == p && m_Register[i].fg == fg) {
			m_Register.erase(m_Register.begin() + i);
			return;
		}
	}
}
bool ParticuleForceRegistry::inside(particule* p) {
	for (auto& force : m_Register) {
		if (force.p->GetID() == p->GetID()) {
			return true;
		}
	}
	return false;
}

void ParticuleForceRegistry::erase() {
	m_Register.clear();
}

void ParticuleForceRegistry::clear() {
	for (ParticuleForceRegister registre : m_Register) {
		registre.p->clearForce();
		if (dynamic_cast<ressort *>(registre.fg)) {
			ressort * r = dynamic_cast<ressort *>(registre.fg);
			if (r->base != nullptr) {
				r->base->clearForce();
			}
		}
	}
}

void ParticuleForceRegistry::updateForces(float duration) {
	clear();
	for (ParticuleForceRegister registre : m_Register) {
		registre.fg->updateForce(registre.p, duration);
	}
}

particuleGravity::particuleGravity() {
	gravity = { 0, -9.81, 0 };
}

particuleGravity::particuleGravity(vect value) {
	gravity = value;
}

void particuleGravity::updateForce(particule* p, float duration) {
	if (p->getMasse() != 0) {
		p->addForce(gravity * p->getMasse());
	}
}
