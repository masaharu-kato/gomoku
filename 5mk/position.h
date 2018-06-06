#pragma once
namespace Kato {

	struct Position {
		using Type = int;
		Type x, y;

		Position(Type x, Type y) : x(x), y(y) {}

		Position operator +(const Position& p) const;
		Position operator -(const Position& p) const;
		Position operator *(Position::Type) const;
		Position operator -() const;

		Position& operator +=(const Position& p);
		Position& operator -=(const Position& p);

		bool operator ==(const Position& p) const;
		bool operator !=(const Position& p) const;

	};
	
	Position operator *(Position::Type, const Position& p);

}