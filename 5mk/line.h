#pragma once
#include "position.h"
#include "pattern.h"
using namespace Kato;

class Line {
private:
	Position beg;
	Position diff;
	size_t size;
public:
	Line(Position&& beg, Position&& diff, size_t size) : beg(beg), diff(diff), size(size) {}

	operator Pattern() const;

};
