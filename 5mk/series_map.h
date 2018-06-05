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
				PosDirSet one_side_empty;	//	片方のみ空いている
				PosDirSet both_side_empty;	//	両方空いている
				PosDirSet separated;		//	分断されている
			} two, three, four; //	三, 四
		} white, black; //	白, 黒

	//	指定位置の特定方向に関するデータを追加する
		void add(const PosDir& posdir, const Series::Dir& series_dir);

	public:

	//	指定位置に関するデータを追加する
		void add(const Position& pos, const Series& series);

		Series* get(const Position& pos) const;

	};

}