////////////////////////////////////////////////////////////////////////////////////////////////
//
//	F19�O���[�v  �l�H�m�\�E�ܖڕ��׃\�[�X�t�@�C���i�֐������̂݁j�@Ver.2019/06/04
//
//	- ���̃t�@�C���̒��g���ׂĂ�g�ݍ���ł��������B
//  - �헪�֐��� strategyF19 �ł��B
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
	
//	���W��\������^
	using CoordType = __int32;

//	X���W
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
	
//	Y���W
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

//	�ʒu�^
	class Pos {
	public:
	//	�n�b�V���֐��N���X
		struct Hash;

		Pos();
		Pos(X x, Y y);

		bool operator ==(const Pos& pos) const;
		bool operator !=(const Pos& pos) const;

		X x;	//	X���W
		Y y;	//	Y���W
	};
	
//	�n�b�V���֐��N���X
	struct Pos::Hash {
		using result_type = __int64;
	//	�n�b�V���֐�
		__int64 operator()(const Pos& pos) const;
	};

	
//	�΂̃N���X
	class Stone{
	public:

	//	�΂̎��(ID)
		enum StoneType : char {
			None  = 0,
			Black = 1,
			White = 2,
		};

	//	�΂̕�����\�����镶���^
		using Char = char;

	//	���o�͂ɗp����΂̕���
		enum class StoneChar : Char {
			None  = '.',
			Black = 'X',
			White = 'O',
		};

		Stone();
		Stone(StoneType stone);
		explicit Stone(Char c);

	//	�΂�ID���擾
		size_t getID() const;

	//	������r
		bool operator ==(Stone stone) const;
		
	//	�s������r
		bool operator !=(Stone stone) const;

	//	�� stone �𔽓]����
		static Stone reverse(Stone stone);
		
	//	���]�����΂�Ԃ�
		Stone getReversed() const;

		friend std::istream& operator >>(std::istream& is, Stone& stone);
		friend std::ostream& operator <<(std::ostream& os, const Stone& stone);

	private:
	//	�΂�\�����镶����Ԃ�
		Char getChar() const;

		StoneType stone;
	};

//	���͊֐�
	std::istream& operator >>(std::istream& is, Stone& stone);

//	�o�͊֐�
	std::ostream& operator <<(std::ostream& os, const Stone& stone);


	
//	�΂̗�i���сj��\������N���X
	class StoneLine : public std::vector<Stone> {
	public:

	//	�n�b�V���֐��p�̃N���X
		struct Hash;

	//	���^(�΂̔z��)
		using Base = std::vector<Stone>;

	//	���̃N���X�p�̃|�C���^
		using Ptr = std::shared_ptr<StoneLine>;

	//	���̃N���X�̃C���X�^���X��ێ����邽�߂̃}�b�v
		using Map = std::unordered_map<Base, Ptr, Hash>;

		using Index = size_t;

	//	�]���l�̌^
		using Value = double;

	//	���ɍ쐬���ꂽ�C���X�^���X��ۑ�����}�b�v
		static Map instances;

	//	�΂̕��т���C���X�^���X���擾����i�Ȃ��ꍇ�͍쐬����j
		static Ptr instance(const Base&);

	//	�ʒu index (0���琔����) �ɐ� stone ��u��������擾
		Ptr getWith(Index index, Stone stone) const;

	//	�΂𔽓]����������擾
		Ptr getReversed() const;

	//	target�̐΂�5��(5�A��)���`�����Ă��邩�ǂ���
		bool is5mk(Stone target) const;

	//	target�̐΂ɂ��Ă̕]���l���擾
		Value value(Stone target) const;

	//	�ʒu index �ɐ� target ��u�������̕]���l���擾
		Value next_value(Stone target, Index index) const;

	////	target�̐΂��ʒuindex�ɒu�������̕]���l���猻�݂̕]���l���������l
	//	Value next_value_diff(Stone target, Index index) const;

	//	������r(�n�b�V���}�b�v�ŗ��p)
		bool operator==(const StoneLine&) const;
		
	//	�s������r(�n�b�V���}�b�v�ŗ��p)
		bool operator!=(const StoneLine&) const;

	//	�f�o�b�O�p
		static void out_instances(std::ostream& os);

	//	�����W��
		static double miss_coef;

	private:
	//	�R���X�g���N�^�i�΂̔z�񂩂� StoneLine �I�u�W�F�N�g���쐬�j
		StoneLine(const Base&);

	//	���ׂĂ̎��̕]���l���v�Z����֐�
	//	void calc_next_values(Stone target) const;

		mutable std::unique_ptr<Value> _value[2];
		mutable std::vector<Value> _next_values[2];

	};
		
//	���͊֐�
	std::istream& operator >>(std::istream& is, StoneLine& stone_line);
	
//	�o�͊֐�
	std::ostream& operator <<(std::ostream& os, const StoneLine& stone_line);

//	�n�b�V���֐��p�̃N���X
	struct StoneLine::Hash {
		using result_type = size_t;
	//	�n�b�V���֐�
		size_t operator()(const Base& stone_line) const;
	};


//	�ՖʃN���X
	class Board {
	public:

		using Ptr = std::shared_ptr<Board>;

	//	�Ֆʂ̃T�C�Y
		constexpr static CoordType size = 10;

	//	�Ֆʂ̐΂̔z��^(�����珇�� X���W, Y���W)
		using BoardArray = Stone[size][size];
	
	//	�Ֆʓ��̗�̌^
		using Line = std::vector<Pos>;

		Board();

	//	�V�����Ֆʂ̍쐬
	//	static Ptr create();

	//	�ʒup�ɐ�stone��u�����Ֆʂ��擾
	//	Ptr getWith(Pos p, Stone stone) const;

	//	�ʒu p �ɂ���΂��擾
		Stone getStone(Pos p) const;

	//	�ʒu (x, y) �ɂ���΂��擾
		Stone getStone(X x, Y y) const;

	//	�ʒu p ���Ֆʓ������ׂ�(�ÓI)
		static bool isInside(Pos p);

	//	�ʒu (x, y) ���Ֆʓ������ׂ�(�ÓI)
		static bool isInside(X x, Y y);

	////	�c�E���E�΂ߑS�Ă̕��т���������(�΂̕��т̂�)
	//	void forEachLines(std::function<void(StoneLine::Ptr)>) const;
		
	//	�c�E���E�΂ߑS�Ă̕��т���������(�ʒu�Ɛ΂̕���)
		void forEachLines(std::function<void(const Line&, StoneLine::Ptr)>) const;

	////	���ׂĂ̋󂢂Ă���ʒu�ɂ��ď�������
	//	void forEachEmpties(std::function<void(Pos)>) const;
	//	void forEachEmpties(std::function<void(X, Y)>) const;
		
	//	�ʒu (x, y) �ɐ� stone ��z�u
		void setStone(X x, Y y, Stone stone);
		
	//	�ʒu p �ɐ� stone ��z�u
		void setStone(Pos p, Stone stone);

	//	�S�󂫈ʒu�̕]���l���v�Z���A���ɐ� target ��u���ׂ��ʒu���擾
		Pos calcNextPosition(Stone target);
		

	//	�]���l�̗ݏ�W��
		static int val_exp_lv;

	//	�]���l�̎����̕]���l�̏d�݌W��
		static double val_ally_weight;


	private:
		using PosValueMap = std::unordered_map<Pos, StoneLine::Value, Pos::Hash>;

	//	�Ֆʂɂ����̐��i�c�E���E�΂߁j
		constexpr static size_t Line_Length = 6 * size - 2;
		using AllLines = std::array<Line, Line_Length>;

	//	�Ֆʂ̐΂̔z��
		BoardArray board = {Stone::None};

	//	�񂩂�΂̗���擾
		StoneLine::Ptr generateStoneLine(const Line& line) const;



	//	(begx, begy) ���N�_�Ƃ��A(invx, invy) ���i�񂾂Ƃ��̗���擾
		static Line getLine(X begx, Y begy, X invx, Y invy);

	//	�Ֆʂ̂��ׂĂ̗�𐶐�
		static void generateLines();
		
	//	�Ֆʂ̂��ׂĂ̗�𐶐��������ǂ���
		static bool f_lines_generated;

	//	�Ֆʂ̂��ׂĂ̗�
		static AllLines lines;

	};
	
//	���͊֐�
	std::istream& operator >>(std::istream& is, Board& board);

//	�o�͊֐�
	std::ostream& operator <<(std::ostream& os, const Board& board);


	
	//	�헪�֐�
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


	//	�R���X�g���N�^
	F19::StoneLine::StoneLine(const Base& base)
		: Base(base) {}

	//	target �̐΂ɂ��Ă̕]���l���擾
	auto F19::StoneLine::value(Stone target) const -> Value {
	
	//	���łɌv�Z�����]���l������΂�����擾
		auto& cache = _value[target.getID() - 1];
		if(cache) return *cache;

	//	�󂫈ʒu�ɐ΂�u�������̕]���l���L�^���邽�߂̔z���������
		auto& nexts = _next_values[target.getID() - 1];
		nexts.resize(size(), 0.0);

	//	�����̐�
		Stone astone = target;
	
	//	�G�̐�
		Stone estone = target.getReversed();

	//	�]���l
		Value value = 0;

	//	�����̐΂�5�ڂ��`�����Ă���� 1 
		if(is5mk(astone)){
			value = 1;
		}
	//	����̐΂�5�ڂ��`�����Ă���� -1
		else if(is5mk(estone)){
			value = -1;
		}
		else{

		//	�����̐΂�u�������̕]���l�̍ő�l
			Value max_aval = 0;	

		//	����̐΂�u�������̕]���l�̍��v�l
			Value sum_eval = 0;

		//	�󂫈ʒu���̃J�E���^
			size_t n_empty = 0;

			for(size_t i = 0; i < size(); i++) {

			//	�󂢂Ă�ʒu���ꂼ�������
				if(at(i) == Stone::None) {

				//	�����̐΂�u�������̕]�����v�Z
					auto c_aval = getWith(i, astone)->value(astone);
					nexts[i] = c_aval;
					if(!n_empty || max_aval < c_aval) max_aval = c_aval;

				//	����̐΂�u�������̕]�����v�Z���A�ŏ��l���X�V
					auto c_eval = getWith(i, estone)->value(astone);
					sum_eval += c_eval;

					n_empty++;
				}

			}
	
		//	�]���l���v�Z�i�󂫈ʒu���Ȃ��ꍇ�� 0 �̂܂܁j
			if(n_empty) {
				value = miss_coef * max_aval + (1.0 - miss_coef) * (sum_eval / n_empty);
			}

		}
	
	//	�]���l���L�^
		cache.reset(new Value(value));
	
		return value;
	}

	//	�ʒu index �ɐ� target ��u�������̕]���l���擾
	auto F19::StoneLine::next_value(Stone target, Index index) const -> Value {
		auto& values = _next_values[target.getID() - 1];
		if(!values.size()) value(target);
		return values[index];
	}

	//	������r
	bool F19::StoneLine::operator==(const StoneLine& _line) const {

		if(size() != _line.size()) return false;

		for(size_t i = 0; i < size(); i++) {
			if(at(i) != _line.at(i)) return false;
		}

		return true;

	}

	//	�s������r
	bool F19::StoneLine::operator!=(const StoneLine& _line) const {
		return !operator ==(_line);
	}

	//	�΂̕��т���C���X�^���X���擾����
	auto F19::StoneLine::instance(const Base& base) -> Ptr {
		if(instances.count(base)) return instances.at(base);
		auto ptr = std::shared_ptr<StoneLine>(new StoneLine(base));
		instances.insert_or_assign(base, ptr);

	//#if _DEBUG
	//	if(instances.size() % 10000 == 0) std::cerr << instances.size() << "\n";
	//#endif

		return ptr;
	}

	//	target �̐΂�5�ڂ��`�����Ă��邩�Ԃ�
	bool F19::StoneLine::is5mk(Stone target) const{

	//	target �̐΂̘A����
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

	//	�ʒu index �ɐ� stone ��u��������擾
	auto F19::StoneLine::getWith(Index index, Stone stone) const -> Ptr {

		if(index < 0 || index >= size()) throw std::exception("Index out of range.");

		Base stone_line = *this;
		stone_line[index] = stone;

		return instance(stone_line);

	}

	//	�΂𔽓]����������擾
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

	//	���͊֐�
	std::istream& F19::operator >>(std::istream& is, StoneLine& stone_line) {
		std::string str;
		is >> str;
		for(size_t i = 0; i < str.size(); i++) {
			stone_line[i] = (Stone::StoneType)str[i];
		}
		return is;
	}

	//	�o�͊֐�
	std::ostream& F19::operator <<(std::ostream& os, const StoneLine& stone_line) {
		for(auto stone : stone_line) {
			os << stone;
		}
		return os;
	}

	//	�n�b�V���֐��̎���
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

	//	�S�Ă̈ʒu�̕]���l���v�Z����
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

	//	�ő�l���Ƃ�ʒu�����߂�

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


	//	���͊֐�
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

	//	�o�͊֐�
	std::ostream& F19::operator <<(std::ostream& os, const Board& board) {
		for(Y y = 0; y < Board::size; ++y) {
			for(X x = 0; x < Board::size; ++x) {
				os << board.getStone(x, y);
			}
			os << "\n";
		}
		return os;
	}


	//	static �ϐ��̏�����

	//	�]���l�̗ݏ�W��
	int F19::Board::val_exp_lv = 1;

	//	�]���l�̎����̕]���l�̏d�݌W��
	double F19::Board::val_ally_weight = 0.5;

	//	�Ֆʂ̂��ׂĂ̗�𐶐��������ǂ���
	bool F19::Board::f_lines_generated = false;

	//	�Ֆʂ̂��ׂĂ̗�
	F19::Board::AllLines F19::Board::lines;


	//	�헪�֐��̎���
	void F19::strategy(const int board_array[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) {

	//	���݂̐΂̐F���Z�o
		Stone cstone = (count % 2 == 0) ? Stone::Black : Stone::White;

	//	�Ֆʂ�ǂݍ���
		Board board;

		for(Y y = 0; y < Board::size; y++) {
			for(X x = 0; x < Board::size; x++) {
				board.setStone(x, y, (Stone::StoneType)board_array[y][x]);
			}
		}

	//	�΂�u���ʒu���v�Z
		Pos next_pos = board.calcNextPosition(cstone);

	//	�ʒu����
		*pos_x = next_pos.x;
		*pos_y = next_pos.y;

		return;
	}

}


//	�헪�֐�
void strategyF19(const int board_array[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) {
	return F19::strategy(board_array, pos_x, pos_y, count, history);
}



////[F19 End of File]/////////////////////////////////////////////////////////////////////////////////////////