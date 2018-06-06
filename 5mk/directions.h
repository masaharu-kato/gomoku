#pragma once
#include "position.h"

namespace Kato {

	class Directions {
	public:
		static const Position half[];
		static const Position full[];
	};

	const Position Directions::half[] = {
		{ 0, -1},
		{ 1, -1},
		{ 1,  0},
		{ 1, +1},
	};

	const Position Directions::full[] = {
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