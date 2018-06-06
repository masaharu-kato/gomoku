#pragma once
#include "stone.h"
#include "position.h"
#include <iostream>
#include <functional>

namespace Kato {

	class Pattern {
	private:
		using RawType = char;
		RawType pattern;
		
		
		std::ostream& outBasicPatternVisual(std::ostream& os) const;
		std::ostream& outBasicPatternName  (std::ostream& os) const;

		using GetRelStoneFunc = std::function<Stone::RelKind(const Position&)>;
		
		static RawType getPattern(GetRelStoneFunc, const Position&, const Position&);
	public:
		Pattern(GetRelStoneFunc, const Position&, const Position&);

		std::ostream& operator <<(std::ostream& os) const;
	};

};
