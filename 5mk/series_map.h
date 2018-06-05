#pragma once
#include "series.h"
#include "position_with_direction.h"
#include <unordered_map>
#include <unordered_set>

namespace Core {

	class Stone::SeriesMap {
		using Map = std::unordered_map<Position, Series>;
		using PosDir = PositionWithDirection;
		using PosDirSet = std::unordered_set<PosDir>;

		Map all;

		struct Specials {
			struct EachNumer {
				PosDirSet one_side_empty;	//	�Е��̂݋󂢂Ă���
				PosDirSet both_side_empty;	//	�����󂢂Ă���
				PosDirSet separated;		//	���f����Ă���
			} two, three, four; //	�O, �l
		} white, black; //	��, ��

	//	�w��ʒu�̓�������Ɋւ���f�[�^��ǉ�����
		void add(const PosDir& posdir, const Series::Dir& series_dir);

	public:

	//	�w��ʒu�Ɋւ���f�[�^��ǉ�����
		void add(const Position& pos, const Series& series);

		Series* get(const Position& pos) const;

	};

}