#include "groups.h"


//=======================================================
// Group 6
//=======================================================

void strategy6(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int i, j, k, cnt = 0, _cnt;
	int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int point[BOARD_SIZE][BOARD_SIZE] = { 0 };

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] != STONE_BLACK && board[i][j] != STONE_WHITE) cnt++;
		}
	}
	if (cnt == BOARD_SIZE * BOARD_SIZE) {
		*pos_x = BOARD_SIZE / 2 - (rand() % 2);
		*pos_y = BOARD_SIZE / 2 - (rand() % 2);
	}

	else {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == STONE_SPACE) {
					int a = 0, b = 0;
					if (i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1) point[i][j] -= 100;
					if (!(i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1)) {
						_cnt = 0;
						for (k = 0; k < 8; k++) {
							if (board[i + dx[k]][j + dy[k]] != STONE_SPACE) _cnt++;
						}
						if (_cnt >= 6) point[i][j] -= 100;
					}
					for (k = 0; k < 8; k++) {
						if (i + dx[k] >= 0 && j + dy[k] >= 0 && i + dx[k] < BOARD_SIZE && j + dy[k] < BOARD_SIZE) { //1ŒÂ—×‚¾‚¯Œ©‚½‚Æ‚«
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK) {
									point[i][j] += 10;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 100;
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 1;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE) {
									point[i][j] += 20;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 50;
										else if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 1;
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE) {
									point[i][j] += 10;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 100;
										else if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 1;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK) {
									point[i][j] += 20;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 50;
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 1;
									}
								}
							}
						}
						if ((i + 2 * dx[k] >= 0 && j + 2 * dy[k] >= 0) && (i + 2 * dx[k] < BOARD_SIZE && j + 2 * dy[k] < BOARD_SIZE)) { //2ŒÂ—×‚Ü‚ÅŒ©‚½‚Æ‚«
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 5;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 10;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 10000;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 30;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 10;
											}
										}
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 80;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 50;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 200;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 1300;
											}
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] -= 40;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 5;
											if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 10;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 50;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) {
											point[i][j] += 50;
											a++;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 100;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) {
											point[i][j] += 100;
											b++;
										}
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 5;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 10;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 10000;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 30;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 10;
											}
										}
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 80;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 50;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 200;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 1300;
											}
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] -= 40;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 5;
											if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 10;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 50;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) {
											point[i][j] += 50;
											a++;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 100;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) {
											point[i][j] += 100;
											b++;
										}
									}
								}
							}
						}
						if ((i + 3 * dx[k] >= 0 && j + 3 * dy[k] >= 0) && (i + 3 * dx[k] < BOARD_SIZE && j + 3 * dy[k] < BOARD_SIZE)) { //3ŒÂ—×‚Ü‚ÅŒ©‚½‚Æ‚«
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									point[i][j] += 100;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 500; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 200;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									point[i][j] += 130;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 700; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
									}
								}
								int l, cnt1 = 0, cnt2 = 0;
								for (l = 1; l <= 3; l++) {
									if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
										cnt1++;
										cnt2 -= 2;
									}
									else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
										cnt1 -= 2;
										cnt2++;
									}
								}
								if (cnt1 >= 2) {
									a++;
									point[i][j] += a * 120;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK)
										{
											point[i][j] += 20; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
											}
										}
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 10; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_WHITE) point[i][j] -= 100;
									}
								}
								if (cnt2 >= 2) {
									b++;
									b += a;
									point[i][j] += b * 140;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 5; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 200; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
										}
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_BLACK) point[i][j] -= 30;
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									point[i][j] += 100;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 500; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
										else if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 200;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									point[i][j] += 130;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 700; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
									}
								}
								int l, cnt1 = 0, cnt2 = 0;
								for (l = 1; l <= 3; l++) {
									if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
										cnt1++;
										cnt2 -= 2;
									}
									else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
										cnt1 -= 2;
										cnt2++;
									}
								}
								if (cnt1 >= 2) {
									a++;
									point[i][j] += a * 120;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 20; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
											}
										}
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 10; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_BLACK) point[i][j] -= 100;
									}
								}
								if (cnt2 >= 2) {
									b++;
									b += a;
									point[i][j] += b * 140;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 5; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 200; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
										}
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_WHITE) point[i][j] -= 30;
									}
								}
							}
						}
						if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) { //4ŒÂ—×‚Ü‚ÅŒ©‚½‚Æ‚«
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_BLACK) {
										point[i][j] += 10000;
										if ((i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1) && (i + 4 * dx[k] == 0 || i + 4 * dx[k] == BOARD_SIZE - 1 || j + 4 * dy[k] == 0 || j + 4 * dy[k] == BOARD_SIZE - 1)) point[i][j] -= 10000;
									}
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_WHITE) point[i][j] += 2000;
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 700;
								}
								if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_WHITE) point[i][j] -= 70;
								else {
									int l, cnt1 = 0, cnt2 = 0;
									for (l = 1; l <= 4; l++) {
										if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
											cnt1++;
											cnt2 -= 2;
										}
										else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
											cnt1 -= 2;
											cnt2++;
										}
									}
									if (cnt1 >= 3) {
										point[i][j] += 90;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
												point[i][j] += 20; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
												if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
													if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
												}
											}
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 10; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
										}
									}
									if (cnt2 >= 3) {
										point[i][j] += 150;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 20; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 20; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
										}
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_WHITE) {
										point[i][j] += 10000;
										if ((i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1) && (i + 4 * dx[k] == 0 || i + 4 * dx[k] == BOARD_SIZE - 1 || j + 4 * dy[k] == 0 || j + 4 * dy[k] == BOARD_SIZE - 1)) point[i][j] -= 10000;
									}
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_BLACK) point[i][j] += 2000;
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 700;
								}
								if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_BLACK) point[i][j] -= 70;
								else {
									int l, cnt1 = 0, cnt2 = 0;
									for (l = 1; l <= 4; l++) {
										if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
											cnt1++;
											cnt2 -= 2;
										}
										else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
											cnt1 -= 2;
											cnt2++;
										}
									}
									if (cnt1 >= 3) {
										point[i][j] += 90;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
												point[i][j] += 20; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
												if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
													if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
												}
											}
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 10; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
										}
									}
									if (cnt2 >= 3) {
										point[i][j] += 150;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 20; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 20; //”½‘Î‘¤‚É1ŒÂ‚ ‚Á‚½Žž
										}
									}
								}
							}
						}
					}
				}
			}
		}
		int p_max = -10000;
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (p_max < point[i][j] && board[i][j] != STONE_BLACK && board[i][j] != STONE_WHITE) p_max = point[i][j];
			}
		}

		int b_flag = 0;
		while (1) {
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					if (point[i][j] == p_max && board[i][j] != STONE_BLACK && board[i][j] != STONE_WHITE) {
						*pos_x = j;
						*pos_y = i;
						if (rand() % 3 == 0) b_flag = 1;
					}
					if (b_flag == 1) break;
				}
				if (b_flag == 1) break;
			}
			if (b_flag == 1) break;
		}
	}

	return;
}
