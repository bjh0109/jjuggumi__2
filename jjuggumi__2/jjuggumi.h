#ifndef _JJUGGUMI_H_
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX		10
#define ITEM_MAX	10

typedef enum Color_type {
	black = 0,  	//0
	dark_blue,	//1
	dark_green,	//2
	dark_sky_blue,//3
	dark_red,  	//4
	dark_purple,	//5
	dark_yellow,	//6
	gray,		//7
	dark_gray,	//8
	blue,		//9
	green,		//10
	sky_blue,	//11
	red,		//12
	purple,		//13
	yellow,		//14
	white		//15
} COLOR;

typedef struct {
	int x;
	int y;
} Point;





typedef struct {
	char name[100];
	int intel_buf, str_buf, stamina_buf;
} ITEM;


typedef struct {
	int id;
	char name[100];
	// �ɷ�ġ: ����, ��, ���¹̳�
	int intel, str;
	double stamina;
	// ���� ����
	bool is_alive; // Ż�������� false
	bool hasitem; // �������� ������ true
	ITEM item; // ������ 1�� ���� ����

} PLAYER;


bool player_tt[PLAYER_MAX];
bool pass_player[PLAYER_MAX];
ITEM item[ITEM_MAX];
ITEM use_item[4];
ITEM ex_item;
ITEM change_item;

PLAYER player[PLAYER_MAX];  // �⺻�� true, Ż���ϸ� false
int jul_fal[PLAYER_MAX];
int n_player,n_item, n_alive;
int tick;  // �ð�

void text_color(int color);
void print_color_text(char const* const text, int color);
void intro_color(char const* const text, int color);
void intro(void);
void ending(void);

// �̴ϰ���
//void sample(void);
void mugunghwa(void);

void nightgame(void);
void juldarigi(void);
//void jebi(void);

int randint(int low, int high);

#endif