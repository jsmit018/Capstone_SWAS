#include "Distribution.h"
/**
 * Distribution Constructor
 */
Distribution::Distribution() {}

/**
 * Returns the string System seedType
 */
string Distribution::GetSystemSeedType()
{
	return _seedType;
}

/**
 * Initalizes seedType and systemSeed
 */
string Distribution::_seedType = "null";
int Distribution::_systemSeed = 0;

/**
 * Skeleton Print Function for Distribution
 */
void Distribution::PrintDistribution()
{
}

/**
 * Initializes the Random Number Engine
 */
std::default_random_engine Distribution::generator;

/**
 * Sets the sytem seed based off of a given seed parameter.
 * Compares the parameter seed to the current seed. If they are the same it does nothing, otherwise 
 * it updates the system seed to a new value
 */
void Distribution::SetSystemSeed(int seed) {
	int sysSeed = IsSystemSeedTypeSameorDifferent(seed);
	if (sysSeed == 0) {
		cout << "Seed was the same" << endl;
	}
	else if (sysSeed == 1)
		cout << "System Seed was different, system seed updated" << endl;
}

/**
 * Function that sets the system seedtype
 */
void Distribution::SetSystemSeedType(string seedType) {
	_seedType = seedType;
}

/**
 * Compares the current seed to a seed parameter
 */
int Distribution::IsSystemSeedTypeSameorDifferent(int seed)
{
	if (_systemSeed == seed) {
		return 0;
	}
	else {
		_systemSeed = seed;
		generator.seed(_systemSeed);
		return 1;
	}
}

/**
 * Function that returns systemSeed
 */
int Distribution::GetSystemSeed()
{
	return _systemSeed;
}

/**
 * Child class for Exponential Distribution
 */
Exponential::Exponential(double mean) : Distribution()
{
	_mean = mean;
	_distr = new std::exponential_distribution<double>(1.0 / mean);
}

/**
 * Returns a random number from the Exponential Distribution curve
 */
double Exponential::GetRV()
{
	return _distr->operator()(generator);
}

/**
 * Prints the distribution values set into Exponential
 */
void Exponential::PrintDistribution()
{
	printf("Exponential - mean: %f \n", _mean);
}

/**
 * Returns Exponential mean value
 */
double Exponential::GetMean()
{
	return _mean;
}

/**
 * Returns the Exponential Distribution
 */
std::exponential_distribution<double>* Exponential::GetDistr()
{
	return _distr;
}

/**
 * Copies Exponential Distribution
 */
Distribution* Exponential::CopyThis()
{
	Exponential* newptr = new Exponential(_mean);
	return newptr;
}

/**
 * Constructor for Child Class Uniform Distribution
 */
Uniform::Uniform(double min, double max) : Distribution()
{
	_min = min;
	_max = max;
	_distr = new std::uniform_real_distribution<double>(min, max);
}

/**
 * Gets a random number from the Uniform Distribution Curve
 */
double Uniform::GetRV()
{
	return _distr->operator()(generator);
}

/**
 * Prints Uniform Distribution values: Min, Max
 */
void Uniform::PrintDistribution()
{
	printf("Uniform - min, max: %f %f \n", _min, _max);

}

/**
 * Copies Uniform Distribution
 */
Distribution* Uniform::CopyThis()
{
	Uniform* newptr = new Uniform(_min, _max);
	return newptr;
}

/**
 * Constructor for Child class Triangular Distribution
 */
Triangular::Triangular(double min, double expected, double max) : Distribution()
{
	_min = min;
	_expected = expected;
	_max = max;
	_distr = new std::uniform_real_distribution<double>(0.0, 1.0);
	a = min;
	c = expected;
	b = max;
	fc = (c - a) / (b - a);
	term1 = (b - a) * (c - a);
	term2 = (b - a) * (b - c);
}

/**
 * Returns the Triangular Distribution object
 */
std::uniform_real_distribution<double>* Triangular::GetDistr()
{
	return _distr;
}

/**
 * Returns a random number from the Triangular distribution curve
 */
double Triangular::GetRV()
{
	double u = _distr->operator()(generator);
	double x;
	if (u < fc)
		x = a + sqrt(u * term1);
	else
		x = b - sqrt((1 - u) * term2);
	return x;
}

/**
 * Copies Triangular Distribution object
 */
Distribution* Triangular::CopyThis()
{
	Triangular* newptr = new Triangular(_min, _expected, _max);
	return newptr;
}

/**
 * Prints the Triangular distribution values: Min, Expected, Max
 */
void Triangular::PrintDistribution()
{
	printf("Triangular - min, expected, max: %f %f %f \n", _min, _expected, _max);
}

/**
 * Constructor for child class Normal Distribution
 */
Normal::Normal(double mean, double stdev)
{
	_mean = mean;
	_stdev = stdev;
	_distr = new std::normal_distribution<double>(mean, stdev);
}

/**
 * Copies Normal Distribution object and returns it
 */
Distribution* Normal::CopyThis()
{
	Normal* newptr = new Normal(_mean, _stdev);
	return newptr;
}

/**
 * Returns a random number from the Normal distribution curve
 */
double Normal::GetRV()
{
	return _distr->operator()(generator);
}

/**
 * Prints the values of the Normal Distribution: mean, Standard Deviation
 */
void Normal::PrintDistribution()
{
	printf("Normal - mean, stdev: %f %f\n", _mean, _stdev);
}

/**
 * Constructor for child class Poisson Distribution
 */
Poisson::Poisson(double mean)
{
	_mean = mean;
	_distr = new std::poisson_distribution<int>(mean);
}

/**
 * Returns Poisson distribution object
 */
std::poisson_distribution<int>* Poisson::GetDistr()
{
	return _distr;
}

/**
 *  Returns a random number from the Poisson distribution curve
 */
double Poisson::GetRV()
{
	return _distr->operator()(generator);
}

/**
 * Copies Poisson distribution object
 */
Distribution* Poisson::CopyThis()
{
	Poisson* newptr = new Poisson(_mean);
	return newptr;
}

/**
 * Prints the mean value from the Poisson object
 */
void Poisson::PrintDistribution()
{
	printf("Poisson - mean: %f\n", _mean);
}

/**
 * Constan Distribution Constructor
 */
Constant::Constant(double mean)
{
	_mean = mean;
}

/**
 * Returns mean value of Constant distribution
 */
double Constant::GetRV()
{
	return _mean;
}

/**
 * Copies Constant distribution object
 */
Distribution* Constant::CopyThis()
{
	Constant* newptr = new Constant(_mean);
	return newptr;
}

/**
 * Prints mean value from Constant distribution
 */
void Constant::PrintDistribution()
{
	printf("Constant - mean: %f\n", _mean);
}

/**
 * Constructor for Weibull distribution
 */
Weibull::Weibull(double scale, double shape)
{
	_scale = scale;
	_shape = shape;
	_distr = new std::weibull_distribution<double>(scale, shape);
}

/**
 * Copies Weibull distribution object
 */
Distribution* Weibull::CopyThis()
{
	Weibull* newptr = new Weibull(_scale, _shape);
	return newptr;
}

/**
 * Gets a random number from the Weibull distribution curve
 */
double Weibull::GetRV()
{
	return _distr->operator()(generator);
}

/**
 * Prints the values Weibull distribution curve: Scale, Shape
 */
void Weibull::PrintDistribution()
{
	printf("Weibull - scale, shape: %f %f %f\n", _scale, _shape);

}
