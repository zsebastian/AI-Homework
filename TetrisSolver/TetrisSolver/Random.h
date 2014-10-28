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