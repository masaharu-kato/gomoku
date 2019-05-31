#include "pattern.h"
using namespace Kato;


Pattern::Pattern(GetRelStoneFunc get_rel_stone, Position&& p, Position&& diff) {
}

Pattern::RawType Pattern::getPattern(GetRelStoneFunc get_rel_stone, Position&& p, Position&& diff) {

	using DIndex = int;

	DIndex pa_beg = 0;	//	相手色パターン開始位置
	DIndex pa_end = 0;	//	相手色パターン終了位置
	
	DIndex me_beg = 0;	//	自色パターン開始位置
	DIndex me_end = 0;	//	自色パターン終了位置
	
	DIndex di = 0;	//	pを基準としたdiff方向に関しての位置

//	パターンの開始・終了値が求まるまで実行 (ただし、中身は5つまでに制限する)
	while((!pa_beg || !pa_end)) {

	//	負方向側
		if(!pa_beg) {
		//	相対的な石の色を取得
			Stone::RelKind relk = get_rel_stone(p - di * diff);
			if(relk == -1) pa_beg = -di;	//	現在位置が壁もしくは相手色の石
			if(relk ==  1) me_beg = -di;	//	現在位置が自色の石
		}
		
	//	正方向側
		if(!pa_end) {
		//	相対的な石の色を取得
			Stone::RelKind relk = get_rel_stone(p + di * diff);
			if(relk == -1) pa_end = +di;	//	現在位置が壁もしくは相手色の石
			if(relk ==  1) me_end = +di;	//	現在位置が自色の石
		}
		
		if(!pa_beg) {
			if(!pa_end) {
			//	正負とも終わっていない
				if(di >= 2) break;

			}
		}

	//	位置を進める
		di++;
	}

	DIndex pat_length = pa_end - pa_beg;

//	パターンの長さが5未満の場合、石を置いても五目を作れないのでパターン30として扱う
	if(pat_length < 5) return 0x30;

//	パターンの長さが5より大きい場合、例外
	if(pat_length > 5) throw "[Error] Pattern::getPattern(): pat_length is more than 5.";




}


/*
	@:これから自分の石を置く場所
	○:自分の石
	●:相手の石
	□:壁
	■:相手の石/壁
	?:自分の石/何もなし
	.:何もなし
*/

//	
//	 +10: 左端が ■ (例: 17: ■○○ ○   )
//	 +20: 右端が ■ (例: 27:  ○○ ○ ■ )
//	(+30: 両端が ■ (例: 37: ■○○ ○ ■ ))

std::ostream& Pattern::outBasicPatternVisual(std::ostream& os) const {
	
	if(pattern & 0x10) os << "■";

	switch(pattern & 0xF) {
	case 0x0: return os << "     ";	
	case 0x1: return os << "○    ";
	case 0x2: return os << "○○	 ";
	case 0x3: return os << "○ ○	 ";
	case 0x4: return os << "○○○  ";
	case 0x5: return os << "○  ○ ";
	case 0x6: return os << "○ ○○ ";
	case 0x7: return os << "○○ ○ ";
	case 0x8: return os << "○○○○ ";
	case 0x9: return os << "○   ○";
	case 0xA: return os << "○  ○○";
	case 0xB: return os << "○○  ○";
	case 0xC: return os << "○ ○○○";
	case 0xD: return os << "○○○ ○";
	case 0xE: return os << "○○ ○○";
	case 0xF: return os << "○ ○ ○";
	};
	
	if(pattern & 0x20) os << "■";

	return os << "[Error]";
}


std::ostream& Pattern::outBasicPatternName(std::ostream& os) const {

	if(pattern & 0x10) os << "B";

	switch(pattern & 0xF) {
	case 0x0: return os << "N___";	
	case 0x1: return os << "W1__";
	case 0x2: return os << "W2__";
	case 0x3: return os << "W11_";
	case 0x4: return os << "W3__";
	case 0x5: return os << "W121";
	case 0x6: return os << "W12_";
	case 0x7: return os << "W21_";
	case 0x8: return os << "W4__";
	case 0x9: return os << "W131";
	case 0xA: return os << "W122";
	case 0xB: return os << "W221";
	case 0xC: return os << "W13_";
	case 0xD: return os << "W31_";
	case 0xE: return os << "W22_";
	case 0xF: return os << "W101";
	};
	
	if(pattern & 0x20) os << "B";

	return os << "[Error]";
}

std::ostream& Pattern::operator<<(std::ostream& os) const {

	outBasicPatternName(os);
	
	os << " [";
	outBasicPatternVisual(os);
	os << "]";

	return os;
}
