#pragma once
#include "board.h"
#include "position.h"
#include "stone.h"
#include "position_map.h"

namespace Core {

	class Strategy {
	protected:
		template <class T> using BoardMap = PositionMap<T, Board::SizeX, Board::SizeY>;
		BoardPtr board;
		Stone::Kind kind;
	public:
		Strategy(BoardPtr board, Stone::Kind kind) : board(board), kind(kind) {}
		
		virtual Position getNextTurn() = 0;
	};

}