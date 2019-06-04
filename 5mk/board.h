#pragma once
#include "StoneLine.h"
#include "coord.h"
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include <array>
#include <iostream>

namespace F19 {

//	盤面クラス
	class Board {
	public:

		using Ptr = std::shared_ptr<Board>;

	//	盤面のサイズ
		constexpr static CoordType size = 10;

	//	盤面の石の配列型(左から順に X座標, Y座標)
		using BoardArray = Stone[size][size];
	
	//	盤面内の列の型
		using Line = std::vector<Pos>;

		Board();

	//	新しい盤面の作成
	//	static Ptr create();

	//	位置pに石stoneを置いた盤面を取得
	//	Ptr getWith(Pos p, Stone stone) const;

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

	////	すべての空いている位置について処理する
	//	void forEachEmpties(std::function<void(Pos)>) const;
	//	void forEachEmpties(std::function<void(X, Y)>) const;
		
	//	位置 (x, y) に石 stone を配置
		void setStone(X x, Y y, Stone stone);
		
	//	位置 p に石 stone を配置
		void setStone(Pos p, Stone stone);

	//	全空き位置の評価値を計算し、次に石 target を置くべき位置を取得
		Pos calcNextPosition(Stone target);
		

	//	評価値の累乗係数
		static int val_exp_lv;

	//	評価値の自分の評価値の重み係数
		static double val_ally_weight;


	private:
		using PosValueMap = std::unordered_map<Pos, StoneLine::Value, Pos::Hash>;

	//	盤面にある列の数（縦・横・斜め）
		constexpr static size_t Line_Length = 6 * size - 2;
		using AllLines = std::array<Line, Line_Length>;

	//	盤面の石の配列
		BoardArray board = {Stone::None};

	//	列から石の列を取得
		StoneLine::Ptr generateStoneLine(const Line& line) const;



	//	(begx, begy) を起点とし、(invx, invy) ずつ進んだときの列を取得
		static Line getLine(X begx, Y begy, X invx, Y invy);

	//	盤面のすべての列を生成
		static void generateLines();
		
	//	盤面のすべての列を生成したかどうか
		static bool f_lines_generated;

	//	盤面のすべての列
		static AllLines lines;

	};
	
//	入力関数
	std::istream& operator >>(std::istream& is, Board& board);

//	出力関数
	std::ostream& operator <<(std::ostream& os, const Board& board);

}