#include "Random.h"

#include <cmath>
#include <limits>
#include <chrono>

const double Noise::PI = 3.14159265359;

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

double Noise::FindNoise(int x)
{
	x = WrapAround(x);
	int n = (x << 13) ^ x;
	return (double)(1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double Noise::FindNoise(int x, int y)
{
	x = WrapAround(x);
	y = WrapAround(y);
	int n = x * 331 + y * 337;
	n = (n << 13) ^ n;
	return (double)(1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double Noise::FindNoise(int x, int y, int z)
{
	x = WrapAround(x);
	y = WrapAround(y);
	z = WrapAround(z);
	//I'm guessing these are primes?
	int n = x * 331 + y * 337 + z * 339;
	n = (n << 13) ^ n;
	return (double)(1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

int Noise::WrapAround(int i)
{
	int max = std::numeric_limits<int>::max();
	if (i < 0)
		return max + i;

	return i;
}

double Noise::Generate(double x)
{
	double x1 = FindNoise(ceil(x));
	double x2 = FindNoise(floor(x));
	double mux = x - floorf(x);
	double xNoise = Interpolate(x1, x2, mux);

	return xNoise;
}

double Noise::Generate(double x, double y)
{
	double s, t, u, v;
	s = FindNoise(floor(x), floor(y));
	t = FindNoise(ceil(x), floor(y));
	u = FindNoise(floor(x), ceil(y));//Get the surrounding pixels to calculate the transition.
	v = FindNoise(ceil(x), ceil(y));
	double y1 = Interpolate(s, t, x - floorf(x));//Interpolate between the values.
	double y2 = Interpolate(u, v, x - floorf(x));//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	return Interpolate(y1, y2, y - floorf(y));//Here we use y-floory, to get the 2nd dimension.
}

double Noise::Generate(double x, double y, double z)
{
	double s, t, u, v, y1, y2;
	//first plane
	s = FindNoise(floor(x), floor(y), floor(z));
	t = FindNoise(ceil(x), floor(y), floor(z));
	u = FindNoise(floor(x), ceil(y), floor(z));//Get the surrounding pixels to calculate the transition.
	v = FindNoise(ceil(x), ceil(y), floor(z));
	y1 = Interpolate(s, t, x - floorf(x));//Interpolate between the values.
	y2 = Interpolate(u, v, x - floorf(x));//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	double plane1 = Interpolate(y1, y2, y - floorf(y));

	//second plane
	s = FindNoise(floor(x), floor(y), ceil(z));
	t = FindNoise(ceil(x), floor(y), ceil(z));
	u = FindNoise(floor(x), ceil(y), ceil(z));//Get the surrounding pixels to calculate the transition.
	v = FindNoise(ceil(x), ceil(y), ceil(z));
	y1 = Interpolate(s, t, x - floorf(x));//Interpolate between the values.
	y2 = Interpolate(u, v, x - floorf(x));//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	double plane2 = Interpolate(y1, y2, y - floorf(y));

	return Interpolate(plane1, plane2, z - floorf(z));

}

double Noise::Interpolate(double y1, double y2, double mu)
{
	double mu2 = (1 - cos(mu * PI)) / 2;
	return (y1 * (1 - mu2) + y2 * mu2);
}

int Noise::floor(double f)
{
	return round<int>(floorf(f));
}

int Noise::ceil(double f)
{
	return round<int>(ceilf(f));
}

double PerlinNoise::Generate(double x)
{
	//properties of one octave (changing each loop)
	double t = 0.0f;
	double freq = frequency_;

	for (int k = 0; k < octaves_; k++)
	{
		t += noise.Generate(x * freq) * amplitude_;
		amplitude_ *= lacunarity_;
		freq *= 2;
	}

	return t;
}

double PerlinNoise::Generate(double x, double y)
{
	//properties of one octave (changing each loop)
	double t = 0.0f;
	double freq = frequency_;

	for (int k = 0; k < octaves_; k++)
	{
		t += noise.Generate(x * freq, y * freq) * amplitude_;
		amplitude_ *= lacunarity_;
		freq *= 2;
	}

	return t;
}

double PerlinNoise::Generate(double x, double y, double z)
{
	//properties of one octave (changing each loop)
	double t = 0.0f;
	double freq = frequency_;

	for (int k = 0; k < octaves_; k++)
	{
		t += noise.Generate(x * freq, y * freq, z * freq) * amplitude_;
		amplitude_ *= lacunarity_;
		freq *= 2;
	}

	return t;
}