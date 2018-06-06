#pragma once
#include "strategy.h"

namespace Core {
	
	class Strategy2 : public Strategy {
	private:
		int scan(const Position& p, const Position& diff, Stone::Kind kind);
		Position calcForce(const Position& p);
		Position calc();

	public:
		virtual Position getNextTurn() override;
	};

}