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

	//	�V�����Ֆʂ̍쐬
		static Ptr create();

	//	�ʒup�ɐ�stone��u�����Ֆʂ��擾
		Ptr getWith(Pos p, Stone stone) const;

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

		constexpr static CoordType size = 10;

	//	���ׂĂ̋󂢂Ă���ʒu�ɂ��ď�������
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
	
//	���͊֐�
	std::istream& operator >>(std::istream& is, Board& board);

//	�o�͊֐�
	std::ostream& operator <<(std::ostream& os, const Board& board);

}