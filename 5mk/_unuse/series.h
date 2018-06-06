#pragma once
#include "stone.h"
#include "directions.h"
#include <vector>

namespace Core {
	
//	��������Ɋւ��Ă̓��F�΂̘A������ێ�����N���X
	class Stone::Series {
	public:
		class Dir {
		private:

			struct Data {
				Kind kind;
				int number;
			};

			std::vector<Data> data;

		public:
			Dir() = default;

			void pushStone(Kind);

		};

		Dir dirs[N_directions];
	};

}