#include "Distribution.h"

Distribution::Distribution() {}

string Distribution::GetSystemSeedType()
{
	return _seedType;  
}

string Distribution::_seedType = "null";  
int Distribution::_systemSeed = 0;  

void Distribution::PrintDistribution()
{
}

std::default_random_engine Distribution::generator;  

void Distribution::SetSystemSeed(int seed) {
	int sysSeed = IsSystemSeedTypeSameorDifferent(seed);  
	//^Functinon above updated system seed
//TO DO: If statement for seed based on type
	if (sysSeed == 0) {
		cout << "Seed was the same" << endl;  
	}
	else if (sysSeed == 1)
		cout << "System Seed was different, system seed updated" << endl;  
}

void Distribution::SetSystemSeedType(string seedType) {
	_seedType = seedType;  
}

//void Distribution::SetSystemSeed(int seed)
//{
//	_systemSeed = seed;  
//}

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

int Distribution::GetSystemSeed()
{
	return _systemSeed;  
}

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

double Exponential::GetMean()
{
	return _mean;  
}

std::exponential_distribution<double>* Exponential::GetDistr()
{
	return _distr;  
}

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
double Uniform::GetRV()
{
	return _distr->operator()(generator);  
}

void Uniform::PrintDistribution()
{
	printf("Uniform - min, max: %f %f \n", _min, _max);  

}

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

std::uniform_real_distribution<double>* Triangular::GetDistr()
{
	return _distr;  
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

Distribution* Triangular::CopyThis()
{
	Triangular* newptr = new Triangular(_min, _expected, _max);  
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

void Normal::PrintDistribution()
{
	printf("Normal - mean, stdev: %f %f\n", _mean, _stdev);  
}

Poisson::Poisson(double mean)
{
	_mean = mean;  
	_distr = new std::poisson_distribution<int>(mean);  
}

std::poisson_distribution<int>* Poisson::GetDistr()
{
	return _distr;  
}

//double Poisson::GetMean()
//{
//	return _mean;  
//}

double Poisson::GetRV()
{
	return _distr->operator()(generator);  
}

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

void Poisson::PrintDistribution()
{
	printf("Poisson - mean: %f\n", _mean);  
}

Constant::Constant(double mean)
{
	_mean = mean;  
}

//double Constant::GetMean()
//{
//	return _mean;  
//}

double Constant::GetRV()
{
	return _mean;  
}

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

//double Weibull::GetScale()
//{
//	return _scale;  
//}
//
//double Weibull::GetShape()
//{
//	return _shape;  
//}

Distribution* Weibull::CopyThis()
{
	Weibull* newptr = new Weibull(_scale, _shape);  
	return newptr;  
}

double Weibull::GetRV()
{
	return _distr->operator()(generator);  
}

void Weibull::PrintDistribution()
{
	printf("Weibull - scale, shape: %f %f %f\n", _scale, _shape);  

}
