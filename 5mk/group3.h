#pragma once
#include "strategy.h"

namespace Core {
	
	class Strategy3 : public Strategy {
	private:
		int choiceValueForComputer(Stone::Number ns_me, Stone::Number ns_prev, Stone::Kind turn);
		int choiceValueForPlayer  (Stone::Number ns_me, Stone::Number ns_prev, Stone::Kind turn);
	public:
		virtual Position getNextTurn() override;
	};

}