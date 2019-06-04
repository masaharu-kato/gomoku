#include "StoneLine.h"
#include <stdexcept>
#include <string>

//	コンストラクタ
F19::StoneLine::StoneLine(const Base& base)
	: Base(base) {}

//	target の石についての評価値を取得
auto F19::StoneLine::value(Stone target) const -> Value {
	
//	すでに計算した評価値があればそれを取得
	auto& cache = _value[target.getID() - 1];
	if(cache) return *cache;

//	空き位置に石を置いた時の評価値を記録するための配列を初期化
	auto& nexts = _next_values[target.getID() - 1];
	nexts.resize(size(), 0.0);

//	自分の石
	Stone astone = target;
	
//	敵の石
	Stone estone = target.getReversed();

//	評価値
	Value value = 0;

//	自分の石で5目を形成していれば 1 
	if(is5mk(astone)){
		value = 1;
	}
//	相手の石で5目を形成していれば -1
	else if(is5mk(estone)){
		value = -1;
	}
	else{

	//	自分の石を置いた時の評価値の最大値
		Value max_aval = 0;	

	//	相手の石を置いた時の評価値の合計値
		Value sum_eval = 0;

	//	空き位置数のカウンタ
		size_t n_empty = 0;

		for(size_t i = 0; i < size(); i++) {

		//	空いてる位置それぞれを処理
			if(at(i) == Stone::None) {

			//	自分の石を置いた時の評価を計算
				auto c_aval = getWith(i, astone)->value(astone);
				nexts[i] = c_aval;
				if(!n_empty || max_aval < c_aval) max_aval = c_aval;

			//	相手の石を置いた時の評価を計算し、最小値を更新
				auto c_eval = getWith(i, estone)->value(astone);
				sum_eval += c_eval;

				n_empty++;
			}

		}
	
	//	評価値を計算（空き位置がない場合は 0 のまま）
		if(n_empty) {
			value = miss_coef * max_aval + (1.0 - miss_coef) * (sum_eval / n_empty);
		}

	}
	
//	評価値を記録
	cache.reset(new Value(value));
	
	return value;
}

//	位置 index に石 target を置いた時の評価値を取得
auto F19::StoneLine::next_value(Stone target, Index index) const -> Value {
	auto& values = _next_values[target.getID() - 1];
	if(!values.size()) value(target);
	return values[index];
}

//	等価比較
bool F19::StoneLine::operator==(const StoneLine& _line) const {

	if(size() != _line.size()) return false;

	for(size_t i = 0; i < size(); i++) {
		if(at(i) != _line.at(i)) return false;
	}

	return true;

}

//	不等価比較
bool F19::StoneLine::operator!=(const StoneLine& _line) const {
	return !operator ==(_line);
}

//	石の並びからインスタンスを取得する
auto F19::StoneLine::instance(const Base& base) -> Ptr {
	if(instances.count(base)) return instances.at(base);
	auto ptr = std::shared_ptr<StoneLine>(new StoneLine(base));
	instances.insert_or_assign(base, ptr);

//#if _DEBUG
//	if(instances.size() % 10000 == 0) std::cerr << instances.size() << "\n";
//#endif

	return ptr;
}

//	target の石が5目を形成しているか返す
bool F19::StoneLine::is5mk(Stone target) const{

//	target の石の連続数
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

//	位置 index に石 stone を置いた列を取得
auto F19::StoneLine::getWith(Index index, Stone stone) const -> Ptr {

	if(index < 0 || index >= size()) throw std::exception("Index out of range.");

	Base stone_line = *this;
	stone_line[index] = stone;

	return instance(stone_line);

}

//	石を反転させた列を取得
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

//	入力関数
std::istream& F19::operator >>(std::istream& is, StoneLine& stone_line) {
	std::string str;
	is >> str;
	for(size_t i = 0; i < str.size(); i++) {
		stone_line[i] = (Stone::StoneType)str[i];
	}
	return is;
}

//	出力関数
std::ostream& F19::operator <<(std::ostream& os, const StoneLine& stone_line) {
	for(auto stone : stone_line) {
		os << stone;
	}
	return os;
}

//	ハッシュ関数の実装
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
double F19::StoneLine::miss_coef = 0.5;