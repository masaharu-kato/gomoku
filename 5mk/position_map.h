#pragma once
#include "position.h"

namespace Core {

	template <class T, Position::Type SizeX, Position::Type SizeY> class PositionMap {
	private:
		T data[SizeY][SizeX];
	public:
		T& set(const Position&);

		T& get(const Position&);
		T  get(const Position&) const;
		
		T& operator [](const Position&);
		T  operator [](const Position&) const;

	};

}
