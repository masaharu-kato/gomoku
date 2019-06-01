#pragma once
#include <iostream>
#include <stdexcept>

namespace F19 {

	class Stone{
	public:
		enum StoneType : char {
			None  = '.',
			Black = 'X',
			White = 'O',
		};

		Stone(StoneType stone);

		operator StoneType() const;

	private:
		StoneType stone;
	};

	//std::istream& operator >>(std::istream& is, Stone& stone);
	//std::ostream& operator <<(std::ostream& os, const Stone& stone);

}