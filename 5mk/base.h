#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10      // �ՖʃT�C�Y 10 * 10
#define STONE_SPACE 0      // �Ֆʂɂ���� �Ȃ�
#define STONE_BLACK 1      // �Ֆʂɂ���� ��
#define STONE_WHITE 2      // �Ֆʂɂ���� ��

typedef struct{
	int x;
	int y;
} position;
