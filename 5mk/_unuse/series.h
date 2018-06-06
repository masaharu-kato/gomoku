#pragma once
#include "stone.h"
#include "directions.h"
#include <vector>

namespace Core {
	
//	ある方向に関しての同色石の連続個数を保持するクラス
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