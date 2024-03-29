#pragma once
#include <iostream>

namespace F19 {

//	石のクラス
	class Stone{
	public:

	//	石の種類(ID)
		enum StoneType : char {
			None  = 0,
			Black = 1,
			White = 2,
		};

	//	石の文字を表現する文字型
		using Char = char;

	//	入出力に用いる石の文字
		enum class StoneChar : Char {
			None  = '.',
			Black = 'X',
			White = 'O',
		};

		Stone();
		Stone(StoneType stone);
		explicit Stone(Char c);

	//	石のIDを取得
		size_t getID() const;

	//	等価比較
		bool operator ==(Stone stone) const;
		
	//	不等価比較
		bool operator !=(Stone stone) const;

	//	石 stone を反転する
		static Stone reverse(Stone stone);
		
	//	反転した石を返す
		Stone getReversed() const;

		friend std::istream& operator >>(std::istream& is, Stone& stone);
		friend std::ostream& operator <<(std::ostream& os, const Stone& stone);

	private:
	//	石を表現する文字を返す
		Char getChar() const;

		StoneType stone;
	};

//	入力関数
	std::istream& operator >>(std::istream& is, Stone& stone);

//	出力関数
	std::ostream& operator <<(std::ostream& os, const Stone& stone);

}