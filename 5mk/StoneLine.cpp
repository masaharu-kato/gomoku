#include "StoneLine.h"
#include <stdexcept>
#include <string>

//	�R���X�g���N�^
F19::StoneLine::StoneLine(const Base& base)
	: Base(base) {}

//	target �̐΂ɂ��Ă̕]���l���擾
auto F19::StoneLine::value(Stone target) const -> Value {
	
//	���łɌv�Z�����]���l������΂�����擾
	auto& cache = _value[target.getID() - 1];
	if(cache) return *cache;

//	�󂫈ʒu�ɐ΂�u�������̕]���l���L�^���邽�߂̔z���������
	auto& nexts = _next_values[target.getID() - 1];
	nexts.resize(size(), 0.0);

//	�����̐�
	Stone astone = target;
	
//	�G�̐�
	Stone estone = target.getReversed();

//	�]���l
	Value value = 0;

//	�����̐΂�5�ڂ��`�����Ă���� 1 
	if(is5mk(astone)){
		value = 1;
	}
//	����̐΂�5�ڂ��`�����Ă���� -1
	else if(is5mk(estone)){
		value = -1;
	}
	else{

	//	�����̐΂�u�������̕]���l�̍ő�l
		Value max_aval = 0;	

	//	����̐΂�u�������̕]���l�̍��v�l
		Value sum_eval = 0;

	//	�󂫈ʒu���̃J�E���^
		size_t n_empty = 0;

		for(size_t i = 0; i < size(); i++) {

		//	�󂢂Ă�ʒu���ꂼ�������
			if(at(i) == Stone::None) {

			//	�����̐΂�u�������̕]�����v�Z
				auto c_aval = getWith(i, astone)->value(astone);
				nexts[i] = c_aval;
				if(!n_empty || max_aval < c_aval) max_aval = c_aval;

			//	����̐΂�u�������̕]�����v�Z���A�ŏ��l���X�V
				auto c_eval = getWith(i, estone)->value(astone);
				sum_eval += c_eval;

				n_empty++;
			}

		}
	
	//	�]���l���v�Z�i�󂫈ʒu���Ȃ��ꍇ�� 0 �̂܂܁j
		if(n_empty) {
			value = miss_coef * max_aval + (1.0 - miss_coef) * (sum_eval / n_empty);
		}

	}
	
//	�]���l���L�^
	cache.reset(new Value(value));
	
	return value;
}

//	�ʒu index �ɐ� target ��u�������̕]���l���擾
auto F19::StoneLine::next_value(Stone target, Index index) const -> Value {
	auto& values = _next_values[target.getID() - 1];
	if(!values.size()) value(target);
	return values[index];
}

//	������r
bool F19::StoneLine::operator==(const StoneLine& _line) const {

	if(size() != _line.size()) return false;

	for(size_t i = 0; i < size(); i++) {
		if(at(i) != _line.at(i)) return false;
	}

	return true;

}

//	�s������r
bool F19::StoneLine::operator!=(const StoneLine& _line) const {
	return !operator ==(_line);
}

//	�΂̕��т���C���X�^���X���擾����
auto F19::StoneLine::instance(const Base& base) -> Ptr {
	if(instances.count(base)) return instances.at(base);
	auto ptr = std::shared_ptr<StoneLine>(new StoneLine(base));
	instances.insert_or_assign(base, ptr);

//#if _DEBUG
//	if(instances.size() % 10000 == 0) std::cerr << instances.size() << "\n";
//#endif

	return ptr;
}

//	target �̐΂�5�ڂ��`�����Ă��邩�Ԃ�
bool F19::StoneLine::is5mk(Stone target) const{

//	target �̐΂̘A����
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

//	�ʒu index �ɐ� stone ��u��������擾
auto F19::StoneLine::getWith(Index index, Stone stone) const -> Ptr {

	if(index < 0 || index >= size()) throw std::exception("Index out of range.");

	Base stone_line = *this;
	stone_line[index] = stone;

	return instance(stone_line);

}

//	�΂𔽓]����������擾
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

//	���͊֐�
std::istream& F19::operator >>(std::istream& is, StoneLine& stone_line) {
	std::string str;
	is >> str;
	for(size_t i = 0; i < str.size(); i++) {
		stone_line[i] = (Stone::StoneType)str[i];
	}
	return is;
}

//	�o�͊֐�
std::ostream& F19::operator <<(std::ostream& os, const StoneLine& stone_line) {
	for(auto stone : stone_line) {
		os << stone;
	}
	return os;
}

//	�n�b�V���֐��̎���
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