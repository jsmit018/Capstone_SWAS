#include "Distribution.h"

Distribution::Distribution() {}

void Distribution::PrintDistribution()
{
}

std::default_random_engine Distribution::generator;

//void Distribution::SetSystemSeed(int seed) {
//	generator.seed(seed);

//TO DO: If statement for seed based on type

//}

/*Void Distribution::SetSystemSeedType(string seedType) {
	_seedType = seedType;
}*/

Exponential::Exponential(double mean) : Distribution()
{
	_mean = mean;
	_distr = new std::exponential_distribution<double>(1.0 / mean);
}

double Exponential::GetRV()
{
	return _distr->operator()(generator);
}

void Exponential::PrintDistribution()
{
	printf("Exponential - mean: %f \n", _mean);
}

Uniform::Uniform(double min, double max) : Distribution()
{
	_min = min;
	_max = max;
	_distr = new std::uniform_real_distribution<double>(min, max);
}

double Uniform::GetRV()
{
	return _distr->operator()(generator);
}

void Uniform::PrintDistribution()
{
	printf("Uniform - min, max: %f %f \n", _min, _max);

}

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

void Triangular::PrintDistribution()
{
	printf("Triangular - min, expected, max: %f %f %f \n", _min, _expected, _max);
}

Normal::Normal(double mean, double stdev)
{
	_mean = mean;
	_stdev = stdev;
	_distr = new std::normal_distribution<double>(mean, stdev);
}

double Normal::GetRV()
{
	return _distr->operator()(generator);
}

void Normal::PrintDistribution()
{
	printf("Normal - mean, stdev: %f %f\n", _mean, _stdev);
}

Poisson::Poisson(double mean)
{
	_mean = mean;
	_distr = new std::poisson_distribution<int>(mean);
}

double Poisson::GetRV()
{
	return _distr->operator()(generator);
}

void Poisson::PrintDistribution()
{
	printf("Poisson - mean: %f\n", _mean);
}

Constant::Constant(double mean)
{
	_mean = mean;
}

double Constant::GetRV()
{
	return _mean;
}

void Constant::PrintDistribution()
{
	printf("Constant - mean: %f\n", _mean);
}

Weibull::Weibull(double scale, double shape)
{
	_scale = scale;
	_shape = shape;
	_distr = new std::weibull_distribution<double>(scale, shape);
}

double Weibull::GetRV()
{
	return _distr->operator()(generator);
}

void Weibull::PrintDistribution()
{
	printf("Weibull - scale, shape: %f %f %f\n", _scale, _shape);

}

