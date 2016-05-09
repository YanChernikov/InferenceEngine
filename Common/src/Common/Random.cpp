#include "Random.h"

#include <time.h>

bool Random::s_Initialized = false;

Random::Generator Random::s_Generator;

void Random::Init()
{
	s_Generator.seed((unsigned int)time(NULL));
	s_Initialized = true;
}

int Random::Int()
{
	ASSERT(s_Initialized);
	std::uniform_int_distribution<uint32_t> distribution;
	return distribution(s_Generator);
}

int Random::Int(int max)
{
	ASSERT(s_Initialized);
	std::uniform_int_distribution<uint32_t> distribution(0, max);
	return distribution(s_Generator);
}

int Random::Int(int min, int max)
{
	ASSERT(s_Initialized);
	std::uniform_int_distribution<uint32_t> distribution(min, max);
	return distribution(s_Generator);
}
