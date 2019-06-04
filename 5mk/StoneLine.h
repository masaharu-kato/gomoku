#pragma once
#include "stone.h"
#include <vector>
#include <limits>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace F19 {

//	�΂̗�i���сj��\������N���X
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

	//	�]���l�̌^
		using Value = double;

	//	���ɍ쐬���ꂽ�C���X�^���X��ۑ�����}�b�v
		static Map instances;

	//	�΂̕��т���C���X�^���X���擾����i�Ȃ��ꍇ�͍쐬����j
		static Ptr instance(const Base&);

	//	�ʒu index (0���琔����) �ɐ� stone ��u��������擾
		Ptr getWith(Index index, Stone stone) const;

	//	�΂𔽓]����������擾
		Ptr getReversed() const;

	//	target�̐΂�5��(5�A��)���`�����Ă��邩�ǂ���
		bool is5mk(Stone target) const;

	//	target�̐΂ɂ��Ă̕]���l���擾
		Value value(Stone target) const;

	//	�ʒu index �ɐ� target ��u�������̕]���l���擾
		Value next_value(Stone target, Index index) const;

	////	target�̐΂��ʒuindex�ɒu�������̕]���l���猻�݂̕]���l���������l
	//	Value next_value_diff(Stone target, Index index) const;

	//	������r(�n�b�V���}�b�v�ŗ��p)
		bool operator==(const StoneLine&) const;
		
	//	�s������r(�n�b�V���}�b�v�ŗ��p)
		bool operator!=(const StoneLine&) const;

	//	�f�o�b�O�p
		static void out_instances(std::ostream& os);

	//	�����W��
		static double miss_coef;

	private:
	//	�R���X�g���N�^�i�΂̔z�񂩂� StoneLine �I�u�W�F�N�g���쐬�j
		StoneLine(const Base&);

	//	���ׂĂ̎��̕]���l���v�Z����֐�
	//	void calc_next_values(Stone target) const;

		mutable std::unique_ptr<Value> _value[2];
		mutable std::vector<Value> _next_values[2];

	};
		
//	���͊֐�
	std::istream& operator >>(std::istream& is, StoneLine& stone_line);
	
//	�o�͊֐�
	std::ostream& operator <<(std::ostream& os, const StoneLine& stone_line);

//	�n�b�V���֐��p�̃N���X
	struct StoneLine::Hash {
		using result_type = size_t;
	//	�n�b�V���֐�
		size_t operator()(const Base& stone_line) const;
	};

}