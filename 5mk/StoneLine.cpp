#include "StoneLine.h"
#include <stdexcept>

F19::StoneLine::StoneLine(const Base& base) : Base(base) {

}

auto F19::StoneLine::value(Stone target) const -> Value {
	
	auto& cache = _value[target.getID() - 1];
	if(cache) return *cache;

	auto& nexts = _next_values[target.getID() - 1];
	nexts.resize(10);

//	std::cerr << "calc_value: " << (Base)*this << "|\n";

	Stone astone = target;
	Stone estone = Stone::reverse(target);

//	自分の石で5目を形成していれば 1, 相手の石で5目を形成していれば -1 を返す
	if(is5mk(astone)) return 1;
	if(is5mk(estone)) return -1;

//	自分の石を置いた時の評価値の合計
	Value sum_aval = 0;	

//	相手の石を置いた時の最小値
	Value max_eval = 0;
//	Value sum_eval = 0;

//	空いている位置の数カウンタ
	size_t n_none = 0;

//	それぞれの自分の石を置いた時の評価
	std::vector<Value> avals(size(), 0.0);

	for(size_t i = 0; i < size(); i++) {

	//	空いてる位置それぞれを処理
		if(at(i) == Stone::None) {

		//	自分の石を置いた時の評価を計算
			auto c_aval = getWith(i, astone)->value(astone);
			avals[i] = c_aval;

		//	自分の石を置いた時の評価を計算し、合計に加算
			sum_aval += c_aval;

		//	相手の石を置いた時の評価を計算し、最小値を更新
			auto c_eval = getWith(i, estone)->value(estone);
			if(!n_none || max_eval < c_eval) max_eval = c_eval;
		//	sum_eval += getWith(i, estone)->value(target);

			n_none++;
		}

	}
	
//	自分の石を置いた時の評価値の平均

//	評価値
	Value value = n_none ? (sum_aval / n_none - max_eval) : 0;
	
//	評価値を保存
	cache.reset(new Value(value));
	
	for(size_t i = 0; i < size(); i++) {
		nexts[i] = - getWith(i, astone)->value(astone) + value;
	}

	return value;
}
//
//
////	すべての次の評価値を計算する関数
//void F19::StoneLine::calc_next_values(Stone target) const {
//	
//	auto& values = _next_values[target.getID() - 1];
//	values.resize(size());
//
//	for(size_t i = 0; i < size(); i++) {
//		if(at(i) == Stone::None) {
//			values[i] = getWith(i, target)->value(target) - value(Stone::reverse(target)) - value(target);
//		}else{
//			values[i] = 0;
//		}
//	}
//
//}


auto F19::StoneLine::next_value(Stone target, Index index) const -> Value {
	auto& values = _next_values[target.getID() - 1];
	if(!values.size()) value(target);
	return values[index];
}


bool F19::StoneLine::operator==(const StoneLine& _line) const {

	if(size() != _line.size()) return false;

	for(size_t i = 0; i < size(); i++) {
		if(at(i) != _line.at(i)) return false;
	}

	return true;

}

bool F19::StoneLine::operator!=(const StoneLine& _line) const {
	return !operator ==(_line);
}


auto F19::StoneLine::instance(const Base& base) -> Ptr {
	if(instances.count(base)) return instances.at(base);
	auto ptr = std::shared_ptr<StoneLine>(new StoneLine(base));
	instances.insert_or_assign(base, ptr);

//#if _DEBUG
//	if(instances.size() % 10000 == 0) std::cerr << instances.size() << "\n";
//#endif

	return ptr;
}

bool F19::StoneLine::is5mk(Stone target) const{
	int n_con = 0;

	for(auto stone : *this) {
		if(stone == target) {
			n_con++;
		}
		else{
			n_con = 0;
		}
		if(n_con == 5) return true;
	}

	return false;
}

auto F19::StoneLine::getWith(Index index, Stone stone) const -> Ptr {

	if(index >= size()) throw std::exception("Index out of range.");

	Base stone_line = *this;
	stone_line[index] = stone;

	return instance(stone_line);

}

auto F19::StoneLine::getReversed() const -> Ptr {

	Base reversed;

	for(auto stone : *this){
		if(stone == Stone::None) {
			reversed.push_back(Stone::None);
		}
		else if(stone == Stone::Black){
			reversed.push_back(Stone::White);
		}
		else if(stone == Stone::White){
			reversed.push_back(Stone::Black);
		}
		else{
			throw new std::exception("Invalid stone type.");
		}
	}

	return instance(reversed);

}

std::istream& F19::operator >>(std::istream& is, StoneLine& stone_line) {
	std::string str;
	is >> str;
	for(size_t i = 0; i < str.size(); i++) {
		stone_line[i] = (Stone::StoneType)str[i];
	}
	return is;
}

std::ostream& F19::operator <<(std::ostream& os, const StoneLine& stone_line) {
	for(auto stone : stone_line) {
		os << stone.getChar();
	}
	return os;
}

size_t F19::StoneLine::Hash::operator ()(const Base& stone_line) const {
	
	size_t hash = 0;

	for(auto stone : stone_line) {
		hash = hash * 4 + stone.getID();
	}

	return hash;

}

void F19::StoneLine::out_instances(std::ostream& os) {
	for(const auto& i : StoneLine::instances) {
		std::cout << i.first
			<< "\tB:" << i.second->value(Stone::Black)
			<< "\tW:" << i.second->value(Stone::White)
			<< "\n";
	}
}

F19::StoneLine::Map F19::StoneLine::instances;