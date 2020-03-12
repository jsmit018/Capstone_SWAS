#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <random>
#include <iostream>

using namespace std;

class Distribution
{
public:
	Distribution();
	virtual double GetRV() = 0;
	static void SetSystemSeedType(string seedType);
	virtual void PrintDistribution() = 0;
protected:
	static std::default_random_engine generator;
private:
	static string _seedType;
};

class Exponential : public Distribution
{
public:
	Exponential(double mean);
	double GetRV();
	void PrintDistribution();
private:
	std::exponential_distribution<double>* _distr;
	double _mean;
};

class Uniform : public Distribution
{
public:
	Uniform(double min, double max);
	double GetRV();
	void PrintDistribution();
private:
	std::uniform_real_distribution<double>* _distr;
	double _min;
	double _max;
};

class Triangular : public Distribution
{
public:
	Triangular(double min, double expected, double max);
	double GetRV();
	void PrintDistribution();
private:
	std::uniform_real_distribution<double>* _distr;
	double a, b, c, fc, term1, term2;
	double _min;
	double _expected;
	double _max;
};

class Normal : public Distribution
{
public:
	Normal(double mean, double stdev);
	double GetRV();
	void PrintDistribution();
private:
	std::normal_distribution<double>* _distr;
	double _mean;
	double _stdev;
};

class Poisson : public Distribution
{
public:
	Poisson(double mean);
	double GetRV();
	void PrintDistribution();
private:
	std::poisson_distribution<int>* _distr;
	double _mean;
};

class Constant : public Distribution
{
public:
	Constant(double mean);
	double GetRV();
	void PrintDistribution();
private:
	double _mean;
};

class Weibull : public Distribution
{
public:
	Weibull(double scale, double shape);
	double GetRV();
	void PrintDistribution();
private:
	std::weibull_distribution<double>* _distr;
	double _scale;
	double _shape;
};

#endif