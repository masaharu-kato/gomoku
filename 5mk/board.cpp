#include "board.h"
using namespace Core;

/*
void Board::load(RawType data) {
	for(Position::Type px=0; px<SizeX; px++) {	
		for(Position::Type py=0; py<SizeY; py++) {
			calcOn({px, py});
		}
	}
}

void Board::calcOn(const Position& pos) {
	if(!getStone(pos)) calcSeries(pos);
}

void Board::calcSeries(const Position& pos){

	Stone::Series series;

	for(Direction dir=0; dir<N_directions; dir++) calcSeries(series.dirs[dir], pos, dir);

	series_map[pos] = series;
}

void Board::calcSeries(Stone::Series::Dir& series_dir, Position pos, Direction dir) {

	while(isInside(pos)) {
		series_dir.pushStone(getStone(pos));
		pos += directions[dir];
	}

}
*/

bool Board::isInside(const Position& pos) const {
	return pos.x >= 0 && pos.y >= 0 && pos.x < SizeX && pos.y < SizeY;
}