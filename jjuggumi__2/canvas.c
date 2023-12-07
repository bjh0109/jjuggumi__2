#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "jjuggumi.h"
#include "canvas.h"

#define DIALOG_DURATION_SEC		4

void draw(void);
void print_status(void);
void print_status_1(void);

// (zero-base) row��, col���� Ŀ�� �̵�
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row��, col���� ch ���
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

void map_init(int n_row, int n_col) {
	// �� ���۸��� ������ ����
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// ���Թ� �̷��� �� �� �ִµ� �Ϻη� �� ����������
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '#' : ' ';
		}
	}
}

// back_buf[row][col]�� �̵��� �� �ִ� �ڸ����� Ȯ���ϴ� �Լ�
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

bool placable_jul(int y) {
	if (n_player == 2) {
		if (y == 16 || y == 24) {
			return false;
		}
	}
	else if (n_player == 3) {
		if (y == 15 || y == 24) {
			return false;
		}
	}
	else if (n_player == 4) {
		if (y == 15 || y == 25) {
			return false;
		}
	}
	else if (n_player == 5) {
		if (y == 14 || y == 25) {
			return false;
		}
	}
	else if (n_player == 6) {
		if (y == 14 || y == 26) {
			return false;
		}
	}
	else if (n_player == 7) {
		if (y == 13 || y == 26) {
			return false;
		}
	}
	else if (n_player == 8) {
		if (y == 13 || y == 27) {
			return false;
		}
	}
	else if (n_player == 9) {
		if (y == 12 || y == 27) {
			return false;
		}
	}
	else if (n_player == 10) {
		if (y == 12 || y == 28) {
			return false;
		}
	}
	else {
		return true;
	}
}

// ��ܿ� ����, �ϴܿ��� ���� ���¸� ���
void display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // �߰��� ǥ���� ������ ������ �ʰ� ����â ������ �� ������ ���
	print_status();
}

void display_1(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // �߰��� ǥ���� ������ ������ �ʰ� ����â ������ �� ������ ���
	print_status_1();
}

void draw(void) {
	for (int row = 0; row < N_ROW; row++) {
		for (int col = 0; col < N_COL; col++) {
			if (front_buf[row][col] != back_buf[row][col]) {
				front_buf[row][col] = back_buf[row][col];
				printxy(front_buf[row][col], row, col);
			}
		}
	}
}

void print_status(void) {
	printf("no. of players left: %d\n", n_player);
	printf("��ȣ   ����ִ°� �̸� ���� �� ���׹̳�  \n");
	for (int i = 0; i < n_player; i++) {
		if (player[i].hasitem == TRUE) {
			printf("player[%d] %5s %5s: %2d  %2d  %3.1f item: %5s   \n",i, player[i].is_alive ? "alive" : "DEAD" ,player[i].name, player[i].intel, player[i].str, player[i].stamina,player[i].item.name);
		}
		else {
			printf("player[%d] %5s %5s:  %2d  %2d  %3.1f      \n",i, player[i].is_alive ? "alive" : "DEAD", player[i].name, player[i].intel, player[i].str, player[i].stamina);
		}
		
	}
	for (int i = 0; i < 4; i++) {
		printf("name: %s  intel_buf: %d   str_buf: %d   stamina_buf: %d         \n", use_item[i].name,use_item[i].intel_buf, use_item[i].str_buf,use_item[i].stamina_buf);
	}
}

void print_status_1(void) {
	printf("no. of players left: %d\n", n_alive);
	for (int p = 0; p < n_player; p++) {
		if (player[p].is_alive || jul_fal[p] == true) {
			printf("player %2d: %5s %d(+%d) %d(+%d) %.1f(%d)\n",
				p, player[p].is_alive ? "alive" : jul_fal[p] ? "alive*" : "DEAD", player[p].intel, player[p].item.intel_buf, player[p].str, player[p].item.str_buf, player[p].stamina, player[p].item.stamina_buf);
		}
	}
}


void kill_player_num(int kill_num[]) {
	for (int j = 0; j < n_player; j++) {
		if (kill_num[j] != -1) {
			printf("%d�� ", kill_num[j]);
		}
	}
}




void dialog(char message[]) {
	// ���� ���� ���� ����
	char temp_buf[ROW_MAX][COL_MAX];
	int center_row = N_ROW / 2;
	int center_col = N_COL / 2;
	int msg_length = strlen(message);
	int box_width = max(msg_length, 18) + 4;
	int box_start_col = center_col - box_width / 2;
	int box_end_col = center_col + box_width / 2;


	memcpy(temp_buf, back_buf, sizeof(back_buf));

	// ��ȭ ���ڸ� �� ���� ǥ��
	for (int i = DIALOG_DURATION_SEC; i >= 0; --i) {
		if (i != DIALOG_DURATION_SEC) {
			Sleep(1000);
		}

		for (int row = center_row - 2; row <= center_row + 2; ++row) {
			for (int col = box_start_col - 2; col <= box_end_col; ++col) {
				back_buf[row][col] = ' ';
			}
		}
		for (int row = center_row - 2; row <= center_row + 2; ++row) {
			for (int col = box_start_col - 2; col <= box_end_col; ++col) {
				if (row == center_row - 2 || row == center_row + 2 || col == box_start_col - 2 || col == box_end_col)
					back_buf[row][col] = '*';
			}
		}



		gotoxy(center_row, box_start_col + 3);
		printf("%s", message);
		gotoxy(center_row, box_start_col + 1);
		printf("%d ", i);

		gotoxy(center_row + 1, box_start_col + 3);
		kill_player_num(kill_player_1);

		draw();
	}


	gotoxy(center_row, box_start_col + 1);

	for (int disappear = 0; disappear < msg_length + 2; disappear++) {
		printf(" ");
	}

	gotoxy(center_row + 1, 2);
	for (int disappear = 0; disappear < 20; disappear++) {
		printf(" ");
	}

	// ���� ���·� ����
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = temp_buf[i][j];
		}
	}

	display();
}