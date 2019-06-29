#pragma once

class Chick {
	
public:

	// Constructor
	Chick();					

	void chickDay();
	void provision(double meal) { this->energy += meal; }						

	// Getters
	bool isAlive() { return this->alive; }
	bool isFledged() { return this->fledged; }
	int getAge() { return this->age; }

	double getEnergy() { return this->energy; }
	double getMass() { return this->mass; }
	double getTarsusLength() { return this->tarsusLength; }
	double getWingLength() { return this->wingLength; }

private:

	// NEED REAL PARAMETERS!
	constexpr static int FLEDGE_DAY = 51;
	constexpr static double METABOLISM = 10;

	void grow();
	
	bool alive;
	bool fledged;

	int age;

	double energy;
	double mass;
	double tarsusLength;
	double wingLength;
};