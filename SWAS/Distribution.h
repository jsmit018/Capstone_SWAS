#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <random>
#include <iostream>

using namespace std;  

class Distribution
{
public:
	Distribution();  

	static string GetSystemSeedType();  

	static void SetSystemSeedType(string seedType);  
	static void SetSystemSeed(int seed);  

	static int IsSystemSeedTypeSameorDifferent(int seed);  
	static int GetSystemSeed();  

	virtual void PrintDistribution() = 0;  
	virtual double GetRV() = 0;  
	virtual Distribution* CopyThis() = 0;  

protected:
	static std::default_random_engine generator;  
private:
	static string _seedType;  
	static int _systemSeed;  
};  

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