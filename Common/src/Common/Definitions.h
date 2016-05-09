#pragma once

typedef unsigned int uint;
typedef unsigned char byte;

#define ASSERT(x) if (!(x)) { __debugbreak(); }
#define BIT(x) 1 << x