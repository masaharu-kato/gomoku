#include "coord.h"
using namespace F19;

X::X() = default;

X::X(CoordType c)
	: c(c) {}

X::operator CoordType() const{
	return c;
}

X X::operator +(X x) const {
	return c + x;
}

X& X::operator +=(X x) {
	c += x;
	return *this;
}

X& X::operator ++() {
	++c;
	return *this;
}

X& X::operator ++(int) {
	c++;
	return *this;
}

X X::operator-() const {
	return -c;
}

X X::operator -(X x) const {
	return c - x;
}

X& X::operator -=(X x) {
	c -= x;
	return *this;
}

X& X::operator --() {
	--c;
	return *this;
}

X& X::operator --(int) {
	c--;
	return *this;
}



Y::Y() = default;

Y::Y(CoordType c)
	: c(c) {}

Y::operator CoordType() const{
	return c;
}

Y Y::operator +(Y Y) const {
	return c + Y;
}

Y& Y::operator +=(Y Y) {
	c += Y;
	return *this;
}

Y& Y::operator ++() {
	++c;
	return *this;
}

Y& Y::operator ++(int) {
	c++;
	return *this;
}

Y Y::operator-() const {
	return -c;
}

Y Y::operator -(Y Y) const {
	return c - Y;
}

Y& Y::operator -=(Y Y) {
	c -= Y;
	return *this;
}

Y& Y::operator --() {
	--c;
	return *this;
}

Y& Y::operator --(int) {
	c--;
	return *this;
}


Pos::Pos() = default;
Pos::Pos(X x, Y y)
	: x(x), y(y) {}

bool Pos::operator ==(const Pos& pos) const {
	return x == pos.x && y == pos.y;
}

bool Pos::operator !=(const Pos& pos) const {
	return !operator==(pos);
}

__int64 Pos::Hash::operator ()(const Pos& pos) const {
	return ((__int64)pos.x << 32) + pos.y;
}