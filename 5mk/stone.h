#pragma once
#include "base.h"

namespace Kato {

	class Stone {
	public:
		
	//	石の種類 0なら無し、'W'なら白、'B'なら黒、-1なら壁
		using Kind = char;

	//	石の相対的種類 0なら無し、1なら自分の色, -1なら相手の色または壁
		using RelKind = char;

		using Number = int;

	//	連続個数
		class Series;

	//	Seriesの集合
		class SeriesMap;


	};

};
