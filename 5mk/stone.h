#pragma once
#include <iostream>

namespace F19 {

//	�΂̃N���X
	class Stone{
	public:

	//	�΂̎��(ID)
		enum StoneType : char {
			None  = 0,
			Black = 1,
			White = 2,
		};

	//	�΂̕�����\�����镶���^
		using Char = char;

	//	���o�͂ɗp����΂̕���
		enum class StoneChar : Char {
			None  = '.',
			Black = 'X',
			White = 'O',
		};

		Stone();
		Stone(StoneType stone);
		explicit Stone(Char c);

	//	�΂�ID���擾
		size_t getID() const;

	//	������r
		bool operator ==(Stone stone) const;
		
	//	�s������r
		bool operator !=(Stone stone) const;

	//	�� stone �𔽓]����
		static Stone reverse(Stone stone);
		
	//	���]�����΂�Ԃ�
		Stone getReversed() const;

		friend std::istream& operator >>(std::istream& is, Stone& stone);
		friend std::ostream& operator <<(std::ostream& os, const Stone& stone);

	private:
	//	�΂�\�����镶����Ԃ�
		Char getChar() const;

		StoneType stone;
	};

//	���͊֐�
	std::istream& operator >>(std::istream& is, Stone& stone);

//	�o�͊֐�
	std::ostream& operator <<(std::ostream& os, const Stone& stone);

}