#include "Util.hpp"

std::vector<double> paramVector(double p[3]) 
{
	double min = p[0];
	double max = p[1];
	double by = p[2];

	std::vector<double> ret;
	for (double i = min; i <= max; i+=by) {
		ret.push_back(i);
	}

	return ret;
}

double vectorMean(std::vector<double>& v)
{
	int items = v.size();
	double sum = std::accumulate(v.begin(), v.end(), 0.0);

	return sum / items;
}

double vectorMean(std::vector<int>& v) 
{
	int items = v.size();
	double sum = std::accumulate(v.begin(), v.end(), 0.0);

	return sum / items;
}

double vectorVar(std::vector<double>& v)
{
	int items = v.size();
	double mean = vectorMean(v);
	double sqSum = 0.0;

	for (int i = 0; i < items; i++) {
		sqSum += pow(v[i]-mean, 2);
	}


	return sqSum / items;
}

double vectorVar(std::vector<int>& v)
{
	int items = v.size();
	double mean = vectorMean(v);
	double sqSum = 0.0;

	for (int i = 0; i < items; i++) {
		sqSum += pow(v[i]-mean, 2);
	}

	return sqSum / items;
}