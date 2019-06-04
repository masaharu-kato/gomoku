////////////////////////////////////////////////////////////////////////////////////////////////
//
//	F19グループ  人工知能・五目並べソースファイル（関数部分のみ）　Ver.2019/06/04
//
//	- このファイルの中身すべてを組み込んでください。
//  - 戦略関数は strategyF19 です。
//
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __F19_COMPILE_WITH_BASE__
#pragma once
#include "define.h"
#endif

#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include <limits>
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <cmath>

void strategyF19(const int board_array[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history);

namespace F19 {
	
//	座標を表現する型
	using CoordType = __int32;

//	X座標
	class X {
	public:
		X();
		X(CoordType c);
		operator CoordType() const;

		X operator +(X x) const;
		X& operator +=(X x);
		X& operator ++();
		X& operator ++(int);
		X operator -() const;
		X operator -(X x) const;
		X& operator -=(X x);
		X& operator --();
		X& operator --(int);

	private:
		CoordType c;
	};
	
//	Y座標
	class Y {
	public:
		Y();
		Y(CoordType c);
		operator CoordType() const;
		
		Y operator +(Y Y) const;
		Y& operator +=(Y Y);
		Y& operator ++();
		Y& operator ++(int);
		Y operator -() const;
		Y operator -(Y Y) const;
		Y& operator -=(Y Y);
		Y& operator --();
		Y& operator --(int);

	private:
		CoordType c;
	};

//	位置型
	class Pos {
	public:
	//	ハッシュ関数クラス
		struct Hash;

		Pos();
		Pos(X x, Y y);

		bool operator ==(const Pos& pos) const;
		bool operator !=(const Pos& pos) const;

		X x;	//	X座標
		Y y;	//	Y座標
	};
	
//	ハッシュ関数クラス
	struct Pos::Hash {
		using result_type = __int64;
	//	ハッシュ関数
		__int64 operator()(const Pos& pos) const;
	};

	
//	石のクラス
	class Stone{
	public:

	//	石の種類(ID)
		enum StoneType : char {
			None  = 0,
			Black = 1,
			White = 2,
		};

	//	石の文字を表現する文字型
		using Char = char;

	//	入出力に用いる石の文字
		enum class StoneChar : Char {
			None  = '.',
			Black = 'X',
			White = 'O',
		};

		Stone();
		Stone(StoneType stone);
		explicit Stone(Char c);

	//	石のIDを取得
		size_t getID() const;

	//	等価比較
		bool operator ==(Stone stone) const;
		
	//	不等価比較
		bool operator !=(Stone stone) const;

	//	石 stone を反転する
		static Stone reverse(Stone stone);
		
	//	反転した石を返す
		Stone getReversed() const;

		friend std::istream& operator >>(std::istream& is, Stone& stone);
		friend std::ostream& operator <<(std::ostream& os, const Stone& stone);

	private:
	//	石を表現する文字を返す
		Char getChar() const;

		StoneType stone;
	};

//	入力関数
	std::istream& operator >>(std::istream& is, Stone& stone);

//	出力関数
	std::ostream& operator <<(std::ostream& os, const Stone& stone);


	
//	石の列（並び）を表現するクラス
	class StoneLine : public std::vector<Stone> {
	public:

	//	ハッシュ関数用のクラス
		struct Hash;

	//	基底型(石の配列)
		using Base = std::vector<Stone>;

	//	このクラス用のポインタ
		using Ptr = std::shared_ptr<StoneLine>;

	//	このクラスのインスタンスを保持するためのマップ
		using Map = std::unordered_map<Base, Ptr, Hash>;

		using Index = size_t;

	//	評価値の型
		using Value = double;

	//	既に作成されたインスタンスを保存するマップ
		static Map instances;

	//	石の並びからインスタンスを取得する（ない場合は作成する）
		static Ptr instance(const Base&);

	//	位置 index (0から数える) に石 stone を置いた列を取得
		Ptr getWith(Index index, Stone stone) const;

	//	石を反転させた列を取得
		Ptr getReversed() const;

	//	targetの石が5目(5連続)を形成しているかどうか
		bool is5mk(Stone target) const;

	//	targetの石についての評価値を取得
		Value value(Stone target) const;

	//	位置 index に石 target を置いた時の評価値を取得
		Value next_value(Stone target, Index index) const;

	////	targetの石を位置indexに置いた時の評価値から現在の評価値を引いた値
	//	Value next_value_diff(Stone target, Index index) const;

	//	等価比較(ハッシュマップで利用)
		bool operator==(const StoneLine&) const;
		
	//	不等価比較(ハッシュマップで利用)
		bool operator!=(const StoneLine&) const;

	//	デバッグ用
		static void out_instances(std::ostream& os);

	//	無視係数
		static double miss_coef;

	private:
	//	コンストラクタ（石の配列から StoneLine オブジェクトを作成）
		StoneLine(const Base&);

	//	すべての次の評価値を計算する関数
	//	void calc_next_values(Stone target) const;

		mutable std::unique_ptr<Value> _value[2];
		mutable std::vector<Value> _next_values[2];

	};
		
//	入力関数
	std::istream& operator >>(std::istream& is, StoneLine& stone_line);
	
//	出力関数
	std::ostream& operator <<(std::ostream& os, const StoneLine& stone_line);

//	ハッシュ関数用のクラス
	struct StoneLine::Hash {
		using result_type = size_t;
	//	ハッシュ関数
		size_t operator()(const Base& stone_line) const;
	};


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


	
	//	戦略関数
	void strategy(const int board_array[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history);








	X::X() = default;

	X::X(CoordType c)
		: c(c) {}

	X::operator CoordType() const{
		return c;
	}

	X X::operator +(X x) const {
		return c + x;
	}

	X& X::operator +=(X x) {
		c += x;
		return *this;
	}

	X& X::operator ++() {
		++c;
		return *this;
	}

	X& X::operator ++(int) {
		c++;
		return *this;
	}

	X X::operator-() const {
		return -c;
	}

	X X::operator -(X x) const {
		return c - x;
	}

	X& X::operator -=(X x) {
		c -= x;
		return *this;
	}

	X& X::operator --() {
		--c;
		return *this;
	}

	X& X::operator --(int) {
		c--;
		return *this;
	}



	Y::Y() = default;

	Y::Y(CoordType c)
		: c(c) {}

	Y::operator CoordType() const{
		return c;
	}

	Y Y::operator +(Y Y) const {
		return c + Y;
	}

	Y& Y::operator +=(Y Y) {
		c += Y;
		return *this;
	}

	Y& Y::operator ++() {
		++c;
		return *this;
	}

	Y& Y::operator ++(int) {
		c++;
		return *this;
	}

	Y Y::operator-() const {
		return -c;
	}

	Y Y::operator -(Y Y) const {
		return c - Y;
	}

	Y& Y::operator -=(Y Y) {
		c -= Y;
		return *this;
	}

	Y& Y::operator --() {
		--c;
		return *this;
	}

	Y& Y::operator --(int) {
		c--;
		return *this;
	}


	Pos::Pos() = default;
	Pos::Pos(X x, Y y)
		: x(x), y(y) {}

	bool Pos::operator ==(const Pos& pos) const {
		return x == pos.x && y == pos.y;
	}

	bool Pos::operator !=(const Pos& pos) const {
		return !operator==(pos);
	}

	__int64 Pos::Hash::operator ()(const Pos& pos) const {
		return ((__int64)pos.x << 32) + pos.y;
	}




	F19::Stone::Stone()
		: stone(None) {}

	F19::Stone::Stone(StoneType stone)
		: stone(stone) {}

	F19::Stone::Stone(Char c) {
		if(c == (Char)StoneChar::None ){
			stone = Stone::None;
		}
		else if(c == (Char)StoneChar::Black){
			stone = Stone::Black;
		}
		else if(c == (Char)StoneChar::White){
			stone = Stone::White;
		}
		else{
			throw std::exception("Invalid stone type.");
		}
	}

	auto F19::Stone::getChar() const -> Char {
		if(stone == Stone::None ) return (Char)StoneChar::None;
		if(stone == Stone::Black) return (Char)StoneChar::Black;
		if(stone == Stone::White) return (Char)StoneChar::White;
		throw std::exception("Invalid stone type.");
	}

	size_t F19::Stone::getID() const {
		return stone;
	}

	bool F19::Stone::operator ==(Stone _stone) const {
		return stone == _stone.stone;
	}

	bool F19::Stone::operator !=(Stone _stone) const {
		return !operator ==(_stone);
	}

	auto F19::Stone::reverse(Stone stone) -> Stone {
		if(stone == Stone::None ) return Stone::None;
		if(stone == Stone::Black) return Stone::White;
		if(stone == Stone::White) return Stone::Black;
		throw std::exception("Invalid stone type.");
	}

	auto F19::Stone::getReversed() const -> Stone {
		return reverse(*this);
	}


	std::istream& F19::operator >>(std::istream& is, Stone& stone) {
		Stone::Char c;
		is >> c;
		stone = (Stone)c;
		return is;
	}

	std::ostream& F19::operator <<(std::ostream& os, const Stone& stone) {
		return os << stone.getChar();
	}

	//bool F19::Stone::isNone() const {
	//	return stone == None;
	//}
	//
	//bool F19::Stone::isBlack() const {
	//	return stone == Black;
	//}
	//
	//bool F19::Stone::isWhite() const {
	//	return stone == White;
	//}
	//
	//bool F19::Stone::isNoneOrBlack() const {
	//	return isNone() || isBlack();
	//}
	//
	//bool F19::Stone::isNoneOrWhite() const {
	//	return isNone() || isWhite();
	//}


	//	コンストラクタ
	F19::StoneLine::StoneLine(const Base& base)
		: Base(base) {}

	//	target の石についての評価値を取得
	auto F19::StoneLine::value(Stone target) const -> Value {
	
	//	すでに計算した評価値があればそれを取得
		auto& cache = _value[target.getID() - 1];
		if(cache) return *cache;

	//	空き位置に石を置いた時の評価値を記録するための配列を初期化
		auto& nexts = _next_values[target.getID() - 1];
		nexts.resize(size(), 0.0);

	//	自分の石
		Stone astone = target;
	
	//	敵の石
		Stone estone = target.getReversed();

	//	評価値
		Value value = 0;

	//	自分の石で5目を形成していれば 1 
		if(is5mk(astone)){
			value = 1;
		}
	//	相手の石で5目を形成していれば -1
		else if(is5mk(estone)){
			value = -1;
		}
		else{

		//	自分の石を置いた時の評価値の最大値
			Value max_aval = 0;	

		//	相手の石を置いた時の評価値の合計値
			Value sum_eval = 0;

		//	空き位置数のカウンタ
			size_t n_empty = 0;

			for(size_t i = 0; i < size(); i++) {

			//	空いてる位置それぞれを処理
				if(at(i) == Stone::None) {

				//	自分の石を置いた時の評価を計算
					auto c_aval = getWith(i, astone)->value(astone);
					nexts[i] = c_aval;
					if(!n_empty || max_aval < c_aval) max_aval = c_aval;

				//	相手の石を置いた時の評価を計算し、最小値を更新
					auto c_eval = getWith(i, estone)->value(astone);
					sum_eval += c_eval;

					n_empty++;
				}

			}
	
		//	評価値を計算（空き位置がない場合は 0 のまま）
			if(n_empty) {
				value = miss_coef * max_aval + (1.0 - miss_coef) * (sum_eval / n_empty);
			}

		}
	
	//	評価値を記録
		cache.reset(new Value(value));
	
		return value;
	}

	//	位置 index に石 target を置いた時の評価値を取得
	auto F19::StoneLine::next_value(Stone target, Index index) const -> Value {
		auto& values = _next_values[target.getID() - 1];
		if(!values.size()) value(target);
		return values[index];
	}

	//	等価比較
	bool F19::StoneLine::operator==(const StoneLine& _line) const {

		if(size() != _line.size()) return false;

		for(size_t i = 0; i < size(); i++) {
			if(at(i) != _line.at(i)) return false;
		}

		return true;

	}

	//	不等価比較
	bool F19::StoneLine::operator!=(const StoneLine& _line) const {
		return !operator ==(_line);
	}

	//	石の並びからインスタンスを取得する
	auto F19::StoneLine::instance(const Base& base) -> Ptr {
		if(instances.count(base)) return instances.at(base);
		auto ptr = std::shared_ptr<StoneLine>(new StoneLine(base));
		instances.insert_or_assign(base, ptr);

	//#if _DEBUG
	//	if(instances.size() % 10000 == 0) std::cerr << instances.size() << "\n";
	//#endif

		return ptr;
	}

	//	target の石が5目を形成しているか返す
	bool F19::StoneLine::is5mk(Stone target) const{

	//	target の石の連続数
		int n_con = 0;

		for(auto stone : *this) {
			if(stone == target) {
				n_con++;
			}
			else{
				n_con = 0;
			}
			if(n_con == 5) return true;
		}

		return false;
	}

	//	位置 index に石 stone を置いた列を取得
	auto F19::StoneLine::getWith(Index index, Stone stone) const -> Ptr {

		if(index < 0 || index >= size()) throw std::exception("Index out of range.");

		Base stone_line = *this;
		stone_line[index] = stone;

		return instance(stone_line);

	}

	//	石を反転させた列を取得
	auto F19::StoneLine::getReversed() const -> Ptr {

		Base reversed;

		for(auto stone : *this){
			if(stone == Stone::None) {
				reversed.push_back(Stone::None);
			}
			else if(stone == Stone::Black){
				reversed.push_back(Stone::White);
			}
			else if(stone == Stone::White){
				reversed.push_back(Stone::Black);
			}
			else{
				throw new std::exception("Invalid stone type.");
			}
		}

		return instance(reversed);

	}

	//	入力関数
	std::istream& F19::operator >>(std::istream& is, StoneLine& stone_line) {
		std::string str;
		is >> str;
		for(size_t i = 0; i < str.size(); i++) {
			stone_line[i] = (Stone::StoneType)str[i];
		}
		return is;
	}

	//	出力関数
	std::ostream& F19::operator <<(std::ostream& os, const StoneLine& stone_line) {
		for(auto stone : stone_line) {
			os << stone;
		}
		return os;
	}

	//	ハッシュ関数の実装
	size_t F19::StoneLine::Hash::operator ()(const Base& stone_line) const {
	
		size_t hash = 0;

		for(auto stone : stone_line) {
			hash = hash * 4 + stone.getID();
		}

		return hash;

	}

	void F19::StoneLine::out_instances(std::ostream& os) {
		for(const auto& i : StoneLine::instances) {
			std::cout << i.first
				<< "\tB:" << i.second->value(Stone::Black)
				<< "\tW:" << i.second->value(Stone::White)
				<< "\n";
		}
	}

	F19::StoneLine::Map F19::StoneLine::instances;
	double F19::StoneLine::miss_coef = 0.5;



	F19::Board::Board() = default;

	auto F19::Board::getStone(X x, Y y) const -> Stone {
		return board[x][y];
	}

	bool F19::Board::isInside(X x, Y y) {
		return x >= 0 && x < size && y >= 0 && y < size;
	}

	auto F19::Board::getStone(Pos p) const -> Stone {
		return getStone(p.x, p.y);
	}

	bool F19::Board::isInside(Pos p) {
		return isInside(p.x, p.y);
	}

	auto F19::Board::getLine(X begx, Y begy, X invx, Y invy) -> Line {

		X cx = begx;
		Y cy = begy;
		Line poses;

		while(isInside(cx, cy)) {
			poses.emplace_back(cx, cy);
			cx += invx;
			cy += invy;
		}

		return poses;

	}

	void F19::Board::generateLines() {

		size_t i = 0;
		for(X x = 0; x < size; ++x) lines[i++] = getLine(x, 0, 0, 1);
		for(Y y = 0; y < size; ++y) lines[i++] = getLine(0, y, 1, 0);
		for(X x = 0; x < size; ++x) lines[i++] = getLine(x, 0, 1, 1);
		for(Y y = 1; y < size; ++y) lines[i++] = getLine(0, y, 1, 1);
		for(X x = 0; x < size; ++x) lines[i++] = getLine(x, 0, -1, 1);
		for(Y y = 1; y < size; ++y) lines[i++] = getLine(size - 1, y, -1, 1);

		f_lines_generated = true;
	
	}


	auto F19::Board::calcNextPosition(Stone target) -> Pos {

		PosValueMap values;

		Stone astone = target;
		Stone estone = target.getReversed();

	//	全ての位置の評価値を計算する
		forEachLines([&](const Line& line, StoneLine::Ptr ptr){
			for(size_t i = 0; i < line.size(); i++) {
				auto cpos = line[i];
				if(getStone(cpos) == Stone::None) {
					values[cpos] +=
								 val_ally_weight  * -log(1 - pow(ptr->next_value(astone, i), val_exp_lv))
						+ (1.0 - val_ally_weight) * -log(1 - pow(ptr->next_value(estone, i), val_exp_lv));
				}
			}
		});

	//	最大値をとる位置を求める

		Pos max_pos;
		StoneLine::Value max_value;
		bool isfirst = true;

		for(const auto& i : values) {
			if(isfirst || max_value < i.second){
				max_pos = i.first;
				max_value = i.second;
			}
			isfirst = false;
		}

		if(isfirst) throw std::exception("No empty positions.");


	#ifdef _DEBUG
		std::cout << "\n==== F19 Debug Information ====\n";
		for(Y y = 0; y < Board::size; ++y) {
			for(X x = 0; x < Board::size; ++x) {
				Pos p(x, y);
				if(values.count(p)){
					auto cval = values[p];
					if(std::isinf(cval)) cval = 99.999999;
					printf("%+10.6f ", cval);
				}else{
					auto cstone = getStone(p);
				//	if(cstone == Stone::None) throw std::exception("None stone is impossible here.");
					std::cout << "    [" << cstone << "]    ";
				}
			}
			std::cout << "\n";
		}
		std::cout << "================================\n";
	#endif

		return max_pos;

	};




	void F19::Board::setStone(X x, Y y, Stone stone) {
		board[x][y] = stone;
	}

	void F19::Board::setStone(Pos p, Stone stone) {
		return setStone(p.x, p.y, stone);
	}

	//void F19::Board::forEachEmpties(std::function<void(Pos)> func) const {
	//	for(Y y = 0; y < Board::size; ++y) {
	//		for(X x = 0; x < Board::size; ++x) {
	//			if(getStone(x, y) == Stone::None) func({x, y});
	//		}
	//	}
	//}
	//
	//void F19::Board::forEachEmpties(std::function<void(X, Y)> func) const {
	//	for(Y y = 0; y < Board::size; ++y) {
	//		for(X x = 0; x < Board::size; ++x) {
	//			if(getStone(x, y) == Stone::None) func(x, y);
	//		}
	//	}
	//}

	void F19::Board::forEachLines(std::function<void(const Line&, StoneLine::Ptr)> func) const {

		if(!f_lines_generated) generateLines();

		for(const auto& line : lines) func(line, generateStoneLine(line));

	}

	auto F19::Board::generateStoneLine(const Line& line) const -> StoneLine::Ptr {
		StoneLine::Base base;
		for(auto pos : line) {
			base.push_back(getStone(pos));
		}
		return StoneLine::instance(base);
	}


	//	入力関数
	std::istream& F19::operator >>(std::istream& is, Board& board) {
		for(Y y = 0; y < Board::size; ++y) {
			std::string str;
			is >> str;
			if(!y && !str.length()) continue;
			if(str.length() != Board::size) throw std::exception("Invalid input length.");
			for(X x = 0; x < Board::size; ++x) {
				board.setStone(x, y, Stone(str[x]));
			}
		}
		return is;
	}

	//	出力関数
	std::ostream& F19::operator <<(std::ostream& os, const Board& board) {
		for(Y y = 0; y < Board::size; ++y) {
			for(X x = 0; x < Board::size; ++x) {
				os << board.getStone(x, y);
			}
			os << "\n";
		}
		return os;
	}


	//	static 変数の初期化

	//	評価値の累乗係数
	int F19::Board::val_exp_lv = 1;

	//	評価値の自分の評価値の重み係数
	double F19::Board::val_ally_weight = 0.5;

	//	盤面のすべての列を生成したかどうか
	bool F19::Board::f_lines_generated = false;

	//	盤面のすべての列
	F19::Board::AllLines F19::Board::lines;


	//	戦略関数の実装
	void F19::strategy(const int board_array[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) {

	//	現在の石の色を算出
		Stone cstone = (count % 2 == 0) ? Stone::Black : Stone::White;

	//	盤面を読み込む
		Board board;

		for(Y y = 0; y < Board::size; y++) {
			for(X x = 0; x < Board::size; x++) {
				board.setStone(x, y, (Stone::StoneType)board_array[y][x]);
			}
		}

	//	石を置く位置を計算
		Pos next_pos = board.calcNextPosition(cstone);

	//	位置を代入
		*pos_x = next_pos.x;
		*pos_y = next_pos.y;

		return;
	}

}


//	戦略関数
void strategyF19(const int board_array[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) {
	return F19::strategy(board_array, pos_x, pos_y, count, history);
}



////[F19 End of File]/////////////////////////////////////////////////////////////////////////////////////////