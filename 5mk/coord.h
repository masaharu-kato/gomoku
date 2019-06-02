#pragma once
namespace F19 {
	
	using CoordType = __int32;

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

	class Pos {
	public:
		struct Hash;

		Pos();
		Pos(X x, Y y);

		bool operator ==(const Pos& pos) const;
		bool operator !=(const Pos& pos) const;

		X x;
		Y y;
	};
	
//	ÉnÉbÉVÉÖä÷êî
	struct Pos::Hash {
		using result_type = __int64;
		__int64 operator()(const Pos& pos) const;
	};

}