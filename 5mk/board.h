#pragma once
#include "StoneLine.h"
#include "coord.h"
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include <array>

namespace F19 {
	
	using Line = std::vector<Pos>;

	class Board {
	public:

		using Ptr = std::shared_ptr<Board>;

	//	新しい盤面の作成
		static Ptr create();

	//	位置pに石stoneを置いた盤面を取得
		Ptr getWith(Pos p, Stone stone) const;

	//	位置 p にある石を取得
		Stone getStone(Pos p) const;

	//	位置 (x, y) にある石を取得
		Stone getStone(X x, Y y) const;

	//	位置 p が盤面内か調べる(静的)
		static bool isInside(Pos p);

	//	位置 (x, y) が盤面内か調べる(静的)
		static bool isInside(X x, Y y);

	////	縦・横・斜め全ての並びを処理する(石の並びのみ)
	//	void forEachLines(std::function<void(StoneLine::Ptr)>) const;
		
	//	縦・横・斜め全ての並びを処理する(位置と石の並び)
		void forEachLines(std::function<void(const Line&, StoneLine::Ptr)>) const;

		constexpr static CoordType size = 10;

	//	すべての空いている位置について処理する
		void forEachEmpties(std::function<void(Pos)>) const;
		void forEachEmpties(std::function<void(X, Y)>) const;
		

		friend std::istream& operator >>(std::istream& is, Board& board);


		Board() = default;

		void calc_next_values(Stone target);

		
		static int val_exp_lv;
		

	private:
		using PosValueMap = std::unordered_map<Pos, StoneLine::Value, Pos::Hash>;

		constexpr static size_t Line_Length = 6 * size - 2;
		using AllLines = std::array<Line, Line_Length>;

		Stone board[size][size] = {Stone::None};

		Board(Ptr parent, Pos pos);

		void setStone(X x, Y y, Stone stone);
		void setStone(Pos p, Stone stone);

		static Line getLine(X begx, Y begy, X invx, Y invy);
		StoneLine::Ptr generateStoneLine(const Line& line) const;
		static void generateLines();

		Ptr parent;
		Stone last_stone;
		
		static bool f_lines_generated;
		static AllLines lines;


	};
	
//	入力関数
	std::istream& operator >>(std::istream& is, Board& board);

//	出力関数
	std::ostream& operator <<(std::ostream& os, const Board& board);

}