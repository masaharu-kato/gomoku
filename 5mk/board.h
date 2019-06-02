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

	//	�V�����Ֆʂ̍쐬
		static Ptr create();

	//	�ʒup�ɐ�stone��u�����Ֆʂ��擾
		Ptr getWith(Pos p, Stone stone) const;

	//	�ʒu p �ɂ���΂��擾
		Stone getStone(Pos p) const;

	//	�ʒu (x, y) �ɂ���΂��擾
		Stone getStone(X x, Y y) const;

	//	�ʒu p ���Ֆʓ������ׂ�
		bool isInside(Pos p) const;

	//	�ʒu (x, y) ���Ֆʓ������ׂ�
		bool isInside(X x, Y y) const;

	//	�c����(Y�����̏���)�̕��т��擾
		StoneLine::Ptr getVertical     (X x) const;

	//	������(X�����̏���)�̕��т��擾
		StoneLine::Ptr getHorizontal   (Y y) const;

	//	X�����N�_�Ƃ��鍶�΂߂̕��т��擾
		StoneLine::Ptr getLeftDiagonal (X x) const;

	//	Y�����N�_�Ƃ��鍶�΂߂̕��т��擾
		StoneLine::Ptr getLeftDiagonal (Y y) const;
		
	//	X�����N�_�Ƃ���E�΂߂̕��т��擾
		StoneLine::Ptr getRightDiagonal(X x) const;
		
	//	Y�����N�_�Ƃ���E�΂߂̕��т��擾
		StoneLine::Ptr getRightDiagonal(Y y) const;

	//	�c�E���E�΂ߑS�Ă̕��т���������
		void forEachLines(std::function<void(StoneLine::Ptr)>) const;

		//AllVerticalIterator      getAllVertical     () const;
		//AllHorizontalIterator    getAllHorizontal   () const;
		//AllRightDiagonalIterator getAllRightDiagonal() const;
		//AllLeftDiagonalIterator  getAllLeftDiagonal () const;

		LastPosLineIterator getLastPosLines() const;
		AllLineIterator getAllLines() const;

		constexpr static CoordType size = 10;

	//	���ׂĂ̋󂢂Ă���ʒu�ɂ��ď�������
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
	
//	���͊֐�
	std::istream& operator >>(std::istream& is, Board& board);

//	�o�͊֐�
	std::ostream& operator <<(std::ostream& os, const Board& board);

}