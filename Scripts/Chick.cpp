#include "Chick.hpp"

// Constructor
Chick::Chick():
	alive(true),
	fledged(false),
	age(0),
	energy(0),
	mass(0.0),
	tarsusLength(0.0),
	wingLength(0.0),
{}


void Chick::chickDay() {

	this->grow();

	this->age++;

	if (this->energy < 0) {
		this->alive = false;
	}
	
	if (this->alive && this->age >= FLEDGE_DAY) {
		this->fledged = true;
	}

}

void Chick::grow() {

	this->energy -= METABOLISM;

	// TODO GROWTH FUNCTIONS FOR MASS, TARSUS, WING, as function of energy
	// this->tarsus += ....
}