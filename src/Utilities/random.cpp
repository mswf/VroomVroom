#include "random.h"
#include "standardIncludes.h"

unsigned int Random::rndSeed;
std::mt19937 Random::generator;


void Random::SetRandomSeed(const unsigned int seed)
{
	rndSeed = seed;
	generator = std::mt19937(rndSeed);
	srand(rndSeed);
}

int Random::GetRandomSeed()
{
	return rndSeed;
}

int Random::Next(const int max)
{
	return Next(0, max);
}

int Random::Next(const int max, const unsigned seed)
{
	SetRandomSeed(seed);
	return Next(max);
}

int Random::Next(const int min, const int max)
{
	int rnd;

	bool fancyRand = true;

	if (fancyRand)
	{
		rnd = generator() % max;
	}
	else
	{
		rnd = rand() % max;
	}
	SDL_assert(rnd >= min);
	SDL_assert(rnd < max);
	SDL_assert(min != max);
	return rnd;
}

int Random::Next(const int min, const int max, const unsigned seed)
{
	SetRandomSeed(seed);
	return Next(min, max);
}