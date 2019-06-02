#pragma once
#include <unordered_map>
#include "Board.h"
#include "StoneLine.h"

namespace F19 {

	class Learning {
	public:
	//	using StoneLine = std::string;
		using Steps = size_t;

		struct StepsData {
			Steps steps_win  = 0;
			Steps steps_lose = 0;
		};

		Learning(StoneLine init);

	private:
		StepsData learn(StoneLine line, Stone turn);

		static int evaluate(StoneLine stone_line);

		std::unordered_map<StoneLine, StepsData> line_to_steps;
	};
	
}