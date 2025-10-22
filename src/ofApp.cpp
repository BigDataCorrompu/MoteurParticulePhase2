#include "ofApp.h"
#include "..//testVector.h";
#include <numbers>
#include <vector>

std::vector<particule> particuleArray;
std::vector<friction> FrictionArray;
particuleGravity g;

float angle = 30;
float duration = 0;

float spawnTimer = 0.0f;
float spawnDelay = 5.0f;

ressort r = ressort(0.6f, 20.0f);
ressort r2 = ressort(0.6f, 20.0f);

ressort ressortBlob(0.60f, 0.0f);

ressort r3 = ressort(0.6, 20);

ParticuleForceRegistry registreForces;
std::vector<Blob> blobArray;

//On réserve un tableau de 256 ressorts au cas ou
std::vector<ressort> r_array(256);
Blob blob;

//--------------------------------------------------------------
void ofApp::setup() {
	particuleArray.reserve(1000);
	FrictionArray.reserve(1000);
	ofSetVerticalSync(true);
	ofBackground(0, 0, 0, 0);

	TestVector::runTests();
	
	particule p1 = particule({ 500, 100, 0 });
	p1.setMasse(1.0f);
	r.setPos({ 500, 250, 0 });
	
	particuleArray.push_back(p1);

	friction f = friction(0.1f);
	FrictionArray.push_back(f);

	//Attention a ne pas mettre directement le pointeurs dans le registre des forces, p1 disparait à la fin de la boucle setup
	//Alors que le tableau particuleArray contient une copie persistente de la particule
	registreForces.add(&particuleArray[0], &g);
	registreForces.add(&particuleArray[0], &r);
	registreForces.add(&particuleArray[0], &FrictionArray[0]);

	particule p2 = particule({ 400, 100, 0 });
	p2.setMasse(1.0f);
	r2.setPos({ 400, 250, 0 });
	particuleArray.push_back(p2);
	
	registreForces.add(&particuleArray[1], &g);
	registreForces.add(&particuleArray[1], &r2);

	particule p_left({ 10, 300, 0 }, { 30, 0, 0 });
	particule p_right({ 150, 300, 0 }, { -10, 0, 0 });

	particuleArray.push_back(p_left);
	particuleArray.push_back(p_right);

	particule playerBlobparticule = particule({ 100, 500, 0 });
	playerBlobparticule.setColor(ofColor(127, 127, 0));
	particuleArray.push_back(playerBlobparticule);


	blob.SetParticuleMaster(&particuleArray[particuleArray.size()-1]);
	blob.setRegisterForce(&registreForces);
	blob.setRessort(&ressortBlob);

	ressortBlob.UseMaxLength = true;
	ressortBlob.SetMaxLength(50.0f);

	particule p12({ 100, 400, 0 }, { -10, 0, 0 });
	particule p13({ 120, 400, 0 }, { 10, 0, 0 });

	particuleArray.push_back(p12);
	particuleArray.push_back(p13);
	r3.ActiveMaxLength(true);
	r3.SetMaxLength(150);
	r3.setBase(&particuleArray[particuleArray.size() - 2]);
	ressortBlob.setBase(blob.getHeadBlob());
	registreForces.add(&particuleArray[particuleArray.size() - 1], &r3);

}

//--------------------------------------------------------------
void ofApp::update() {
	float time = 1.0f / 60.0f;
	for (auto& particule : particuleArray) {
		particule.clearForce();
	}
	registreForces.updateForces(time);
	for (auto & part : particuleArray) {
		for (int i = 0; i < particuleArray.size(); i++) {
			if (part.GetID() != particuleArray[i].GetID() && part.checkCollision(particuleArray[i])) {
				part.SolveCollision(&particuleArray[i]);
			}
		}
		part.computeAcceleration();
		part.EulerIntegration(time, ofGetWidth(), ofGetHeight());
		part.addTracePosition(part.getPosition());
	}

	for (auto & part : particuleArray) {
		//On vérifie que la particule n'appartient pas deja au blob
		if (!blob.inside(part)) {
			//On vérifie que la particule ne se trouve pas dans un autre blob en attente
			if (blobArray.size() == 0) {
				if (blob.checkCollision(part)) {
					//Dans ce cas on peut l'ajouter au blob player
					blob.addParticule(&part);
					registreForces.add(&part, &ressortBlob);
				}
			} else {
				for (int i = 0; i < blobArray.size(); i++) {
					if (!blobArray[i].inside(part) && blob.checkCollision(part)) {
						blob.addParticule(&part);
						registreForces.add(&part, &ressortBlob);
					}
				}
			}
		}
	}
	if (spawnTimer > spawnDelay) {
		spawnTimer = 0.0f;
		particule newparticule = particule({ ofRandom(400), ofRandom(600) - 100, 0 });
		particuleArray.push_back(newparticule);
	} else {
		spawnTimer += 1 / ofGetFrameRate();
	}
	}
//--------------------------------------------------------------
void ofApp::draw() {
	for (auto & part : particuleArray) {
		part.draw();
		//part.drawTrace(ofColor(127, 0, 0));	
	}

	for (auto& part : registreForces.m_Register) {
		if (dynamic_cast<ressort*>(part.fg)) {
			ressort * r = dynamic_cast<ressort *>(part.fg);
			ofDrawLine(glm::vec3(r->getPos().x, r->getPos().y, r->getPos().z), glm::vec3(part.p->getPosition().x, part.p->getPosition().y, part.p->getPosition().z));
		}
	}

	ofDrawBitmapStringHighlight("800 metres", 930, 600);
	ofDrawArrow(glm::vec3(100, 600, 0), glm::vec3(900, 600, 0), 2);

	duration += 1 / ofGetFrameRate();
	char msgTime[128];
	sprintf(msgTime, "Durée de la simulation : %.2f (s)", duration);
	ofDrawBitmapStringHighlight(msgTime, 100, 100);

	char msgAngle[128];
	sprintf(msgAngle, "Angle : %.f °", angle);
	ofDrawBitmapStringHighlight(msgAngle, 100, 120);

	char cmd[256];
	sprintf(cmd, "Commandes :\n\nLancer boule de feu : 'f'\nLancer balle : 'b'\nTirer laser : 'l'\nLancer boule de canon : 'c'\n\nModifier l'angle de tir : flèche directionnelle up/dwn\n\nReset l'affichage : backspace");
	ofDrawBitmapStringHighlight(cmd, 600, 100);

	char cmdBlob[256];
	sprintf(cmdBlob, "Commande Blob : 'z', 'q', 's', 'd' pour le déplacer\nSplit le Blob : 'k'\nFusion du Blob : 'm'\n");
	ofDrawBitmapStringHighlight(cmdBlob, 600, 400);

	char nbrBlob[256];
	sprintf(nbrBlob, "Nombre de particule dans le blob principal : %d", blob.getNumparticule());
	ofDrawBitmapStringHighlight(nbrBlob, 600, 450);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 112) {
		particule p1({ ofRandom(ofGetWindowSize().x), ofRandom(ofGetWindowSize().y), 0 }, { ofRandom(100), ofRandom(100), 0 });
		p1.setDamping(0.01);
		p1.setMasse(12);
		p1.setGravity(-9.81);
		particuleArray.push_back(p1);
	}
	if (key == 'c') {
		float speed = 150; // vitesse initiale de 150m/s
		particule canon({ 100, 550, 0 }, { speed * std::cosf(angle / 360 * 2 * std::numbers::pi), speed * std::sinf(angle / 360 * 2 * std::numbers::pi), 0 }, { 0, 0, 0 });
		canon.setDamping(0.05);
		canon.setMasse(12);
		canon.setGravity(-9.81);
		particuleArray.push_back(canon);
		friction f = friction(canon.getDamping());
		FrictionArray.push_back(f);
		registreForces.add(&particuleArray[particuleArray.size() - 1], &g);
		registreForces.add(&particuleArray[particuleArray.size() - 1], &FrictionArray[FrictionArray.size() - 1]);
	}

	// send a laser beam
	if (key == 'l') {
		float speed = 5000;
		particule laser({ 100, 550, 0 }, { speed * std::cosf(angle / 360 * 2 * std::numbers::pi), speed * std::sinf(angle / 360 * 2 * std::numbers::pi), 0 }, { 0, 0, 0 });
		laser.setDamping(0.0f);
		laser.setMasse(0.0f);
		laser.setGravity(-9.81);
		particuleArray.push_back(laser);
	}

	if (key == 'f') {
		float speed = 50;
		particule fireball({ 100, 550, 0 }, { speed * std::cosf(angle / 360 * 2 * std::numbers::pi), speed * std::sinf(angle / 360 * 2 * std::numbers::pi), 0 }, { 0, 0, 0 });
		fireball.setDamping(0.0f);
		fireball.setMasse(0.0f);
		fireball.setGravity(0);
		particuleArray.push_back(fireball);
	}

	if (key == 'b') {
		float speed = 500;
		particule bullet({ 100, 550, 0 }, { speed * std::cosf(angle / 360 * 2 * std::numbers::pi), speed * std::sinf(angle / 360 * 2 * std::numbers::pi), 0 }, { 0, 0, 0 });
		bullet.setDamping(0.0001f);
		bullet.setMasse(0.01f);
		bullet.setGravity(-9.81f);
		particuleArray.push_back(bullet);
		friction f = friction(bullet.getDamping());
		FrictionArray.push_back(f);
		registreForces.add(&particuleArray[particuleArray.size() - 1], &g);
		registreForces.add(&particuleArray[particuleArray.size() - 1], &FrictionArray[FrictionArray.size() - 1]);
	}

	if (key == 'k') {
		// On crée directement le ressort dans le vecteur
		r_array.emplace_back(0.6f, 10.0f);
		ressort & r = r_array.back();

		// On crée directement le blob dans le tableau
		blobArray.emplace_back();
		Blob & newBlob = blobArray.back();

		// On tente de splitter le blob
		if (!blob.splitBlob(&registreForces, &r, &newBlob)) {
			// Si échec, on supprime le blob et le ressort ajoutés
			blobArray.pop_back();
			r_array.pop_back();
		}
	}

	if (key == 'm') {
		//check si un autre blob se trouve a proximité
		for (int i = 0 ; i<blobArray.size(); i++) {
			if (blobArray[i].getHeadBlob()->getDistance(*blob.getHeadBlob()) < 50)
			{
				//On merge et on supprime l'ancien blob
				blob.mergeBlob(&registreForces, &blobArray[i], &ressortBlob);
				blobArray.erase(blobArray.begin() + i);
			}
		}
	}

	if (key == 8) {
		particuleArray.clear();
		registreForces.erase();
		blob.clear();
		particule playerBlobparticule = particule({ 100, 500, 0 });
		
		particuleArray.push_back(playerBlobparticule);
		ressortBlob.setBase(&particuleArray[0]);
		blob.setRessort(&ressortBlob);
		blob.SetParticuleMaster(&particuleArray[0]);
		blob.setRegisterForce(&registreForces);
	}
	// Arrow down
	if (key == 57359) {
		angle -= 1;
	}

	// Arrow up
	if (key == 57357) {
		angle += 1;
	}

	if (key == 'z') {
		blob.MoveBlob(up);
	}
	if (key == 's') {
		blob.MoveBlob(down);
	}
	if (key == 'q') {
		blob.MoveBlob(left);
	}
	if (key == 'd') {
		blob.MoveBlob(right);
	}

	if (key == 'f') {
		//split the blob
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
