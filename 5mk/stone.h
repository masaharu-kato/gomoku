#pragma once
#include "base.h"

namespace Kato {

	class Stone {
	public:
		
	//	�΂̎�� 0�Ȃ疳���A'W'�Ȃ甒�A'B'�Ȃ獕�A-1�Ȃ��
		using Kind = char;

	//	�΂̑��ΓI��� 0�Ȃ疳���A1�Ȃ玩���̐F, -1�Ȃ瑊��̐F�܂��͕�
		using RelKind = char;

		using Number = int;

	//	�A����
		class Series;

	//	Series�̏W��
		class SeriesMap;


	};

};
