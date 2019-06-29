#pragma once

#include <vector>
#include <numeric>
#include <cmath>

// Return sequence vector from {min, max, by} array
std::vector<double> paramVector(double p[3]);

// Returns mean of vector contents
double vectorMean(std::vector<double>&);
double vectorMean(std::vector<int>&);		// overloaded

// Returns variance of vector contents
double vectorVar(std::vector<double>&);
double vectorVar(std::vector<int>&);		// overloaded