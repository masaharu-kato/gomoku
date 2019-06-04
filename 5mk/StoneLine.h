#pragma once
#include "stone.h"
#include <vector>
#include <limits>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace F19 {

//	�΂̕��т�\������N���X
	class StoneLine : public std::vector<Stone> {
	public:

	//	�n�b�V���֐��p�̃N���X
		struct Hash;

	//	���^(�΂̔z��)
		using Base = std::vector<Stone>;

	//	���̃N���X�p�̃|�C���^
		using Ptr = std::shared_ptr<StoneLine>;

	//	���̃N���X�̃C���X�^���X��ێ����邽�߂̃}�b�v
		using Map = std::unordered_map<Base, Ptr, Hash>;

		using Index = size_t;
		using Value = double;

		static Map instances;
		static Ptr instance(const Base&);

		Ptr getWith(Index index, Stone stone) const;
		Ptr getReversed() const;

	//	target�̐΂�5��(5�A��)���`�����Ă��邩�ǂ���
		bool is5mk(Stone target) const;

	//	target�̐΂̕]���l
		Value value(Stone target) const;

	//	target�̐΂��ʒuindex�ɒu�������̕]���l
		Value next_value(Stone target, Index index) const;

	////	target�̐΂��ʒuindex�ɒu�������̕]���l���猻�݂̕]���l���������l
	//	Value next_value_diff(Stone target, Index index) const;

	//	������r(�n�b�V���}�b�v�ŗ��p)
		bool operator==(const StoneLine&) const;
		bool operator!=(const StoneLine&) const;

	//	�f�o�b�O�p
		static void out_instances(std::ostream& os);

	//	�����W��
		static double miss_coef;

	private:
		StoneLine(const Base&);

	//	���ׂĂ̎��̕]���l���v�Z����֐�
	//	void calc_next_values(Stone target) const;

		mutable std::unique_ptr<Value> _value[2];
		mutable std::vector<Value> _next_values[2];

	};
		
//	�o�͊֐�
	std::istream& operator >>(std::istream& is, StoneLine& stone_line);

//	���͊֐�
	std::ostream& operator <<(std::ostream& os, const StoneLine& stone_line);

//	�n�b�V���֐�
	struct StoneLine::Hash {
		using result_type = size_t;
		size_t operator()(const Base& stone_line) const;
	};

}