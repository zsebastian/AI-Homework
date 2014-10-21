#pragma once

/*	TO FIX:
*	The distibutions on the random number generator that uses marseene twister
*	is wrong. See STL's speech during GoingNative 2013 on rand().
*
*/
#include <ctime>
#include <mutex>
#include <random>

/*	A simpler random-generator, for no works on rand(), by reseeding for every number
*	with the last number. This means every Random-object is internally consistent
*	regardless of other objects.
*/
class Randomizer
{
public:
	//default seeds the generator with time()
	Randomizer();
	Randomizer(unsigned int seed);

	void Reseed();
	void Reseed(unsigned int seed);

	//returns a random int from 0 up to max (inclusive)
	unsigned int NextInt(unsigned int max);
	//returns a random int from min to max (inclusive)
	int NextInt(int min, int max);
	//returns a random double from min to max (inclusive)
	double NextDouble(double min, double max);
	//returns a random bool, where trueFactor is a value between 0.0 and 1.0, 
	//the higher the factor he higher the chance for true. With the current 
	//implementation the random double seems to be scewed towards 0.5
	bool NextBool(double true_factor);
	//just returns the next rand()
	unsigned int NextRand();

private:
	std::mt19937 engine_;
};

class Noise
{
public:
	double Generate(double x);
	double Generate(double x, double y);
	double Generate(double x, double y, double z);
private:
	double FindNoise(int x);
	double FindNoise(int x, int y);
	double FindNoise(int x, int y, int z);

	double Interpolate(double y1, double y2, double mu);

	int floor(double f);
	int ceil(double f);
	//wraps negative ints to postive.
	int WrapAround(int i);

	static const double PI;
};

class PerlinNoise
{
public:
	PerlinNoise(double frequency, int octaves, double amplitude, double lacunarity)
		:frequency_(frequency), lacunarity_(lacunarity), amplitude_(amplitude), octaves_(octaves)
	{}

	void FetFrequency(double frequency) { frequency_ = frequency; }
	void SetOctaves(int octaves) { octaves_ = octaves; }
	void SetLacunarity(double lacunarity) { lacunarity_ = lacunarity; }
	void SetAmplitude(double amplitude) { amplitude_ = amplitude; }

	double Generate(double x);
	double Generate(double x, double y);
	double Generate(double x, double y, double z);
private:
	template<typename returnType>
	returnType round(double f)
	{
		if (f >= 0x1e23) return static_cast<returnType>(f);

		if (f > 0.f)
			return static_cast<returnType>(static_cast<int>(f + 0.49999997f));
		else
		{
			try
			{
				f = abs(f);
				return static_cast<returnType>(-static_cast<int>(f + 0.49999997f));
			}
			catch (...)
			{
				return 0;
			}
		}
	}

	Noise noise;

	double frequency_;
	double lacunarity_;
	double amplitude_;
	int octaves_;
 };