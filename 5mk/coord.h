#pragma once
namespace F19 {
	
//	座標を表現する型
	using CoordType = __int32;

//	X座標
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
	
//	Y座標
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

//	位置型
	class Pos {
	public:
	//	ハッシュ関数クラス
		struct Hash;

		Pos();
		Pos(X x, Y y);

		bool operator ==(const Pos& pos) const;
		bool operator !=(const Pos& pos) const;

		X x;	//	X座標
		Y y;	//	Y座標
	};
	
//	ハッシュ関数クラス
	struct Pos::Hash {
		using result_type = __int64;
	//	ハッシュ関数
		__int64 operator()(const Pos& pos) const;
	};

}