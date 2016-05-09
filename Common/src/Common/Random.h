#pragma once

#include <random>
#include "Definitions.h"

class Random
{
private:
	typedef std::mt19937 Generator;
private:
	static bool s_Initialized;
	static Generator s_Generator;
public:
	static void Init();
	static int Int();
	static int Int(int max);
	static int Int(int min, int max);
};
