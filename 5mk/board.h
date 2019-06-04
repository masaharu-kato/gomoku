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

}