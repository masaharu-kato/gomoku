#pragma once
#include "position.h"

namespace Core {

	Position directions[] = {
		{ 0, -1},
		{ 1, -1},
		{ 1,  0},
		{ 1, +1},
		{ 0, +1},
		{-1, +1},
		{-1,  0},
		{-1, -1},
	};

	using Direction = char;

	constexpr int N_directions = 8;

}