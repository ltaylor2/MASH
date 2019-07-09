#include "Parent.hpp"

Parent::Parent(Sex sex_, std::mt19937* randGen_,
	       Chick* chick_, 
	       int numShortBouts_ , int shortBoutLength_,
	       int numLongBouts_, int longBoutLength_, 
	       Bout boutState_):
	sex(sex_),
	alive(true),
	randGen(randGen_),
	boutState(boutState_),
	energy(START_ENERGY),
	chick(chick_),
	mealSize(0),
	dayCounter(0),
	boutCounter(0),
	shortForageMetabolism(SHORT_FORAGE_METABOLISM),
	shortForageMean(SHORT_FORAGE_MEAN),
	shortForageSD(SHORT_FORAGE_SD),
	shortForageIntake(std::normal_distribution<double>(shortForageMean,
							   shortForageSD)),
	longForageMetabolism(LONG_FORAGE_METABOLISM),
	longForageMean(LONG_FORAGE_MEAN),
	longForageSD(LONG_FORAGE_SD),
	longForageIntake(std::normal_distribution<double>(longForageMean,
							  longForageSD)),
	numShortBouts(numShortBouts_),
	shortBoutLength(shortBoutLength_),
	numLongBouts(numLongBouts_),
	longBoutLength(longBoutLength_),
	energyRecord(std::vector<double>())
{}

void Parent::parentDay()
{
	if (this->boutState == Bout::shortBout) {
		this->shortBout();

		if (this->dayCounter == this->shortBoutLength) {
			chick->provision(mealSize);

			this->dayCounter = 0;
			this->boutCounter++;

			if (this->boutCounter == this->numShortBouts) {
				this->boutState = Bout::longBout;
				this->boutCounter = 0;
			}
		}
	} else if (this->boutState == Bout::longBout) {
		this->longBout();

		if (this->dayCounter == this->longBoutLength) {
			chick->provision(mealSize);

			this->dayCounter = 0;
			this->boutCounter++;

			if (this->boutCounter == this->numLongBouts) {
				this->boutState = Bout::shortBout;
				this->boutCounter = 0;
			}
		}
	}

	this->energyRecord.push_back(energy);
}

void Parent::shortBout() 
{
	this->energy -= shortForageMetabolism;

	double intake = this->shortForageIntake(randGen);
	this->energy += intake;

	this->dayCounter++;
}

void Parent::longBout() 
{
	this->energy -= longForageMetabolism;

	double intake = this->longForageIntake(randGen);
	this->energy += intake;

	this->dayCounter++;
}

// TODO calculate meal size from available energy
double calcMealSize() 
{
	return 0;
}

std::string Parent::getBoutState() {
	return this->getBoutStr();
}

std::string Parent::getBoutStr() 
{
	std::string s = "";
	if (this->boutState == Bout::shortBout) {
		s = "short";
	} else if (this->boutState == Bout::longBout) {
		s = "long";
	}

	return s;
}