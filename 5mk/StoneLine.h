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

		using Steps = int;
		constexpr static Steps Steps_Infinity = 99; // = std::numeric_limits<Steps>::max();

		static Map instances;
		static Ptr instance(const Base&);

		Ptr getWith(size_t index, Stone stone) const;
		Ptr getReversed() const;

	//	target�̐΂�5��(5�A��)���`�����Ă��邩�ǂ���
		bool is5mk(Stone target) const;

	//	target�̐΂���������܂ł̍ŏ��X�e�b�v��
		Steps win_steps(Stone target) const;

	//	target�̐΂̏������j�~�����(target�̐΂�5�ڂ����Ȃ��Ȃ�)�܂ł̍ŏ��X�e�b�v��
		Steps blocked_steps(Stone target) const;
		
	//	�P�\�X�e�b�v��
		Steps grace_steps(Stone target) const;

	//	�e�ʒu�̗P�\�X�e�b�v��(���̈ʒu��target�̐΂�u�������̗P�\�X�e�b�v��)
		Steps grace_steps(Stone target, size_t index) const;

	//	������r(�n�b�V���}�b�v�ŗ��p)
		bool operator==(const StoneLine&) const;
		bool operator!=(const StoneLine&) const;

	//	�f�o�b�O�p
		static void out_instances(std::ostream& os);

	private:
		StoneLine(const Base&);

	//	win_steps ���v�Z����֐�
		Steps calc_win_steps(Stone target) const;

	//	blocked_steps ���v�Z����֐�
		Steps calc_blocked_steps(Stone target) const;

	//	�e�ʒu�ɂ����� grace_steps ���v�Z����֐�
		void calc_grace_steps(Stone target) const;


		void out_graces(Stone target) const;


		mutable std::unique_ptr<Steps> _win_steps[2];
		mutable std::unique_ptr<Steps> _blocked_steps[2];
		mutable std::vector<Steps> _grace_steps[2];

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