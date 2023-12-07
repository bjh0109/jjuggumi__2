// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);




// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {

	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1; // -1 리턴하면 메인함수에서 처리하고 종료
	}

	// 플레이어 데이터 load
	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		// 아직 안 배운 문법(구조체 포인터, 간접참조연산자)
		PLAYER* p = &player[i];
		// 파일에서 각 스탯 최댓값 읽기
		fscanf_s(fp, "%s%d%d",p->name, (unsigned int)sizeof(p->name),&(p->intel), &(p->str));
		p->stamina = 100; // 100%
		// 현재 상태
		p->is_alive = true;
		p->hasitem = false;
	}
	// 아이템 데이터 load
	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d",
			item[i].name, (unsigned int)sizeof(item[i].name),
			&(item[i].intel_buf),
			&(item[i].str_buf),
			&(item[i].stamina_buf));
	}

	fclose(fp);

	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		player_tt[i] = true;
	}


	return 0;
}

void text_color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void print_color_text(char const* const text, int color) {
	text_color(color);
	printf(text);
	text_color(white);
}

void intro_color(char const* const text, int color) {
	printf("|");
	print_color_text(text, color);
	printf("|\n");
}

void intro(void) {
	printf(".--------------------------------------------------------------.\n");
	printf("|                                                              |\n");
	Sleep(35);
	intro_color("       _     _          ____  ____         __  __ _           ", red);
	Sleep(45);
	intro_color("      | |   | |_   _   / ___|/ ___|_   _  |  \\/  (_)          ", red);
	Sleep(55);
	intro_color("   _  | |_  | | | | | | |  _| |  _| | | | | |\\/| | |          ", red);
	Sleep(75);
	intro_color("  | |_| | |_| | |_| | | |_| | |_| | |_| | | |  | | |          ", red);
	Sleep(85);
	intro_color("   \\___/ \\___/ \\__,_|  \\____|\\____|\\__,_| |_|  |_|_|          ", red);
	Sleep(95);
	intro_color("    ____    _    __  __ _____   ____ _____  _    ____ _____   ", yellow);
	Sleep(105);
	intro_color("   / ___|  / \\  |  \\/  | ____| / ___|_   _|/ \\  |  _ \\_   _|  ", yellow);
	Sleep(115);
	intro_color("  | |  _  / _ \\ | |\\/| |  _|   \\___ \\ | | / _ \\ | |_) || |    ", yellow);
	Sleep(125);
	intro_color("  | |_| |/ ___ \\| |  | | |___   ___) || |/ ___ \\|  _ < | |    ", yellow);
	Sleep(135);
	intro_color("   \\____/_/   \\_\\_|  |_|_____| |____/ |_/_/   \\_\\_| \\_\\|_|    ", yellow);
	printf("|                                                              |\n");
	printf("'--------------------------------------------------------------'\n");
	Sleep(1500);
}


void ending(void) {
	system("cls");
	printf(" _____                                                  _____ \n");
	printf("( ___ )------------------------------------------------( ___ )\n");
	printf(" |   |                                                  |   | \n");
	printf(" |   |      _     _        ____  ____       __  __ _    |   | \n");
	printf(" |   |     | |   | |_   _ / ___|/ ___|_   _|  \\/  (_)   |   | \n");
	printf(" |   |  _  | |_  | | | | | |  _| |  _| | | | |\\/| | |   |   | \n");
	printf(" |   | | |_| | |_| | |_| | |_| | |_| | |_| | |  | | |   |   | \n");
	printf(" |   |  \\___/ \\___/ \\__,_|\\____|\\____|\\__,_|_|  |_|_|   |   | \n");
	printf(" |   |   ____                        _____ _   _ ____   |   | \n");
	printf(" |   |  / ___| __ _ _ __ ___   ___  | ____| \\ | |  _ \\  |   | \n");
	printf(" |   | | |  _ / _` | '_ ` _ \\ / _ \\ |  _| |  \\| | | | | |   | \n");
	printf(" |   | | |_| | (_| | | | | | |  __/ | |___| |\\  | |_| | |   | \n");
	printf(" |   |  \\____|\\__,_|_| |_| |_|\\____| |_____|_| \\_|____/  |   | \n");
	printf(" |___|                                                  |___| \n");
	printf("(_____)------------------------------------------------(_____)\n");
	printf("winner : ");

	int a[PLAYER_MAX];
	for (int i = 0; i < n_player; i++) {
		a[i] = -1; // 모든 요소를 -1로 초기화
	}

	for (int i = 0; i < n_player; i++) {
		if (pass_player[i] == true) {
			a[i] = i;
			for (int j = 0; j < n_player; j++) {
				if (a[j] != -1) {
					printf("%d번 ", a[j]);
				}
			}
		}
	}



}

int main(void) {
	jjuggumi_init();
	intro();
	//sample();
	mugunghwa();
	nightgame();
	juldarigi();
	jebi();
	ending();
	return 0;
}