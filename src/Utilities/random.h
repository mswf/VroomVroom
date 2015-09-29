#ifndef random_h
#define random_h
#include <random>

class Random
{
public:

	static void SetRandomSeed(const unsigned int seed);
	static int GetRandomSeed();

	//Exclusive max
	static int Next(const int max);
	static int Next(const int max, const unsigned int seed);

	//Inclusive min, Exclusive max
	static int Next(const int min, const int max);
	static int Next(const int min, const int max, const unsigned int seed);


private:
	static unsigned int rndSeed;
	static std::mt19937 generator;
};

#endif /* random_h */
