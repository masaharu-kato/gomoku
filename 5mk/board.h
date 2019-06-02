#pragma once
#include "StoneLine.h"
#include "coord.h"
#include <memory>
#include <unordered_map>
#include <functional>

namespace F19 {

	class Board {
	public:

		using Ptr = std::shared_ptr<Board>;

		class Iterator;
		//class VerticalIterator;
		//class HorizontalIterator;
		//class RightDiagonalIterator;
		//class LeftDiagonalIterator;

		//class AllVerticalIterator;
		//class AllHorizontalIterator;
		//class AllRightDiagonalIterator;
		//class AllLeftDiagonalIterator;

		class LastPosLineIterator;
		class AllLineIterator;

		using Value = int;

	//	新しい盤面の作成
		static Ptr create();

	//	位置pに石stoneを置いた盤面を取得
		Ptr getWith(Pos p, Stone stone) const;

	//	位置 p にある石を取得
		Stone getStone(Pos p) const;

	//	位置 (x, y) にある石を取得
		Stone getStone(X x, Y y) const;

	//	位置 p が盤面内か調べる
		bool isInside(Pos p) const;

	//	位置 (x, y) が盤面内か調べる
		bool isInside(X x, Y y) const;

	//	縦方向(Y方向の昇順)の並びを取得
		StoneLine::Ptr getVertical     (X x) const;

	//	横方向(X方向の昇順)の並びを取得
		StoneLine::Ptr getHorizontal   (Y y) const;

	//	X軸を起点とする左斜めの並びを取得
		StoneLine::Ptr getLeftDiagonal (X x) const;

	//	Y軸を起点とする左斜めの並びを取得
		StoneLine::Ptr getLeftDiagonal (Y y) const;
		
	//	X軸を起点とする右斜めの並びを取得
		StoneLine::Ptr getRightDiagonal(X x) const;
		
	//	Y軸を起点とする右斜めの並びを取得
		StoneLine::Ptr getRightDiagonal(Y y) const;

	//	縦・横・斜め全ての並びを処理する
		void forEachLines(std::function<void(StoneLine::Ptr)>) const;

		//AllVerticalIterator      getAllVertical     () const;
		//AllHorizontalIterator    getAllHorizontal   () const;
		//AllRightDiagonalIterator getAllRightDiagonal() const;
		//AllLeftDiagonalIterator  getAllLeftDiagonal () const;

		LastPosLineIterator getLastPosLines() const;
		AllLineIterator getAllLines() const;

		constexpr static CoordType size = 10;

	//	すべての空いている位置について処理する
		void forEachEmpties(std::function<void(Pos)>) const;
		void forEachEmpties(std::function<void(X, Y)>) const;
		

		friend std::istream& operator >>(std::istream& is, Board& board);


		Board() = default;

		Value calc_value(Stone target) const;

		void calc_next_values(Stone target);
		

	private:
		Stone board[size][size] = {Stone::None};

		Board(Ptr parent, Pos pos);

		void setStone(X x, Y y, Stone stone);
		void setStone(Pos p, Stone stone);

		StoneLine::Ptr getLine(X begx, Y begy, X invx, Y invy) const;


		Ptr parent;
		Stone last_stone;
		Value current_value;

	//	std::unordered_map<Pos, Ptr> nexts; 

	};
	
//	入力関数
	std::istream& operator >>(std::istream& is, Board& board);

//	出力関数
	std::ostream& operator <<(std::ostream& os, const Board& board);

}