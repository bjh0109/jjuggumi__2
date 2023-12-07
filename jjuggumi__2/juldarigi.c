#include <stdio.h>
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define DIR_LEFT	0
#define DIR_RIGHT	1
void sample_initj(void);
void str_setting(void);
void jul_pass(void);
void move_left(void);
void move_right(void);
void move_tail_jul(int, int, int);
void move_tail_line(int, int, int);
void jul_false(int);
void right_lying(void);
void left_lying(void);
int px[PLAYER_MAX], py[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기
int lx[3], ly[3];
char line[3] = { '-','-','-' };
double str_1 = 0; //오른쪽(홀수애들) 유효힘 합
double str_2 = 0; //왼쪽(짝수애들) 유효힘 합
double str_l = 0; //양수면 right 음수면 left
int left_lyi = 0; //왼쪽팀 눕기 확인 변수
int right_lyi = 0; //오른쪽팀 눕기 확인 변수
bool left_line_end;
void str_setting(void) {
	str_l = 0;
	str_1 = 0;
	str_2 = 0;
	for (int i = 0; i < n_player; i++) {
		if ((player[i].is_alive == true && jul_fal[i] == false) || (player[i].is_alive == true && jul_fal[i] == true)) {
			if (i % 2 == 1) {
				str_1 = str_1 + player[i].str * (player[i].stamina / 100);
			}
			else {
				str_2 = str_2 + player[i].str * (player[i].stamina / 100);
			}
		}
	}
	str_l = -str_2 + str_1;
}
void sample_initj(void) {
	map_init(3, 41);
	for (int i = 0; i < n_player; i++) {
		jul_fal[i] = false;
	}
	//player[1].is_alive = false; test
	for (int i = 0; i < n_player; i++) {
		int x = 1; // 모든 플레이어를 1번 행에 위치시킵니다.
		int y;
		if (player[i].is_alive == false) {
			player[i].str = 0;
			player[i].intel = 0;
			jul_fal[i] = true;
		}
		if (i % 2 == 0) { // 짝수번째 플레이어
			y = N_COL / 2 - 2 - i / 2;
		}
		else { // 홀수번째 플레이어
			y = N_COL / 2 + 2 + i / 2;
		}
		px[i] = x;
		py[i] = y;
		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
	}
	//줄 위치
	int line_cnt = -1;
	for (int i = 0; i < 3; i++) {
		lx[i] = 1;
		ly[i] = 20 + line_cnt;
		back_buf[lx[i]][ly[i]] = line[i];
		line_cnt++;
	}
	//낭떠러지
	back_buf[0][20] = ' ';
	back_buf[2][20] = ' ';
	tick = 0;
}

//왼쪽 or 오른쪽으로 move하는 함수
void move_left() {
	int nx, ny;
	//먼저 왼쪽으로 가니까 맨왼쪽에 있는 플레이어부터 움직임. 즉 n_player = 7이면 0123456이니까 6부터 n_player - 1부터. 근데 n_player = 8이면 01234567 n_player - 2
	if (n_player % 2 == 0) { //짝수면 for문에서 -2
		for (int i = n_player - 2; i >= 0; i -= 2) {
			nx = px[i] + 0;
			ny = py[i] - 1;
			if (placable_jul(ny) == false) {
				break;
			}
			move_tail_jul(i, nx, ny);
		}
	}
	else if (n_player % 2 == 1) {//홀수면 for문에서 -1
		for (int i = n_player - 1; i >= 0; i -= 2) {
			nx = px[i] + 0;
			ny = py[i] - 1;
			if (placable_jul(ny) == false) {
				break;
			}
			move_tail_jul(i, nx, ny);
		}
	}
	//그다음 줄을 움직임
	for (int i = 0; i < 3; i++) {
		nx = lx[i] + 0;
		ny = ly[i] - 1;
		if (placable(nx, ny) == false) {
			break;
		}
		move_tail_line(i, nx, ny);
	}
	// 그다음으로 오른쪽에 있는 플레이어를 1부터움직임. n_player = 7 0123456 
	for (int i = 1; i < n_player; i += 2) {
		nx = px[i] + 0;
		ny = py[i] - 1;
		if (ny == 20) {
			jul_false(i);
			gotoxy(N_ROW + 1, 0); //임시 탈락자 출력
			printf("%d번", i);
			continue;
		}
		move_tail_jul(i, nx, ny);
	}
}

void move_right() {
	int nx, ny;
	//먼저 오른쪽으로 가니까 맨오른쪽에 있는 플레이어부터 움직임. n_player = 8(01234567) n_player - 1; n_player = 7(0123456) -2
	if (n_player % 2 == 0) { //짝수면 - 1
		for (int i = n_player - 1; i > 0; i -= 2) {
			nx = px[i] + 0;
			ny = py[i] + 1;
			if (placable_jul(ny) == false) {
				break;
			}
			move_tail_jul(i, nx, ny);
		}
	}
	else if (n_player % 2 == 1) { //홀수면 - 2
		for (int i = n_player - 2; i > 0; i -= 2) {
			nx = px[i] + 0;
			ny = py[i] + 1;
			if (placable_jul(ny) == false) {
				break;

				move_tail_jul(i, nx, ny);
			}
		}
	}
	// 그다음 줄('-')을 오른쪽 라인 (line[2])부터 오른쪽으로 움직입니다.
	for (int i = 2; i >= 0; i--) {
		nx = lx[i] + 0;
		ny = ly[i] + 1;
		if (placable(nx, ny) == false) {
			break;
		}
		move_tail_line(i, nx, ny);
	}
	// 그다음으로 왼쪽에 있는 플레이어를 0부터 움직인다. 예시(6420 이니까 0 -> 2 -> 4 -> 6 순) 7일떄(0,2,4,6,8) 8이면 (0,1,2,3,4,5,6,7) 9이면 (0,1,2,3,4,5,6,7,8)
	for (int i = 0; i < n_player; i += 2) {
		nx = px[i] + 0;
		ny = py[i] + 1;
		if (ny == 20) {
			jul_false(i);
			gotoxy(N_ROW + 1, 0);
			printf("%d번", i);
			continue;
		}
		move_tail_jul(i, nx, ny);
	}
}

// back_buf[][]에 기록
void move_tail_jul(int player, int nx, int ny) {
	int p = player;  // 이름이 길어서...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void move_tail_line(int line, int nx, int ny) {
	int l = line;
	back_buf[nx][ny] = back_buf[lx[l]][ly[l]];
	back_buf[lx[l]][ly[l]] = ' ';
	lx[l] = nx;
	ly[l] = ny;
}
//키셋팅


void juldarigi(void) {
	sample_initj();
	system("cls");
	display_1();
	str_setting();
	while (1) {
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key == K_Z) {
			str_l -= 1;
		}
		else if (key == K_S) {
			str_l += 1;
		}
		else if (key == K_X) {
			left_lying();
			gotoxy(N_ROW, 0);
			printf("왼쪽팀이 눕기를 사용했습니다!!");
			str_l = -str_2 + str_1;
		}
		else if (key == K_D) {
			right_lying();
			gotoxy(N_ROW, 0);
			printf("오른쪽팀이 눕기를 사용했습니다!!");
			str_l = -str_2 + str_1;
		}
		gotoxy(N_ROW + 2, 0);
		printf("str = %.2f\n", str_l);
		printf("tick = %d", tick);
		display_1();
		Sleep(100);
		tick += 10;
		if (tick % 500 == 0) {
			if (str_l < 0) {
				if (left_lyi == 1) {
					move_left();
					move_left();
					left_lyi = 0;
				}
				else {
					move_left();
				}
				str_setting();
			}
			else if (str_l > 0) {
				if (right_lyi == 1) {
					move_right();
					move_right();
					right_lyi = 0;
				}
				else {
					move_right();
				}
				str_setting();
			}
			else {
				str_setting();
			}
		}
		if (tick == 4000) { //테스트로 30초동안 게임진행뒤 30초동안 남아있는 플레이어 통과
			jul_pass();
			break;
		}
	}
}
//눕기함수
void right_lying(void) {
	for (int i = 0; i < n_player; i++) {
		if (i % 2 == 1) {
			if (player[i].stamina < 30) {
				player[i].stamina = 0;
			}
			else {
				player[i].stamina -= 30;
			}
			right_lyi = 1;
		}
	}
	str_1 *= 2;
}
void left_lying(void) {
	for (int i = 0; i < n_player; i++) {
		if (i % 2 == 0) {
			if (player[i].stamina < 30) {
				player[i].stamina = 0;
			}
			else {
				player[i].stamina -= 30;
			}
			left_lyi = 1;
		}
	}
	str_2 *= 2;
}

//통과하는함수
void jul_pass(void) {
	for (int i = 0; i < n_player; i++) {
		if (player[i].is_alive == false || jul_fal[i] == true) {
			player[i].is_alive = true;
			jul_fal[i] = false;
			player[i].stamina = 100;
		}
	}
}
//탈락하는함수 (위에 이미탈락한플레이어랑 구분해야됨)
void jul_false(int i) {
	//다음게임에서는 is_alive == true && jul_fal[i] == false인 애들만 참가가능
	if (player[i].is_alive == true && jul_fal[i] == false) {
		player[i].hasitem = false;
		player[i].str /= 50;
		player[i].intel /= 50;
	}
	else if (player[i].is_alive == false || jul_fal[i] == true) {
		jul_fal[i] = false;
	}
}