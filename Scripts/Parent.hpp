#pragma once

#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include "Chick.hpp"

enum class Sex { male, female };
enum class Bout { shortBout, longBout };

/*
	A breeding adult Leach's Storm-petrel parent, flying back and forth from
	the foraging ground to the breeding ground
*/
class Parent {

public:

	// Constructor
	Parent(Sex, std::mt19937*,
		   Chick*,
		   int, int,
		   int, int,
		   Bout);

	void parentDay();

	// Setters
	void setShortMetabolism(double metabolism) { this->shortForageMetabolism = metabolism; }
	void setLongMetabolism(double metabolism) { this->longForageMetabolism = metabolism; }
	
	// Getters
	Sex getSex() { return this->sex; }
	bool isAlive() { return this->alive; }
	double getEnergy() { return this->energy; }
	std::string getBoutState();

	int getNumShortBouts() { return numShortBouts; }
	int getShortBoutLength() { return shortBoutLength; }
	double getShortMetabolism() { return shortForageMetabolism; }
	double getShortForageMean() { return shortForageMean; }
	double getShortForageSD() { return shortForageSD; }

	int getNumLongBouts() { return numLongBouts; }
	int getLongBoutLength() { return longBoutLength; }
	double getLongMetabolism() { return longForageMetabolism; }
	double getLongForageMean() { return longForageMean; }
	double getLongForageSD() { return longForageSD; }

	std::vector<double> getEnergyRecord() { return this->energyRecord; }

private:

	constexpr static double START_ENERGY = 10;

	constexpr static double SHORT_FORAGE_METABOLISM = 10;
	constexpr static double SHORT_FORAGE_MEAN = 100;
	constexpr static double SHORT_FORAGE_SD = 15;

	constexpr static double LONG_FORAGE_METABOLISM = 10;
	constexpr static double LONG_FORAGE_MEAN = 100;
	constexpr static double LONG_FORAGE_SD = 15;

	void shortBout();
	void longBout();
	double calcMealSize();

	std::string getBoutStr();

	Sex sex;
	bool alive;
	
	std::mt19937* randGen;
	Bout boutState;

	double energy;

	Chick* chick;

	double mealSize;
	int dayCounter;
	int boutCounter;

	double shortForageMetabolism;
	double shortForageMean;
	double shortForageSD;
	std::normal_distribution<double> shortForageIntake;

	double longForageMetabolism;
	double longForageMean;
	double longForageSD;
	std::normal_distribution<double> longForageIntake;

	int numShortBouts;
	int shortBoutLength;

	int numLongBouts;
	int longBoutLength;

	std::vector<double> energyRecord;
};