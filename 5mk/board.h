#include "base.h"
#include "stone.h"
#include "position.h"
#include <vector>
#include <memory>
#include <functional>

namespace Kato {

	class Board;
	using BoardPtr = std::shared_ptr<Board>;

	class Board {
		using Data = Stone::Kind**;
		using History = std::vector<Position>;
		using Count = int;
		constexpr static int Size = BOARD_SIZE;
		constexpr static int SizeX = Size;
		constexpr static int SizeY = Size;

		Data data;
		History history;
		Count count;

		

	/*
		struct Data {
			Position p;
			Stone::Kind kind;
			Stone::Series series;

			Data(const Position& p, Stone::Kind kind) : p(p), kind(kind) {}
		};

		std::vector<Data> series_list;

	//	����ʒu�Ɋւ��ď������s��
		void calcOn(const Position& pos);

	//	�ʒu�ƕ������w�肵�ĘA�������v�Z
		void calcSeries(Stone::Series::Dir&, Position pos, Direction dir);

	//	�ʒu�̑S�����̘A�������v�Z
		void calcSeries(const Position& pos);
	*/

	//	�ʒu���͈͓����Ԃ�
		bool isInside(const Position& pos) const;

	//	�f�[�^��̎w�肵���ʒu�ɐ΂�u��(����, ��Ԃ̃J�E���g�͍s��Ȃ�)
		Stone::Kind setStone(const Position& pos);

	public:

		Board() {};

	//	�w��ʒu�̐΂̎�ނ�Ԃ��@�͈͊O�Ȃ�-1
		Stone::Kind getStone(const Position&) const;

	//	�w��ʒu�ɔC�ӂ̐F�̐΂�u��
		Stone::Kind putStone(const Position&, Stone::Kind&);

		const History& getHistory() const {
			return history;
		}

	//	n�Ԗڂɒu�����΂̐F���擾 (����� 0 �Ƃ���)
		Position getHistory(Count) const;

	//	�Ōォ��n�Ԗڂɒu�����΂̐F���擾 (��O�̎�� 1 �Ƃ���)
		Position getLastHistory(Count) const;

	//	���݉���ڂł��邩�擾����
		Count getCount() const;

	//	���݂̃^�[�����擾����
		Stone::Kind getKind() const;
		
	//	�Ֆʏ�̂��ׂĂ̈ʒu�Ɋւ��ď������s��
		void doEach(std::function<void(const Position&)>) const;
		void doEach(std::function<void(const Position&, Stone::Kind)>) const;
		void doEach(std::function<void(const Position&, Stone::Kind&)>);
		void doEachEmpty(std::function<void(const Position&)>) const;
		Stone::Kind& operator [](const Position&);

	};

}