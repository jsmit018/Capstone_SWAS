#include "Distribution.h"

/**
 * Distribution constructor
 */
Distribution::Distribution() {}

/**
 * Sets the initial seed type to "null" before it is read in
 */
string Distribution::_seedType = "null";

/**
 * Pringt function for the inherited parent class of Distribution
 */
void Distribution::PrintDistribution()
{
}

/**
 * Creates a generator for Ditribution to be used for the random variable creation
 */
std::default_random_engine Distribution::generator;

//void Distribution::SetSystemSeed(int seed) {
//	generator.seed(seed);

//TO DO: If statement for seed based on type

//}

/**
 * Sets the seedType of distribution based on the seed type listed in the input file
 */
void Distribution::SetSystemSeedType(string seedType) {
	_seedType = seedType;
}

/**
 * Exponential Distribution constructor that sets the mean and creates the specified distribution
 */
Exponential::Exponential(double mean) : Distribution()
{
	_mean = mean;
	_distr = new std::exponential_distribution<double>(1.0 / mean);
}

/**
 * Gets a random variable for the distribution
 */
double Exponential::GetRV()
{
	return _distr->operator()(generator);
}

/**
 * Prints the Exponential Distributions values
 */
void Exponential::PrintDistribution()
{
	printf("Exponential - mean: %f \n", _mean);
}

/**
 * Returns the mean of the Exponential Distribution
 */
double Exponential::GetMean()
{
	return _mean;
}

/**
 * Returns the distibution itself
 */
std::exponential_distribution<double>* Exponential::GetDistr()
{
	return _distr;
}

/**
 * Creates an Exponential Distribution pointer in order to copy the distribution
 */
Distribution* Exponential::CopyThis()
{
	Exponential* newptr = new Exponential(_mean);
	return newptr;
}

//void Exponential::Copy(Distribution& dis)
//{
//	Exponential * old_child = dynamic_cast<Exponential *>(&dis);
//	_mean = old_child->GetMean();
//	_distr = old_child->GetDistr();
//}

/**
 * Uniform Distribution constructor that sets the min, max, and creates the specified distribution
 */
Uniform::Uniform(double min, double max) : Distribution()
{
	_min = min;
	_max = max;
	_distr = new std::uniform_real_distribution<double>(min, max);
}

//double Uniform::GetMin()
//{
//	return _min;
//}
//
//double Uniform::GetMax()
//{
//	return _max; 
//}
//

/**
 * Gets a random variable for the distribution
 */
double Uniform::GetRV()
{
	return _distr->operator()(generator);
}

/**
 * Prints the Uniform Distribution values
 */
void Uniform::PrintDistribution()
{
	printf("Uniform - min, max: %f %f \n", _min, _max);

}

/**
 * Creates an Uniform Distribution pointer in order to copy the distribution
 */
Distribution* Uniform::CopyThis()
{
	Uniform* newptr = new Uniform(_min, _max);
	return newptr;
}

//
//void Uniform::Copy(Distribution& dis)
//{
//	Uniform * old_child = dynamic_cast<Uniform*>(&dis);
//	_min = old_child->GetMin();
//	_max = old_child->GetMax();
//}

/**
 * Triangular Distribution constructor that sets the min, max, expected, and creates the specified distribution
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

/*double Triangular::GetA()
{
	return a;
}

double Triangular::GetB()
{
	return b;
}

double Triangular::GetC()
{
	return c;
}

double Triangular::GetFc()
{
	return fc;
}

double Triangular::GetMin()
{
	return _min;
}

double Triangular::GetMax()
{
	return _max;
}

double Triangular::GetTerm1()
{
	return term1;
}
double Triangular::GetTerm2()
{
	return term2;
}

double Triangular::GetExpected()
{
	return _expected;
}*/

/**
 * Returns the distibution itself
 */
std::uniform_real_distribution<double>* Triangular::GetDistr()
{
	return _distr;
}

/**
 * Gets a random variable for the distribution
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
 * Creates an Triangular Distribution pointer in order to copy the distribution
 */
Distribution * Triangular::CopyThis()
{
	Triangular* newptr = new Triangular(_min,_expected, _max);
	return newptr; 
}

//void Triangular::Copy(Distribution& dis)
//{
//	Triangular* old_child = dynamic_cast<Triangular*>(&dis);
//	_min = old_child->GetMin();
//	_max = old_child->GetMax();
//	_expected = old_child->GetExpected();
//	a = old_child->GetA();
//	b = old_child->GetB();
//	c = old_child->GetC();
//	fc = old_child->GetFc();
//	term1 = old_child->GetTerm1();
//	term2 = old_child->GetTerm2();
//	_distr = old_child->GetDistr();
//}

/**
 * Prints the Triangular Distribution values
 */
void Triangular::PrintDistribution()
{
	printf("Triangular - min, expected, max: %f %f %f \n", _min, _expected, _max);
}

/**
 * Normal Distribution constructor that sets the mean, standard deviation, and creates the specified distribution
 */
Normal::Normal(double mean, double stdev)
{
	_mean = mean;
	_stdev = stdev;
	_distr = new std::normal_distribution<double>(mean, stdev);
}

/**
 * Creates an Normal Distribution pointer in order to copy the distribution
 */
Distribution* Normal::CopyThis()
{
	Normal* newptr = new Normal(_mean, _stdev);
	return newptr;
}

//double Normal::GetMean()
//{
//	return _mean;
//}
//
//double Normal::GetStdev()
//{
//	return _stdev;
//}

/**
 * Gets a random variable for the distribution
 */
double Normal::GetRV()
{
	return _distr->operator()(generator);
}

//void Normal::Copy(Distribution& dis)
//{
//	Normal* old_child = dynamic_cast<Normal*>(&dis);
//	_mean = old_child->GetMean();
//	_stdev = old_child->GetStdev();
//}

/**
 * Prints the Normal Distribution values
 */
void Normal::PrintDistribution()
{
	printf("Normal - mean, stdev: %f %f\n", _mean, _stdev);
}

/**
 * Poisson Distribution constructor that sets the mean and creates the specified distribution
 */
Poisson::Poisson(double mean)
{
	_mean = mean;
	_distr = new std::poisson_distribution<int>(mean);
}

/**
 * Returns the distibution itself
 */
std::poisson_distribution<int>* Poisson::GetDistr()
{
	return _distr; 
}

//double Poisson::GetMean()
//{
//	return _mean;
//}

/**
 * Gets a random variable for the distribution
 */
double Poisson::GetRV()
{
	return _distr->operator()(generator);
}

/**
 * Creates an Poisson Distribution pointer in order to copy the distribution
 */
Distribution* Poisson::CopyThis()
{
	Poisson* newptr = new Poisson(_mean);
	return newptr;
}

//void Poisson::Copy(Distribution& dis)
//{
//	Poisson* old_child = dynamic_cast<Poisson*>(&dis);
//	_mean = old_child->GetMean();
//	_distr = old_child->GetDistr();
//}

/**
 * Prints the Poisson Distribution values
 */
void Poisson::PrintDistribution()
{
	printf("Poisson - mean: %f\n", _mean);
}

/**
 * Constant Distribution constructor that sets the mean.
 */
Constant::Constant(double mean)
{
	_mean = mean;
}

//double Constant::GetMean()
//{
//	return _mean;
//}

/**
 * Gets a random variable for the distribution
 */
double Constant::GetRV()
{
	return _mean;
}

/**
 * Creates an Constant Distribution pointer in order to copy the distribution
 */
Distribution* Constant::CopyThis()
{
	Constant* newptr = new Constant(_mean);
	return newptr;
}

//void Constant::Copy(Distribution& dis)
//{
//	Constant* old_child = dynamic_cast<Constant*>(&dis);
//	_mean = old_child->GetMean();
//}

/**
 * Prints the Constant Distribution values
 */
void Constant::PrintDistribution()
{
	printf("Constant - mean: %f\n", _mean);
}

/**
 * Weibull Distribution constructor that sets the scale, shape, and creates the specified distribution
 */
Weibull::Weibull(double scale, double shape)
{
	_scale = scale;
	_shape = shape;
	_distr = new std::weibull_distribution<double>(scale, shape);
}

//double Weibull::GetScale()
//{
//	return _scale;
//}
//
//double Weibull::GetShape()
//{
//	return _shape;
//}

/**
 * Creates an Weibull Distribution pointer in order to copy the distribution
 */
Distribution* Weibull::CopyThis()
{
	Weibull* newptr = new Weibull(_scale, _shape);
	return newptr;
}

/**
 * Gets a random variable for the distribution
 */
double Weibull::GetRV()
{
	return _distr->operator()(generator);
}

/**
 * Prints the Weibull Distribution values
 */
void Weibull::PrintDistribution()
{
	printf("Weibull - scale, shape: %f %f %f\n", _scale, _shape);

}