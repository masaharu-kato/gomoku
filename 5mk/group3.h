#pragma once
#include "strategy.h"

namespace Core {
	
	class Strategy3 : public Strategy {
	public:
		virtual Position getNextTurn() override;
	};

}