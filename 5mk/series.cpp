#include "series.h"
using namespace Kato;

void Stone::Series::EachDir::pushStone(Stone::Kind kind) {

	if(data.size() && data.back().kind == kind){
		data.back().number++;
	}
	else {
		data.emplace_back(kind, 1);
	}

}
