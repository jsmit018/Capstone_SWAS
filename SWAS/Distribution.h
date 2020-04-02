#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <random>
#include <iostream>

using namespace std;

/**
 * Base class  for distirubtion to be inherited  by the unique distribtuion types
 */
class Distribution
{
public:
	Distribution();
	virtual double GetRV() = 0;
	static void SetSystemSeedType(string seedType);
	virtual void PrintDistribution() = 0;
	virtual Distribution* CopyThis() = 0; 
protected:
	static std::default_random_engine generator;
private:
	static string _seedType;
};

/**
 * Class for an Exponential Distribution
 */
class Exponential : public Distribution
{
public:
	Exponential(double mean);
	double GetRV();
	double GetMean();
	std::exponential_distribution<double>* GetDistr();
	void PrintDistribution();
//	void Copy(Distribution& dis);
	Distribution* CopyThis();
private:
	std::exponential_distribution<double>* _distr;
	double _mean;
};

/**
 * Class for a Unifrom Distribution
 */
class Uniform : public Distribution
{
public:
	Uniform(double min, double max);
	double GetRV();
	double GetMin();
	double GetMax();
	void PrintDistribution();
	void Copy(Distribution& dis);
	Distribution* CopyThis();
private:
	std::uniform_real_distribution<double>* _distr;
	double _min;
	double _max;
};

/**
 * Class for a Triangular Distribution
 */
class Triangular : public Distribution
{
public:
	Triangular(double min, double expected, double max);
	std::uniform_real_distribution<double>* GetDistr();
	double GetRV();
	void PrintDistribution();
	void Copy(Distribution& dis);
	Distribution* CopyThis();
private:
	std::uniform_real_distribution<double>* _distr;
	double a, b, c, fc, term1, term2;
	double _min;
	double _expected;
	double _max;
};

/**
 * Class for a Normal Distribution
 */
class Normal : public Distribution
{
public:
	Normal(double mean, double stdev);
	double GetRV();
	double GetMean();
	double GetStdev();
	Distribution* CopyThis();
	void PrintDistribution();
private:
	std::normal_distribution<double>* _distr;
	double _mean;
	double _stdev;
};

/**
 * Class for a Poisson Distribution
 */
class Poisson : public Distribution
{
public:
	Poisson(double mean);
	double GetRV();
	Distribution* CopyThis();
	std::poisson_distribution<int>* GetDistr();
	double GetMean();
	void PrintDistribution();
//	void Copy(Distribution& dis);
private:
	std::poisson_distribution<int>* _distr;
	double _mean;
};

/**
 * Class for a Constant Distribution
 */
class Constant : public Distribution
{
public:
	Constant(double mean);
	double GetRV();
	double GetMean();
	Distribution* CopyThis();
	void PrintDistribution();
//	void Copy(Distribution& dis);
private:
	double _mean;
};

/**
 * Class for a Weibull Distribution
 */
class Weibull : public Distribution
{
public:
	Weibull(double scale, double shape);
	double GetRV();
	double GetScale();
	double GetShape();
	Distribution* CopyThis();
	void PrintDistribution();
//	void Copy(Distribution& dis);
private:
	std::weibull_distribution<double>* _distr;
	double _scale;
	double _shape;
};

#endif