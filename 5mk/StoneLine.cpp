#include "StoneLine.h"
#include <stdexcept>

F19::StoneLine::StoneLine(const Base& base) : Base(base) {

}

auto F19::StoneLine::calc_win_steps(Stone target) const -> Steps {

	if(is5mk(target)) return 0;

	Steps min_steps = Steps_Infinity;

	for(size_t i = 0; i < size(); i++) {
		if(at(i) == Stone::None) {
			auto c_steps = getWith(i, target)->win_steps(target) + 1;
			if(min_steps > c_steps) min_steps = c_steps;
		}
	}

	return min_steps;

}

auto F19::StoneLine::calc_blocked_steps(Stone target) const -> Steps {

	if(win_steps(target) == Steps_Infinity) return 0;

	Steps min_steps = Steps_Infinity;
	auto rev_target = Stone::reverse(target);

	for(size_t i = 0; i < size(); i++) {
		if(at(i) == Stone::None) {
			auto c_steps = getWith(i, rev_target)->blocked_steps(target) + 1;
			if(min_steps > c_steps) min_steps = c_steps;
		}
	}

	return min_steps;

}

auto F19::StoneLine::win_steps(Stone target) const -> Steps {
	size_t index = target.getID() - 1;
	if(!_win_steps[index]){
		_win_steps[index].reset(new Steps(calc_win_steps(target)));
	}
	return *_win_steps[index];
}

auto F19::StoneLine::blocked_steps(Stone target) const -> Steps {
	size_t index = target.getID() - 1;
	if(!_blocked_steps[index]){
		_blocked_steps[index].reset(new Steps(calc_blocked_steps(target)));
	}
	return *_blocked_steps[index];
}

auto F19::StoneLine::grace_steps(Stone target) const -> Steps {
	auto rev_target = Stone::reverse(target);
	return win_steps(rev_target) - blocked_steps(rev_target);
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

auto F19::StoneLine::getWith(size_t index, Stone stone) const -> Ptr {

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
		std::cout << stone.getChar();
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
			<< "\tBw:" << i.second->win_steps    (Stone::Black)
			<< "\tBb:" << i.second->blocked_steps(Stone::Black)
			<< "\tWw:" << i.second->win_steps    (Stone::White)
			<< "\tWb:" << i.second->blocked_steps(Stone::White)
			<< "\n";
	}
}


F19::StoneLine::Map F19::StoneLine::instances;