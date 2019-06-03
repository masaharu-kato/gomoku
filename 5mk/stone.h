#pragma once
#include <iostream>
#include <stdexcept>

namespace F19 {

	class Stone{
	public:
		enum StoneType : char {
			None  = 0,
			Black = 1,
			White = 2,
		};

		enum class StoneChar : char {
			None  = '.',
			Black = 'X',
			White = 'O',
		};

		Stone();
		Stone(StoneType stone);
		explicit Stone(char c);

		size_t getID() const;
		char getChar() const;

		//bool isNone() const;
		//bool isBlack() const;
		//bool isWhite() const;
		//bool isNoneOrBlack() const;
		//bool isNoneOrWhite() const;

		bool operator ==(Stone stone) const;
		bool operator !=(Stone stone) const;

		static Stone reverse(Stone stone);

	private:
		StoneType stone;
	};

	std::istream& operator >>(std::istream& is, Stone& stone);
	std::ostream& operator <<(std::ostream& os, const Stone& stone);

}