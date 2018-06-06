#pragma once
#include "strategy.h"

namespace Core {
	
	class Strategy5 : public Strategy {
	private:
		int tomecheck(const Position& p ,int s ,int t);
		int l_check(const Position& p ,int s ,int t);
		int hyouka2(const Position& p);
		int hyouka1(const Position& p);
		int kuu(const Position& p, int len[][BOARD_SIZE]);
		int sslencheck(const Position& p);
		int yonlenCheck(const Position& p);
		int golenCheck(const Position& p);

		BoardMap<Stone::Kind> cboard;
	public:
		virtual Position getNextTurn() override;
	};

}