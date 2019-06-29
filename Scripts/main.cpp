#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <unistd.h>

#include "Util.hpp"
#include "Chick.hpp"
#include "Parent.hpp"

// TODO Make key global constants cmd line input parameters

// The number of iterations for each model or parameter set of a given model
constexpr static int ITERATIONS = 1000;

// Unique (will overwrite) output file for each model run,
// Sent to runModel()
constexpr static char OUTPUT_FNAME[] = "sims.txt";


// Need a single, static random generator device to let us only seed once
static std::mt19937* randGen;

// Prototypes -- see functions for documentation
void runModel(int, 
	          std::string);

void chickSeason(Parent&, Parent&, Chick&);

/*
Called when program is run.
Calls each individual model with runModel(), which 
takes the address of the correct behavioral breeding season model
as its second parameter.
Make sure to send the correct OUTPUT_FNAME to each runModel() call!
*/
int main()
{
	// Record model timing to help me know which computer is the fanciest
	auto startTime = std::chrono::system_clock::now();

	// Seed static random generator device with ridiculous C++11 things
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 r = std::mt19937(seed);
	randGen = &r;

	std::cout << "\n\n\nBeginning model runs\n\n\n";

	// Standard breeding season
	runModel(ITERATIONS, 
			 OUTPUT_FNAME);

	// Report output and exit
	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> runTime = endTime - startTime;

	// Congrats you survived! I hope the storm-petrels did too.
	std::cout << "All model output written" 
		  	  << std::endl
		      << "Runtime in "
		      << runTime.count() << " s."
	  	      << std::endl;
	return 0;
}

/*
Master function to call each model and write formatted output
@param iterations the number of breeding season replicates
@param outfileName file to write output
*/
void runModel(int iterations, 
	      	  std::string outfileName)
{
	// Start formatted output
	std::ofstream outfile;
	outfile.open("../Output/" + outfileName, std::ofstream::trunc);

	// Header column for CSV format
	outfile << "iterations" << ","
			<< "numLivingChicks" << ","
		    << "shortForageMetabolism" << ","
		    << "shortForageMean" << ","
		    << "shortForageSD" << ","
		    << "numShortBouts_F" << ","
		    << "shortBoutLength_F" << ","
		    << "numShortBouts_M" << ","
		    << "shortBoutLength_M" << ","
		    << "longForageMetabolism" << ","
		    << "longForageMean" << ","
		    << "longForageSD" << ","
		    << "numLongBouts_F" << ","
		    << "longBoutLength_F" << ","
		    << "numLongBouts_M" << ","
		    << "longBoutLength_M" << ","
		    << "endEnergy_F" << ","
		    << "meanEnergy_F" << ","
		    << "varEnergy_F" << ","
		    << "endEnergy_M" << ","
		    << "meanEnergy_M" << ","
		    << "varEnergy_M" << ","
		    << "meanChickMass" << ","
		    << "varChickMass" << ","
		    << "meanChickTarsus" << ","
		    << "varChickTarsus" << ","
		    << "meanChickWing" << ","
		    << "varChickWing" << std::endl;

	/*
	Initialize output objects to store records from each parameter combo iteration
	At the end of each param combination, summarize rates or mean values
	and only output one line for each combo. 
	NOTE one cannot keep a vector in active memory that doesn't have 
		 summary values because you'll run out of memory, but you
		 can print output for each iteration to a file if you have the 
		 drive space. 
	*/

	// Female energy parameters to track
	std::vector<double> endEnergy_F  = std::vector<double>();
	std::vector<double> meanEnergy_F = std::vector<double>();
	std::vector<double> varEnergy_F  = std::vector<double>();

	// Male energy parameters to track
	std::vector<double> endEnergy_M  = std::vector<double>();
	std::vector<double> meanEnergy_M = std::vector<double>();
	std::vector<double> varEnergy_M  = std::vector<double>();

	// Chick parameters to track
	std::vector<double> chickMass = std::vector<double>();
	std::vector<double> chickTarsus = std::vector<double>();
	std::vector<double> chickWing = std::vector<double>();

	int numLivingChicks = 0;

	int numShortBouts_F = 1;
	int shortBoutLength_F = 1;

	int numShortBouts_M = 1;
	int shortBoutLength_M = 1;

	int numLongBouts_F = 1;
	int longBoutLength_F = 1;

	int numLongBouts_M = 1;
	int longBoutLength_M = 1;

	for (int i = 0; i < iterations; i++) {

		if (i % iterations / 10 == 0) {
			std::cout << "On iteration " << i
					  << "of " << iterations << std::endl;
		}

		Chick chick = Chick();

		Bout boutState_F = Bout::shortBout;
		Bout boutState_M = Bout::shortBout;

		if (rand() / RAND_MAX < 0.5) {
			boutState_F = Bout::longBout;
		} else {
			boutState_M = Bout::longBout;
		}

		Parent pf = Parent(Sex::female, randGen,
						   &chick,
						   numShortBouts_F, shortBoutLength_F,
						   numLongBouts_F, longBoutLength_F,
						   boutState_F);

		Parent pm = Parent(Sex::male, randGen,
						   &chick,
						   numShortBouts_M, shortBoutLength_M,
						   numLongBouts_M, longBoutLength_M,
						   boutState_M);

		chickSeason(pf, pm, chick);

		if (chick.isAlive()) {
			numLivingChicks++;
		}

		std::vector<double> energy_F = pf.getEnergyRecord();
		endEnergy_F.push_back(energy_F.back());
		meanEnergy_F.push_back(vectorMean(energy_F));
		varEnergy_F.push_back(vectorVar(energy_F));

		std::vector<double> energy_M = pm.getEnergyRecord();
		endEnergy_M.push_back(energy_M.back());
		meanEnergy_M.push_back(vectorMean(energy_M));
		varEnergy_M.push_back(vectorVar(energy_M));

		chickMass.push_back(chick.getMass());
		chickTarsus.push_back(chick.getTarsusLength());
		chickWing.push_back(chick.getWingLength());

	}


	// TODO VARY METABOLISM, MEAN, SD
	Chick sampleChick = Chick();

	Parent sampleParent = Parent(Sex::female, randGen,
								 &sampleChick,
								 numShortBouts_F, shortBoutLength_F,
								 numLongBouts_F, longBoutLength_F,
								 Bout::shortBout);

	double shortForageMetabolism = sampleParent.getShortMetabolism();
	double shortForageMean = sampleParent.getShortForageMean();
	double shortForageSD = sampleParent.getShortForageSD();

	double longForageMetabolism = sampleParent.getLongMetabolism();
	double longForageMean = sampleParent.getLongForageMean();
	double longForageSD = sampleParent.getLongForageSD();

	double mean_endEnergy_F = vectorMean(endEnergy_F);
	double mean_meanEnergy_F = vectorMean(meanEnergy_F);
	double mean_varEnergy_F = vectorVar(varEnergy_F);

	double mean_endEnergy_M = vectorMean(endEnergy_M);
	double mean_meanEnergy_M = vectorMean(meanEnergy_M);
	double mean_varEnergy_M = vectorVar(varEnergy_M);
	
	double mean_chickMass = vectorMean(chickMass);
	double var_chickMass = vectorVar(chickMass);

	double mean_chickTarsus = vectorMean(chickTarsus);
	double var_chickTarsus = vectorVar(chickTarsus);

	double mean_chickWing = vectorMean(chickWing);
	double var_chickWing = vectorVar(chickWing);

	outfile << iterations << ","
			<< numLivingChicks << ","
			<< shortForageMetabolism << ","
			<< shortForageMean << ","
			<< shortForageSD << ","
			<< numShortBouts_F << ","
			<< shortBoutLength_F << ","
			<< numShortBouts_M << ","
			<< shortBoutLength_M << ","
			<< longForageMetabolism << ","
			<< longForageMean << ","
			<< longForageSD << ","
			<< numLongBouts_F << ","
			<< longBoutLength_F << ","
			<< numLongBouts_M << ","
			<< longBoutLength_M << ","
			<< mean_endEnergy_F << ","
			<< mean_meanEnergy_F << ","
			<< mean_varEnergy_F << ","
			<< mean_endEnergy_M << ","
			<< mean_meanEnergy_M << ","
			<< mean_varEnergy_M << ","
			<< mean_chickMass << ","
			<< var_chickMass << ","
			<< mean_chickTarsus << ","
			<< var_chickTarsus << ","
			<< mean_chickWing << ","
			<< var_chickWing << std::endl;

	// Clear all output storage vectors for next param combo
	numLivingChicks = 0;
	endEnergy_F.clear();
	meanEnergy_F.clear();
	varEnergy_F.clear();

	endEnergy_M.clear();
	meanEnergy_M.clear();
	varEnergy_M.clear();

	chickMass.clear();
	chickTarsus.clear();
	chickWing.clear();

	// Close file and exit
	outfile.close();
	std::cout << "Final output written to " << outfileName << "\n";
}

void chickSeason(Parent& pf, Parent& pm, Chick& chick) 
{
	while(chick.isAlive() && !chick.isFledged()) {
		pf.parentDay();
		pm.parentDay();
		chick.chickDay();
	}
}

