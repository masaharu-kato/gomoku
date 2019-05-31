#include "pattern.h"
using namespace Kato;


Pattern::Pattern(GetRelStoneFunc get_rel_stone, Position&& p, Position&& diff) {
}

Pattern::RawType Pattern::getPattern(GetRelStoneFunc get_rel_stone, Position&& p, Position&& diff) {

	using DIndex = int;

	DIndex pa_beg = 0;	//	����F�p�^�[���J�n�ʒu
	DIndex pa_end = 0;	//	����F�p�^�[���I���ʒu
	
	DIndex me_beg = 0;	//	���F�p�^�[���J�n�ʒu
	DIndex me_end = 0;	//	���F�p�^�[���I���ʒu
	
	DIndex di = 0;	//	p����Ƃ���diff�����Ɋւ��Ă̈ʒu

//	�p�^�[���̊J�n�E�I���l�����܂�܂Ŏ��s (�������A���g��5�܂łɐ�������)
	while((!pa_beg || !pa_end)) {

	//	��������
		if(!pa_beg) {
		//	���ΓI�Ȑ΂̐F���擾
			Stone::RelKind relk = get_rel_stone(p - di * diff);
			if(relk == -1) pa_beg = -di;	//	���݈ʒu���ǂ������͑���F�̐�
			if(relk ==  1) me_beg = -di;	//	���݈ʒu�����F�̐�
		}
		
	//	��������
		if(!pa_end) {
		//	���ΓI�Ȑ΂̐F���擾
			Stone::RelKind relk = get_rel_stone(p + di * diff);
			if(relk == -1) pa_end = +di;	//	���݈ʒu���ǂ������͑���F�̐�
			if(relk ==  1) me_end = +di;	//	���݈ʒu�����F�̐�
		}
		
		if(!pa_beg) {
			if(!pa_end) {
			//	�����Ƃ��I����Ă��Ȃ�
				if(di >= 2) break;

			}
		}

	//	�ʒu��i�߂�
		di++;
	}

	DIndex pat_length = pa_end - pa_beg;

//	�p�^�[���̒�����5�����̏ꍇ�A�΂�u���Ă��ܖڂ����Ȃ��̂Ńp�^�[��30�Ƃ��Ĉ���
	if(pat_length < 5) return 0x30;

//	�p�^�[���̒�����5���傫���ꍇ�A��O
	if(pat_length > 5) throw "[Error] Pattern::getPattern(): pat_length is more than 5.";




}


/*
	@:���ꂩ�玩���̐΂�u���ꏊ
	��:�����̐�
	��:����̐�
	��:��
	��:����̐�/��
	?:�����̐�/�����Ȃ�
	.:�����Ȃ�
*/

//	
//	 +10: ���[�� �� (��: 17: ������ ��   )
//	 +20: �E�[�� �� (��: 27:  ���� �� �� )
//	(+30: ���[�� �� (��: 37: ������ �� �� ))

std::ostream& Pattern::outBasicPatternVisual(std::ostream& os) const {
	
	if(pattern & 0x10) os << "��";

	switch(pattern & 0xF) {
	case 0x0: return os << "     ";	
	case 0x1: return os << "��    ";
	case 0x2: return os << "����	 ";
	case 0x3: return os << "�� ��	 ";
	case 0x4: return os << "������  ";
	case 0x5: return os << "��  �� ";
	case 0x6: return os << "�� ���� ";
	case 0x7: return os << "���� �� ";
	case 0x8: return os << "�������� ";
	case 0x9: return os << "��   ��";
	case 0xA: return os << "��  ����";
	case 0xB: return os << "����  ��";
	case 0xC: return os << "�� ������";
	case 0xD: return os << "������ ��";
	case 0xE: return os << "���� ����";
	case 0xF: return os << "�� �� ��";
	};
	
	if(pattern & 0x20) os << "��";

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
