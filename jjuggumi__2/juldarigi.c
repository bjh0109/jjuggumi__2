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
int px[PLAYER_MAX], py[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�
int lx[3], ly[3];
char line[3] = { '-','-','-' };
double str_1 = 0; //������(Ȧ���ֵ�) ��ȿ�� ��
double str_2 = 0; //����(¦���ֵ�) ��ȿ�� ��
double str_l = 0; //����� right ������ left
int left_lyi = 0; //������ ���� Ȯ�� ����
int right_lyi = 0; //�������� ���� Ȯ�� ����
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
		int x = 1; // ��� �÷��̾ 1�� �࿡ ��ġ��ŵ�ϴ�.
		int y;
		if (player[i].is_alive == false) {
			player[i].str = 0;
			player[i].intel = 0;
			jul_fal[i] = true;
		}
		if (i % 2 == 0) { // ¦����° �÷��̾�
			y = N_COL / 2 - 2 - i / 2;
		}
		else { // Ȧ����° �÷��̾�
			y = N_COL / 2 + 2 + i / 2;
		}
		px[i] = x;
		py[i] = y;
		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
	}
	//�� ��ġ
	int line_cnt = -1;
	for (int i = 0; i < 3; i++) {
		lx[i] = 1;
		ly[i] = 20 + line_cnt;
		back_buf[lx[i]][ly[i]] = line[i];
		line_cnt++;
	}
	//��������
	back_buf[0][20] = ' ';
	back_buf[2][20] = ' ';
	tick = 0;
}

//���� or ���������� move�ϴ� �Լ�
void move_left() {
	int nx, ny;
	//���� �������� ���ϱ� �ǿ��ʿ� �ִ� �÷��̾���� ������. �� n_player = 7�̸� 0123456�̴ϱ� 6���� n_player - 1����. �ٵ� n_player = 8�̸� 01234567 n_player - 2
	if (n_player % 2 == 0) { //¦���� for������ -2
		for (int i = n_player - 2; i >= 0; i -= 2) {
			nx = px[i] + 0;
			ny = py[i] - 1;
			if (placable_jul(ny) == false) {
				break;
			}
			move_tail_jul(i, nx, ny);
		}
	}
	else if (n_player % 2 == 1) {//Ȧ���� for������ -1
		for (int i = n_player - 1; i >= 0; i -= 2) {
			nx = px[i] + 0;
			ny = py[i] - 1;
			if (placable_jul(ny) == false) {
				break;
			}
			move_tail_jul(i, nx, ny);
		}
	}
	//�״��� ���� ������
	for (int i = 0; i < 3; i++) {
		nx = lx[i] + 0;
		ny = ly[i] - 1;
		if (placable(nx, ny) == false) {
			break;
		}
		move_tail_line(i, nx, ny);
	}
	// �״������� �����ʿ� �ִ� �÷��̾ 1���Ϳ�����. n_player = 7 0123456 
	for (int i = 1; i < n_player; i += 2) {
		nx = px[i] + 0;
		ny = py[i] - 1;
		if (ny == 20) {
			jul_false(i);
			gotoxy(N_ROW + 1, 0); //�ӽ� Ż���� ���
			printf("%d��", i);
			continue;
		}
		move_tail_jul(i, nx, ny);
	}
}

void move_right() {
	int nx, ny;
	//���� ���������� ���ϱ� �ǿ����ʿ� �ִ� �÷��̾���� ������. n_player = 8(01234567) n_player - 1; n_player = 7(0123456) -2
	if (n_player % 2 == 0) { //¦���� - 1
		for (int i = n_player - 1; i > 0; i -= 2) {
			nx = px[i] + 0;
			ny = py[i] + 1;
			if (placable_jul(ny) == false) {
				break;
			}
			move_tail_jul(i, nx, ny);
		}
	}
	else if (n_player % 2 == 1) { //Ȧ���� - 2
		for (int i = n_player - 2; i > 0; i -= 2) {
			nx = px[i] + 0;
			ny = py[i] + 1;
			if (placable_jul(ny) == false) {
				break;

				move_tail_jul(i, nx, ny);
			}
		}
	}
	// �״��� ��('-')�� ������ ���� (line[2])���� ���������� �����Դϴ�.
	for (int i = 2; i >= 0; i--) {
		nx = lx[i] + 0;
		ny = ly[i] + 1;
		if (placable(nx, ny) == false) {
			break;
		}
		move_tail_line(i, nx, ny);
	}
	// �״������� ���ʿ� �ִ� �÷��̾ 0���� �����δ�. ����(6420 �̴ϱ� 0 -> 2 -> 4 -> 6 ��) 7�ϋ�(0,2,4,6,8) 8�̸� (0,1,2,3,4,5,6,7) 9�̸� (0,1,2,3,4,5,6,7,8)
	for (int i = 0; i < n_player; i += 2) {
		nx = px[i] + 0;
		ny = py[i] + 1;
		if (ny == 20) {
			jul_false(i);
			gotoxy(N_ROW + 1, 0);
			printf("%d��", i);
			continue;
		}
		move_tail_jul(i, nx, ny);
	}
}

// back_buf[][]�� ���
void move_tail_jul(int player, int nx, int ny) {
	int p = player;  // �̸��� ��...
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
//Ű����


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
			printf("�������� ���⸦ ����߽��ϴ�!!");
			str_l = -str_2 + str_1;
		}
		else if (key == K_D) {
			right_lying();
			gotoxy(N_ROW, 0);
			printf("���������� ���⸦ ����߽��ϴ�!!");
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
		if (tick == 4000) { //�׽�Ʈ�� 30�ʵ��� ��������� 30�ʵ��� �����ִ� �÷��̾� ���
			jul_pass();
			break;
		}
	}
}
//�����Լ�
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

//����ϴ��Լ�
void jul_pass(void) {
	for (int i = 0; i < n_player; i++) {
		if (player[i].is_alive == false || jul_fal[i] == true) {
			player[i].is_alive = true;
			jul_fal[i] = false;
			player[i].stamina = 100;
		}
	}
}
//Ż���ϴ��Լ� (���� �̹�Ż�����÷��̾�� �����ؾߵ�)
void jul_false(int i) {
	//�������ӿ����� is_alive == true && jul_fal[i] == false�� �ֵ鸸 ��������
	if (player[i].is_alive == true && jul_fal[i] == false) {
		player[i].hasitem = false;
		player[i].str /= 50;
		player[i].intel /= 50;
	}
	else if (player[i].is_alive == false || jul_fal[i] == true) {
		jul_fal[i] = false;
	}
}