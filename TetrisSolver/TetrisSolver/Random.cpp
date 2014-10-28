#include "Random.h"

#include <cmath>
#include <limits>
#include <chrono>

Randomizer::Randomizer()
{
	Reseed();
}

Randomizer::Randomizer(unsigned int seed)
{
	Reseed(seed);
}

/*	Reseeds with current time. We can't seed with 0 until Microsoft fixes bug
*		Written: Sebastian Zander 2013-06-05 19:18
*/
void Randomizer::Reseed()
{
	unsigned int now = time(0);

	engine_.seed(now == 0 ? 1 : now);
}

/* Note: We can't seed with 0 until Microsoft fixes bug, so if 0 is input,
* it will seed with 1 instead
*		Written: Sebastian Zander 2013-06-05 19:18
*/
void Randomizer::Reseed(unsigned int seed)
{
	engine_.seed(seed == 0 ? 1 : seed);
}

unsigned int Randomizer::NextInt(unsigned int max)
{
	if (max == 0) return 0;
	std::uniform_int_distribution<unsigned int> dist(0, max);
	return dist(engine_);
}

int Randomizer::NextInt(int min, int max)
{
	if (max - min <= 0) return min;
	
	std::uniform_int_distribution<int> dist(min, max);
	return dist(engine_);
}

double Randomizer::NextDouble(double min, double max)
{
	if (max - min <= 0) return min;

	std::uniform_real_distribution<double> dist(min, max);
	return dist(engine_);
}

//this Generates a double between 0 and 1, and if that double is lower than trueFactor, true is returned
bool Randomizer::NextBool(double true_factor)
{
	return NextDouble(0.f, 1.f) <= true_factor;
}

unsigned int Randomizer::NextRand()
{
	return engine_();
}
