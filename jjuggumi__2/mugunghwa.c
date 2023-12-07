#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3


//bool pass_player[PLAYER_MAX];
bool dialog_player[PLAYER_MAX];
bool map[ROW_MAX][100];


void sample_init_1(void);
void move_manual_1(key_t key);
void move_random_1(int i, int dir);
void move_tail_2(int i, int nx, int ny);
void young();
void kill(int p);
void haha();

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�

int kill_point = 0;
int b = 0;
int c = 0;

int kill_1[10];



void sample_init_1(void) {
	map_init(10, 30);
	int x, y;

	for (int i = 0; i < n_player; i++) {
		// ���� �ڸ��� ������ �ٽ� ����
		do {
			//�÷�������� ������ �����ڸ��� �������� ��������
			x = randint(2, N_ROW - 3);
			y = randint(28, N_COL - 2);
		} while (!placable(x, y));
		px[i] = x;
		py[i] = y;
		period[i] = randint(100, 150);

		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
	}

	//�÷��̾� ���δ� ������� ���� ����
	for (int i = 0; i < n_player; i++) {
		pass_player[i] = false;
	}

	for (int i = 0; i < n_player; i++) {
		dialog_player[i] = false;
	}

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			map[i][j] = false;
		}
	}


	tick = 0;
}

void move_manual_1(key_t key) {
	// �� �������� ������ �� x, y�� delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // ������ ����(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// �������� ���� �ڸ�
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny)) {
		return;
	}
	if (b == 1) {
		//�÷��̾� �� ������ ����
		if (map[nx][ny] == true) {
			move_tail_2(0, nx, ny);
		}
		else if (map[nx][ny] == false) {
			move_tail_2(0, nx, ny);
			display();
			Sleep(500);
			kill(0);
		}


	}
	else {
		move_tail_2(0, nx, ny);
	}
}

// 0 <= dir < 4�� �ƴϸ� ����
void move_random_1(int player_1, int dir) {
	int p = player_1;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�


	// ������ ������ ���� ���� ���ٰ� ����(���� ������ ����)	
	int x[4] = { 0,-1,1,0 };
	int y[4] = { -1.0,0,0 };
	int r = 0;
	do {
		r = randint(0, 101);

		if (r <= 70) {
			nx = px[p] + x[0];
			ny = py[p] + y[0];
		}
		else if (r <= 80) {
			nx = px[p] + x[1];
			ny = py[p] + y[1];
		}
		else if (r <= 90) {
			nx = px[p] + x[2];
			ny = py[p] + y[2];
		}
		else {
			nx = px[p] + x[3];
			ny = py[p] + y[3];
		}
	} while (!placable(nx, ny));

	int kill_move = randint(1, 100);

	if (b == 1 && kill_move < 6) {
		if (map[nx][ny] == false) {
			move_tail_2(p, nx, ny);
			display();
			Sleep(500);
			kill(p);
		}
		else if (map[nx][ny] == true) {
			move_tail_2(p, nx, ny);
		}

	}
	else if (b == 0) {
		move_tail_2(p, nx, ny);
	}
}



void move_tail_2(int player1, int nx, int ny) {
	int v = 0;
	int p = player1;  // �̸��� ��...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;


	for (int x = 0; x < ROW_MAX; x++) {
		for (int y = 0; y < COL_MAX; y++) {
			map[x][y] = false;
		}
	}
	for (int i = 0; i < n_player; i++) {
		if (player_tt[i] == true && pass_player[i] == false) {
			for (int y = py[i]; y < COL_MAX; y++) {
				map[px[i]][y] = true;
			}
		}
	}


}


void young() {
	int a;
	gotoxy(N_ROW, 0);
	printf("%d", tick);

	if (b == 0) {
		back_buf[3][1] = '#';
		back_buf[4][1] = '#';
		back_buf[5][1] = '#';
		back_buf[6][1] = '#';
	}


	if (tick % 1000 == 0) {
		gotoxy(N_ROW + 1, 0);
		printf("��");
	}

	if (tick % 1500 == 0) {
		gotoxy(N_ROW + 1, 2);
		printf("��");
	}

	if (tick % 2000 == 0) {
		gotoxy(N_ROW + 1, 4);
		printf("ȭ");
	}

	if (tick % 2500 == 0) {
		gotoxy(N_ROW + 1, 6);
		printf("��");
	}

	if (tick % 3000 == 0) {
		gotoxy(N_ROW + 1, 8);
		printf("��");
	}

	if (tick % 3200 == 0) {
		gotoxy(N_ROW + 1, 10);
		printf("��");
	}

	if (tick % 3400 == 0) {
		gotoxy(N_ROW + 1, 12);
		printf("��");
	}

	if (tick % 3600 == 0) {
		gotoxy(N_ROW + 1, 14);
		printf("��");
	}

	if (tick % 3800 == 0) {
		gotoxy(N_ROW + 1, 16);
		printf("��");

	}

	if (tick % 4000 == 0) {
		gotoxy(N_ROW + 1, 18);
		printf("��");
		b = 1;
	}

	if (b == 1) {
		back_buf[3][1] = '@';
		back_buf[4][1] = '@';
		back_buf[5][1] = '@';
		back_buf[6][1] = '@';
	}

	if (tick % 7000 == 0) {
		gotoxy(N_ROW + 1, 0);
		for (int i = 0; i < 20 + 2; i++) {
			printf(" ");
			b = 0;
		}
		tick = 0;
		if (kill_point > 0) {
			dialog("Ż��");
			kill_point = 0;
		}


	}
}

void kill(int p) {
	back_buf[px[p]][py[p]] = ' ';
	player[p].is_alive = FALSE;
	player_tt[p] = false;
	kill_point += 1;

	for (int i = 0; i < n_player; i++) {
		kill_player_1[i] = -1; // ��� ��Ҹ� -1�� �ʱ�ȭ
	}

	for (int p = 0; p < n_player; p++) {
		if (player_tt[p] == false && dialog_player[p] == false) {
			kill_player_1[p] = p;
		}
	}
}



void haha() {
	for (int j = 0; j < n_player; j++) {
		if (kill_player_1[j] != -1 && tick % 7000 == 0) {
			dialog_player[j] = true;
		}
	}

	if (tick % 6800 == 0) {
		for (int i = 0; i < ROW_MAX; i++) {
			for (int j = 0; j < COL_MAX; j++) {
				map[i][j] = false;
			}
		}
	}
}





void pass1() {
	for (int i = 0; i < n_player; i++) {
		if (player_tt[i] == true) {
			if ((px[i] == 2 && py[i] == 1) || (px[i] == 3 && py[i] == 2) || (px[i] == 4 && py[i] == 2) ||
				(px[i] == 5 && py[i] == 2) || (px[i] == 6 && py[i] == 2) || (px[i] == 7 && py[i] == 1)) {
				back_buf[px[i]][py[i]] = ' ';
				pass_player[i] = true;
			}
		}
	}
}


void mugunghwa(void) {
	sample_init_1();
	system("cls");
	display();
	Sleep(1000);
	while (1) {
		// player 0�� ������ ������(4����)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual_1(key);
		}

		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0 && pass_player[i] == false) {
				move_random_1(i, -1);
			}
			else
				continue;
		}

		int a[PLAYER_MAX];
		for (int i = 0; i < n_player; i++) {
			a[i] = -1; // ��� ��Ҹ� -1�� �ʱ�ȭ
		}

		for (int i = 0; i < n_player; i++) {
			if (pass_player[i] == true) {
				a[i] = i;
				gotoxy(N_ROW + 3, 0);
				printf("���: ");
				for (int j = 0; j < n_player; j++) {
					if (a[j] != -1) {
						printf("%d�� ", a[j]);
					}
				}
			}
		}
		int passCount_1 = 0;
		int falseCount = 0;
		for (int i = 0; i < n_player; i++) {
			if (pass_player[i]) {
				passCount_1++;
			}
			if (!player_tt[i]) {
				falseCount++;
			}
		}

		if (passCount_1 + falseCount == n_player) {
			break; // Ư�� ������ �����ϸ� ���� ������ ����
		}



		pass1();
		display();
		Sleep(10);
		tick += 10;
		young();
		haha();
	}
}