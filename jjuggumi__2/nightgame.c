#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void sample_init(void);
void ITEM_random(void);
void ITEM_USE(void);
void player_adjacency(void);
void move_manual(key_t key);
void move_random(int i, int dir);
void move_tail(int i, int nx, int ny);
void status_pluse(int i,int j);
void status_subtraction(int i, int j);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�
int ix[ITEM_MAX], iy[ITEM_MAX];
bool item_used[4];
bool item_ask[4];
int copy_item_num[6];
bool ask_num[6];

void status_pluse(int i,int j) {
	if (player[j].hasitem == TRUE) {
		player[j].intel = player[j].intel + use_item[i].intel_buf;
		player[j].str = player[j].str + use_item[i].str_buf;
		player[j].stamina = player[j].stamina + use_item[i].stamina_buf;
	}
}
void status_subtraction(int i, int j) {
	if (player[j].hasitem == TRUE) {
		player[j].intel = player[j].intel - use_item[i].intel_buf;
		player[j].str = player[j].str - use_item[i].str_buf;
		player[j].stamina = player[j].stamina - use_item[i].stamina_buf;
	}
}


void ITEM_USE(void) {
	int num;
	int percent = randint(0, 50);
	for (int j = 0; j < n_player; j++) {
		for (int i = 0; i < 4; i++) {
			//�������� �����ʿ� ���� �ش� �ƴ����� �÷��̾�� �� 
			if (px[j] == ix[i] && py[j] == iy[i]+1 || px[j] == ix[i]+1 && py[j] == iy[i]
				|| px[j] == ix[i] -1 && py[j] == iy[i] || px[j] == ix[i]  && py[j] == iy[i]-1) {
				if (player[j].hasitem == FALSE && item_used[i]==FALSE) {

					player[j].item = use_item[i];
					//�÷��̾ ���� ������ ��ȣ ���
					copy_item_num[j] = i;
					
					back_buf[ix[i]][iy[i]] = ' ';
					player[j].hasitem = TRUE;
					item_used[i] = TRUE;
					status_pluse(i, j);
					
				}
				//�������� ������ �ִ� ��� �������� �ٲ��� �׳� ���� ����
				else if(player[j].hasitem == TRUE && item_used[i] == FALSE && item_ask[i] == FALSE) {
					if (j == 0) {
						gotoxy(N_ROW + 2, 0);
						printf(" 0�� ������ ������(1�� ��������, 2�� ��ȯ): ");
						scanf_s("%d", &num);
						if (num == 2) {
							status_subtraction(copy_item_num[j], j);
							//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
							use_item[copy_item_num[j]] = use_item[i];

							//���� �������� �÷��̾ ���� ���������� ����
							use_item[i] = player[j].item;

							//�÷��̾��� �������� ����
							player[j].item = use_item[copy_item_num[j]];
							status_pluse(copy_item_num[j], j);
							printf("0�� ���� ��ȯ");
						}
						item_ask[i] = TRUE;
					}
					else {
						if (percent <51){
							status_subtraction(copy_item_num[j], j);
							use_item[copy_item_num[j]] = use_item[i];
							use_item[i] = player[j].item;
							player[j].item = use_item[copy_item_num[j]];
							status_pluse(copy_item_num[j], j);
							gotoxy(N_ROW + 2, 0);
							printf("%d�� ���� ��ȯ",j);
							item_ask[i] = TRUE;
						}
					}
				}
				
			}
		}
	}
	
}

void player_adjacency(void) {
	//������ ���� ����
	int num; 
	int num_1;
	for (int j = 0; j < n_player; j++) {
		for (int i = 0; i < n_player; i++) {
			if (player[j].stamina < 0) {
				player[j].stamina = 0;
			}
			if (player[i].stamina < 0) {
				player[i].stamina = 0;
			}
			if (px[j] == px[i] && py[j] == py[i] + 1 || px[j] == px[i] + 1 && py[j] == py[i] 
				|| px[j] == px[i] - 1 && py[j] == py[i]  || px[j] == px[i] && py[j] == py[i] - 1 ) {
				
				if (j == 0 && ask_num[i]==FALSE) {
					//���� �����ϴ� �÷��̾�� ����ġ��
					gotoxy(N_ROW + 2, 0);
					printf(" %d�� %d�� ����ħ (1�� ��Ż, 2�� ȸ�� 3�� ����): ",j,i);
					scanf_s("%d", &num);

					if (num == 1) {
						//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
						if (player[j].stamina>0) {
							if (player[j].str > player[i].str) {
								//����
								//�÷��̾ �������� ������ 
								if (player[j].hasitem == TRUE) {
									//��밡 �������� ������
									if (player[i].hasitem == TRUE) {
										player[j].stamina = player[j].stamina - 40;

										status_subtraction(copy_item_num[j], j);
										status_subtraction(copy_item_num[i], i);
										//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
										use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

										//���� �������� �÷��̾ ���� ���������� ����
										use_item[copy_item_num[i]] = player[j].item;

										//�÷��̾��� �������� ����
										player[j].item = use_item[copy_item_num[j]];
										player[i].item = use_item[copy_item_num[i]];
										status_pluse(copy_item_num[j], j);
										status_pluse(copy_item_num[i], i);

									}
									//��밡 �������� ������
									else {
										printf("��ȯ ����");
									}
								}
								//�÷��̾� �������� ������
								else if((player[j].hasitem == FALSE)){
									//��밡 �������� ������
									if (player[i].hasitem == TRUE) {
										status_subtraction(copy_item_num[i], i);
										copy_item_num[j] = copy_item_num[i];
										player[i].hasitem = FALSE;
										player[j].hasitem = TRUE;
										player[j].item = use_item[copy_item_num[j]];
										player[j].stamina = player[j].stamina - 40;
										status_pluse(copy_item_num[j], j);
										printf("��Ż ����");
									}
									//��밡 �������� ������
									else {
										printf("��Ż ����");
									}
								}
							}
							else {//����
								player[j].stamina = player[j].stamina - 60;
								printf("������");
							}
						}
						else {
							printf("���׹̳� ����");
						}
					}
					else if (num == 2) {
						//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
						if (player[j].stamina > 0) {
							if (player[j].intel > player[i].intel) {
								//����
								//�÷��̾ �������� ������ 
								if (player[j].hasitem == TRUE) {
									//��밡 �������� ������
									if (player[i].hasitem == TRUE) {
										player[j].stamina = player[j].stamina - 20;

										status_subtraction(copy_item_num[j], j);
										status_subtraction(copy_item_num[i], i);
										//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
										use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

										//���� �������� �÷��̾ ���� ���������� ����
										use_item[copy_item_num[i]] = player[j].item;

										//�÷��̾��� �������� ����
										player[j].item = use_item[copy_item_num[j]];
										player[i].item = use_item[copy_item_num[i]];
										status_pluse(copy_item_num[j], j);
										status_pluse(copy_item_num[i], i);

									}
									//��밡 �������� ������
									else {
										printf("��ȯ ����");
									}
								}
								//�÷��̾� �������� ������
								else if ((player[j].hasitem == FALSE)) {
									//��밡 �������� ������
									if (player[i].hasitem == TRUE) {
										status_subtraction(copy_item_num[i], i);
										copy_item_num[j] = copy_item_num[i];
										player[i].hasitem = FALSE;
										player[j].hasitem = TRUE;
										player[j].item = use_item[copy_item_num[j]];
										player[j].stamina = player[j].stamina - 20;
										status_pluse(copy_item_num[j], j);
										printf("��Ż ����");
									}
									//��밡 �������� ������
									else {
										printf("��Ż ����");
									}
								}
							}
							else {//����
								player[j].stamina = player[j].stamina - 40;
								printf("���ɺ���");
							}
						}
						else {
							printf("���׹̳� ����");
						}
					}

					//�ٽ� ���� �ʱ� ����
					ask_num[i] = TRUE;
				}

				else if(ask_num[i] == FALSE){
					gotoxy(N_ROW + 2, 0);
					printf(" %d�� %d�� ����ħ", j, i);
					//�ٸ� �÷��̾��
					//�Ѹ� ���Ⱑ �ִ� ��Ȳ
					num_1 = randint(1, 6);
					if (player[i].hasitem == TRUE && player[j].hasitem == FALSE || player[i].hasitem == FALSE && player[j].hasitem == TRUE) {
						if (player[i].hasitem == TRUE) {
							if (num_1 == 1) {
								//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
								if (player[i].stamina > 0) {
									if (player[i].str > player[j].str) {
										//����
										//�÷��̾ �������� ������ 
										if (player[i].hasitem == TRUE) {
											//��밡 �������� ������
											if (player[j].hasitem == TRUE) {
												player[i].stamina = player[i].stamina - 40;

												status_subtraction(copy_item_num[i], i);
												status_subtraction(copy_item_num[j], j);
												//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
												use_item[copy_item_num[i]] = use_item[copy_item_num[j]];

												//���� �������� �÷��̾ ���� ���������� ����
												use_item[copy_item_num[j]] = player[i].item;

												//�÷��̾��� �������� ����
												player[i].item = use_item[copy_item_num[i]];
												player[j].item = use_item[copy_item_num[j]];
												status_pluse(copy_item_num[i], i);
												status_pluse(copy_item_num[j], j);

											}
											//��밡 �������� ������
											else {
												printf("��ȯ ����");
											}
										}
										//�÷��̾� �������� ������
										else if ((player[i].hasitem == FALSE)) {
											//��밡 �������� ������
											if (player[j].hasitem == TRUE) {
												status_subtraction(copy_item_num[j], j);
												copy_item_num[i] = copy_item_num[j];
												player[j].hasitem = FALSE;
												player[i].hasitem = TRUE;
												player[i].item = use_item[copy_item_num[i]];
												player[i].stamina = player[i].stamina - 40;
												status_pluse(copy_item_num[i], i);
												printf("��Ż ����");
											}
											//��밡 �������� ������
											else {
												printf("��Ż ����");
											}
										}
									}
									else {//����
										player[i].stamina = player[i].stamina - 60;
										printf("������");
									}
								}
								else {
									printf("���׹̳� ����");
								}
							}
							else if (num_1 == 2) {
								//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
								if (player[i].stamina > 0) {
									if (player[i].intel > player[j].intel) {
										//����
										//�÷��̾ �������� ������ 
										if (player[i].hasitem == TRUE) {
											//��밡 �������� ������
											if (player[j].hasitem == TRUE) {
												player[i].stamina = player[i].stamina - 20;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
												use_item[copy_item_num[i]] = use_item[copy_item_num[j]];

												//���� �������� �÷��̾ ���� ���������� ����
												use_item[copy_item_num[j]] = player[i].item;

												//�÷��̾��� �������� ����
												player[i].item = use_item[copy_item_num[i]];
												player[j].item = use_item[copy_item_num[j]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//��밡 �������� ������
											else {
												printf("��ȯ ����");
											}
										}
										//�÷��̾� �������� ������
										else if ((player[i].hasitem == FALSE)) {
											//��밡 �������� ������
											if (player[j].hasitem == TRUE) {
												status_subtraction(copy_item_num[j], j);
												copy_item_num[i] = copy_item_num[j];
												player[j].hasitem = FALSE;
												player[i].hasitem = TRUE;
												player[i].item = use_item[copy_item_num[i]];
												player[i].stamina = player[i].stamina - 20;
												status_pluse(copy_item_num[i], i);
												printf("��Ż ����");
											}
											//��밡 �������� ������
											else {
												printf("��Ż ����");
											}
										}
									}
									else {//����
										player[i].stamina = player[i].stamina - 40;
										printf("���ɺ���");
									}
								}
								else {
									printf("���׹̳� ����");
								}
							}

							//�ٽ� ���� �ʱ� ����/////////////////
							ask_num[j] = TRUE;
						}
						else if (player[j].hasitem == TRUE) {
							if (num_1 == 1) {
								//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
								if (player[j].stamina > 0) {
									if (player[j].str > player[i].str) {
										//����
										//�÷��̾ �������� ������ 
										if (player[j].hasitem == TRUE) {
											//��밡 �������� ������
											if (player[i].hasitem == TRUE) {
												player[j].stamina = player[j].stamina - 40;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
												use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

												//���� �������� �÷��̾ ���� ���������� ����
												use_item[copy_item_num[i]] = player[j].item;

												//�÷��̾��� �������� ����
												player[j].item = use_item[copy_item_num[j]];
												player[i].item = use_item[copy_item_num[i]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//��밡 �������� ������
											else {
												printf("��ȯ ����");
											}
										}
										//�÷��̾� �������� ������
										else if ((player[j].hasitem == FALSE)) {
											//��밡 �������� ������
											if (player[i].hasitem == TRUE) {
												status_subtraction(copy_item_num[i], i);
												copy_item_num[j] = copy_item_num[i];
												player[i].hasitem = FALSE;
												player[j].hasitem = TRUE;
												player[j].item = use_item[copy_item_num[j]];
												player[j].stamina = player[j].stamina - 40;
												status_pluse(copy_item_num[j], j);
												printf("��Ż ����");
											}
											//��밡 �������� ������
											else {
												printf("��Ż ����");
											}
										}
									}
									else {//����
										player[j].stamina = player[j].stamina - 60;
										printf("������");
									}
								}
								else {
									printf("���׹̳� ����");
								}
							}
							else if (num_1 == 2) {
								//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
								if (player[j].stamina > 0) {
									if (player[j].intel > player[i].intel) {
										//����
										//�÷��̾ �������� ������ 
										if (player[j].hasitem == TRUE) {
											//��밡 �������� ������
											if (player[i].hasitem == TRUE) {
												player[j].stamina = player[j].stamina - 20;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
												use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

												//���� �������� �÷��̾ ���� ���������� ����
												use_item[copy_item_num[i]] = player[j].item;

												//�÷��̾��� �������� ����
												player[j].item = use_item[copy_item_num[j]];
												player[i].item = use_item[copy_item_num[i]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//��밡 �������� ������
											else {
												printf("��ȯ ����");
											}
										}
										//�÷��̾� �������� ������
										else if ((player[j].hasitem == FALSE)) {
											//��밡 �������� ������
											if (player[i].hasitem == TRUE) {
												status_subtraction(copy_item_num[i], i);
												copy_item_num[j] = copy_item_num[i];
												player[i].hasitem = FALSE;
												player[j].hasitem = TRUE;
												player[j].item = use_item[copy_item_num[j]];
												player[j].stamina = player[j].stamina - 20;
												status_pluse(copy_item_num[j], j);
												printf("��Ż ����");
											}
											//��밡 �������� ������
											else {
												printf("��Ż ����");
											}
										}
									}
									else {//����
										player[j].stamina = player[j].stamina - 40;
										printf("���ɺ���");
									}
								}
								else {
									printf("���׹̳� ����");
								}
							}

							//�ٽ� ���� �ʱ� ����
							ask_num[j] = TRUE;
						}
					}
					//�Ѵ� ���Ⱑ �ִ� ��Ȳ
					else if (player[i].hasitem == TRUE && player[j].hasitem == TRUE || player[i].hasitem == FALSE && player[j].hasitem == FALSE) {
						//�Ѵ� ���Ⱑ �ְų� ���� ��Ȳ���� �÷��̾� ��ȣ�� �� ū���� ����
						if (i > j) {
							if (num_1 == 1) {
								//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
								if (player[i].stamina > 0) {
									if (player[i].str > player[j].str) {
										//����
										//�÷��̾ �������� ������ 
										if (player[i].hasitem == TRUE) {
											//��밡 �������� ������
											if (player[j].hasitem == TRUE) {
												player[i].stamina = player[i].stamina - 40;

												status_subtraction(copy_item_num[i], i);
												status_subtraction(copy_item_num[j], j);
												//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
												use_item[copy_item_num[i]] = use_item[copy_item_num[j]];

												//���� �������� �÷��̾ ���� ���������� ����
												use_item[copy_item_num[j]] = player[i].item;

												//�÷��̾��� �������� ����
												player[i].item = use_item[copy_item_num[i]];
												player[j].item = use_item[copy_item_num[j]];
												status_pluse(copy_item_num[i], i);
												status_pluse(copy_item_num[j], j);

											}
											//��밡 �������� ������
											else {
												printf("��ȯ ����");
											}
										}
										//�÷��̾� �������� ������
										else if ((player[i].hasitem == FALSE)) {
											//��밡 �������� ������
											if (player[j].hasitem == TRUE) {
												status_subtraction(copy_item_num[j], j);
												copy_item_num[i] = copy_item_num[j];
												player[j].hasitem = FALSE;
												player[i].hasitem = TRUE;
												player[i].item = use_item[copy_item_num[i]];
												player[i].stamina = player[i].stamina - 40;
												status_pluse(copy_item_num[i], i);
												printf("��Ż ����");
											}
											//��밡 �������� ������
											else {
												printf("��Ż ����");
											}
										}
									}
									else {//����
										player[i].stamina = player[i].stamina - 60;
										printf("������");
									}
								}
								else {
									printf("���׹̳� ����");
								}
							}
							else if (num_1 == 2) {
								//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
								if (player[i].stamina > 0) {
									if (player[i].intel > player[j].intel) {
										//����
										//�÷��̾ �������� ������ 
										if (player[i].hasitem == TRUE) {
											//��밡 �������� ������
											if (player[j].hasitem == TRUE) {
												player[i].stamina = player[i].stamina - 20;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
												use_item[copy_item_num[i]] = use_item[copy_item_num[j]];

												//���� �������� �÷��̾ ���� ���������� ����
												use_item[copy_item_num[j]] = player[i].item;

												//�÷��̾��� �������� ����
												player[i].item = use_item[copy_item_num[i]];
												player[j].item = use_item[copy_item_num[j]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//��밡 �������� ������
											else {
												printf("��ȯ ����");
											}
										}
										//�÷��̾� �������� ������
										else if ((player[i].hasitem == FALSE)) {
											//��밡 �������� ������
											if (player[j].hasitem == TRUE) {
												status_subtraction(copy_item_num[j], j);
												copy_item_num[i] = copy_item_num[j];
												player[j].hasitem = FALSE;
												player[i].hasitem = TRUE;
												player[i].item = use_item[copy_item_num[i]];
												player[i].stamina = player[i].stamina - 20;
												status_pluse(copy_item_num[i], i);
												printf("��Ż ����");
											}
											//��밡 �������� ������
											else {
												printf("��Ż ����");
											}
										}
									}
									else {//����
										player[i].stamina = player[i].stamina - 40;
										printf("���ɺ���");
									}
								}
								else {
									printf("���׹̳� ����");
								}
							}

							//�ٽ� ���� �ʱ� ����/////////////////
							ask_num[j] = TRUE;
						}
						else if (j > i) {
							if (num_1 == 1) {
								//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
								if (player[j].stamina > 0) {
									if (player[j].str > player[i].str) {
										//����
										//�÷��̾ �������� ������ 
										if (player[j].hasitem == TRUE) {
											//��밡 �������� ������
											if (player[i].hasitem == TRUE) {
												player[j].stamina = player[j].stamina - 40;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
												use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

												//���� �������� �÷��̾ ���� ���������� ����
												use_item[copy_item_num[i]] = player[j].item;

												//�÷��̾��� �������� ����
												player[j].item = use_item[copy_item_num[j]];
												player[i].item = use_item[copy_item_num[i]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//��밡 �������� ������
											else {
												printf("��ȯ ����");
											}
										}
										//�÷��̾� �������� ������
										else if ((player[j].hasitem == FALSE)) {
											//��밡 �������� ������
											if (player[i].hasitem == TRUE) {
												status_subtraction(copy_item_num[i], i);
												copy_item_num[j] = copy_item_num[i];
												player[i].hasitem = FALSE;
												player[j].hasitem = TRUE;
												player[j].item = use_item[copy_item_num[j]];
												player[j].stamina = player[j].stamina - 40;
												status_pluse(copy_item_num[j], j);
												printf("��Ż ����");
											}
											//��밡 �������� ������
											else {
												printf("��Ż ����");
											}
										}
									}
									else {//����
										player[j].stamina = player[j].stamina - 60;
										printf("������");
									}
								}
								else {
									printf("���׹̳� ����");
								}
							}
							else if (num_1 == 2) {
								//��Ż ���ý� �� �� (���׹̳��� 0���� ũ��) 
								if (player[j].stamina > 0) {
									if (player[j].intel > player[i].intel) {
										//����
										//�÷��̾ �������� ������ 
										if (player[j].hasitem == TRUE) {
											//��밡 �������� ������
											if (player[i].hasitem == TRUE) {
												player[j].stamina = player[j].stamina - 20;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//�� ����ڰ� ���� �������� ��ȣ�� ������� �������� ���� ���������� ����
												use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

												//���� �������� �÷��̾ ���� ���������� ����
												use_item[copy_item_num[i]] = player[j].item;

												//�÷��̾��� �������� ����
												player[j].item = use_item[copy_item_num[j]];
												player[i].item = use_item[copy_item_num[i]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//��밡 �������� ������
											else {
												printf("��ȯ ����");
											}
										}
										//�÷��̾� �������� ������
										else if ((player[j].hasitem == FALSE)) {
											//��밡 �������� ������
											if (player[i].hasitem == TRUE) {
												status_subtraction(copy_item_num[i], i);
												copy_item_num[j] = copy_item_num[i];
												player[i].hasitem = FALSE;
												player[j].hasitem = TRUE;
												player[j].item = use_item[copy_item_num[j]];
												player[j].stamina = player[j].stamina - 20;
												status_pluse(copy_item_num[j], j);
												printf("��Ż ����");
											}
											//��밡 �������� ������
											else {
												printf("��Ż ����");
											}
										}
									}
									else {//����
										player[j].stamina = player[j].stamina - 40;
										printf("���ɺ���");
									}
								}
								else {
									printf("���׹̳� ����");
								}
							}

							//�ٽ� ���� �ʱ� ����
							ask_num[i] = TRUE;
						}
					}
				}


			}
		}
	}

}






void ITEM_random(void) {
	int x, y,a;
	int N_item;

	N_item = n_item - n_player;
	for (int i = 0; i < 4; i++) {
		// ���� �ڸ��� ������ �ٽ� ����
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!placable(x, y));
		ix[i] = x;
		iy[i] = y;
		a=randint(0, 9);
		use_item[i] = item[a];
		back_buf[ix[i]][iy[i]] = 'I';
	}
}

void sample_init(void) {
	map_init(8, 25);
	int x, y;
	for (int i = 0; i < n_player; i++) {
		// ���� �ڸ��� ������ �ٽ� ����
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!placable(x, y));
		px[i] = x;
		py[i] = y;
		period[i] = randint(100, 150);

		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
		
	}

	tick = 0;
}

void move_manual(key_t key) {
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

	move_tail(0, nx, ny);
}

// 0 <= dir < 4�� �ƴϸ� ����
void move_random(int player, int dir) {
	int p = player;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�

	// ������ ������ ���� ���� ���ٰ� ����(���� ������ ����)	

	do {
		nx = px[p] + randint(-1, 1);
		ny = py[p] + randint(-1, 1);
	} while (!placable(nx, ny));

	move_tail(p, nx, ny);
}

// back_buf[][]�� ���
void move_tail(int player, int nx, int ny) {
	int p = player;  // �̸��� ��...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void nightgame(void) {
	int end_point = 0;
	sample_init();
	ITEM_random();
	system("cls");
	display();
	while (1) {
		// player 0�� ������ ������(4����)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual(key);
			//�÷��̾��� �������� ������ �������� �ߺ��϶� ������ �޵�����
			for (int i = 0; i < 4; i++) {
				item_ask[i] = FALSE;
			}
			//�÷��̾� ����ĥ�� ���� ����
			for (int i = 0; i < 6; i++) {
				ask_num[i] = FALSE;
			}
			//�����϶����� �߰��� �޼��� �����
			gotoxy(N_ROW + 2, 0);
			printf("                                                            \n");
			printf("                                                             ");
		}

		// player 1 ���ʹ� �������� ������(8����)
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				move_random(i, -1);
			}
		}
		ITEM_USE();
		player_adjacency();
		display();
		Sleep(10);
		tick += 10;
		for (int i = 0; i < 6; i++) {
			if (tick==10000) {
				end_point += 1;
			}
		}
		if (end_point > 0) {
			break;
		}
	}
}