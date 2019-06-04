#pragma once
namespace F19 {
	
//	���W��\������^
	using CoordType = __int32;

//	X���W
	class X {
	public:
		X();
		X(CoordType c);
		operator CoordType() const;

		X operator +(X x) const;
		X& operator +=(X x);
		X& operator ++();
		X& operator ++(int);
		X operator -() const;
		X operator -(X x) const;
		X& operator -=(X x);
		X& operator --();
		X& operator --(int);

	private:
		CoordType c;
	};
	
//	Y���W
	class Y {
	public:
		Y();
		Y(CoordType c);
		operator CoordType() const;
		
		Y operator +(Y Y) const;
		Y& operator +=(Y Y);
		Y& operator ++();
		Y& operator ++(int);
		Y operator -() const;
		Y operator -(Y Y) const;
		Y& operator -=(Y Y);
		Y& operator --();
		Y& operator --(int);

	private:
		CoordType c;
	};

//	�ʒu�^
	class Pos {
	public:
	//	�n�b�V���֐��N���X
		struct Hash;

		Pos();
		Pos(X x, Y y);

		bool operator ==(const Pos& pos) const;
		bool operator !=(const Pos& pos) const;

		X x;	//	X���W
		Y y;	//	Y���W
	};
	
//	�n�b�V���֐��N���X
	struct Pos::Hash {
		using result_type = __int64;
	//	�n�b�V���֐�
		__int64 operator()(const Pos& pos) const;
	};

}