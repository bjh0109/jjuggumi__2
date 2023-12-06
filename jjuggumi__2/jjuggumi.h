#ifndef _JJUGGUMI_H_
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX		10
#define ITEM_MAX	10

typedef struct {
	char name[100];
	int intel_buf, str_buf, stamina_buf;
} ITEM;


typedef struct {
	int id;
	char name[100];
	// �ɷ�ġ: ����, ��, ���¹̳�
	int intel, str, stamina;
	// ���� ����
	bool is_alive; // Ż�������� false
	bool hasitem; // �������� ������ true
	ITEM item; // ������ 1�� ���� ����

} PLAYER;



ITEM item[ITEM_MAX];
ITEM use_item[4];
ITEM ex_item;
ITEM change_item;

PLAYER player[PLAYER_MAX];  // �⺻�� true, Ż���ϸ� false

int n_player,n_item;
int tick;  // �ð�



// �̴ϰ���
//void sample(void);
//void mugunghwa(void);
//void mugunghwa(void);
void nightgame(void);
//void juldarigi(void);
//void jebi(void);

int randint(int low, int high);

#endif