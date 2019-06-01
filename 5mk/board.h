#pragma once
#include "stone.h"
#include <memory>
#include <unordered_map>

namespace F19 {

	using CoordType = int;

	struct X {CoordType c;};
	struct Y {CoordType c;};

	struct Pos {
		X x;
		Y y;
	};

	class Board {
	public:

		using Ptr = std::shared_ptr<Board>;

		class Itr;
		class VerticalItr;
		class HorizontalItr;
		class RightDiagonalItr;
		class LeftDiagonalItr;

		//class AllVerticalItr;
		//class AllHorizontalItr;
		//class AllRightDiagonalItr;
		//class AllLeftDiagonalItr;

		class LastPosLineItr;
		class AllLineItr;

		using Value = int;

		Ptr getBoardWith(Pos p, Stone stone) const;
		Stone getStone(Pos p) const;
		bool isInside(Pos p) const;

		VerticalItr      getVertical     (X x) const;
		HorizontalItr    getHorizontal   (Y y) const;
		RightDiagonalItr getRightDiagonal(X x) const;
		RightDiagonalItr getRightDiagonal(Y y) const;
		LeftDiagonalItr  getLeftDiagonal (X x) const;
		LeftDiagonalItr  getLeftDiagonal (Y y) const;

		//AllVerticalItr      getAllVertical     () const;
		//AllHorizontalItr    getAllHorizontal   () const;
		//AllRightDiagonalItr getAllRightDiagonal() const;
		//AllLeftDiagonalItr  getAllLeftDiagonal () const;

		LastPosLineItr getLastPosLines() const;
		AllLineItr getAllLines() const;

		constexpr static CoordType size = 10;

	private:
	//	Stone board[size][size];

		Board();
		Board(Ptr base, Pos pos);

		Ptr base;
		Pos last_pos;
		Stone last_stone;
		Value current_value;

		std::unordered_map<Pos, Ptr> nexts; 

	};

}