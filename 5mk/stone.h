#pragma once
#include <iostream>

namespace F19 {

//	Î‚ÌƒNƒ‰ƒX
	class Stone{
	public:

	//	Î‚Ìí—Ş(ID)
		enum StoneType : char {
			None  = 0,
			Black = 1,
			White = 2,
		};

	//	Î‚Ì•¶š‚ğ•\Œ»‚·‚é•¶šŒ^
		using Char = char;

	//	“üo—Í‚É—p‚¢‚éÎ‚Ì•¶š
		enum class StoneChar : Char {
			None  = '.',
			Black = 'X',
			White = 'O',
		};

		Stone();
		Stone(StoneType stone);
		explicit Stone(Char c);

	//	Î‚ÌID‚ğæ“¾
		size_t getID() const;

	//	“™‰¿”äŠr
		bool operator ==(Stone stone) const;
		
	//	•s“™‰¿”äŠr
		bool operator !=(Stone stone) const;

	//	Î stone ‚ğ”½“]‚·‚é
		static Stone reverse(Stone stone);
		
	//	”½“]‚µ‚½Î‚ğ•Ô‚·
		Stone getReversed() const;

		friend std::istream& operator >>(std::istream& is, Stone& stone);
		friend std::ostream& operator <<(std::ostream& os, const Stone& stone);

	private:
	//	Î‚ğ•\Œ»‚·‚é•¶š‚ğ•Ô‚·
		Char getChar() const;

		StoneType stone;
	};

//	“ü—ÍŠÖ”
	std::istream& operator >>(std::istream& is, Stone& stone);

//	o—ÍŠÖ”
	std::ostream& operator <<(std::ostream& os, const Stone& stone);

}