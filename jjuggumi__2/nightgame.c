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

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기
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
			//아이템의 오른쪽에 가면 해당 아니템이 플레이어에게 들어감 
			if (px[j] == ix[i] && py[j] == iy[i]+1 || px[j] == ix[i]+1 && py[j] == iy[i]
				|| px[j] == ix[i] -1 && py[j] == iy[i] || px[j] == ix[i]  && py[j] == iy[i]-1) {
				if (player[j].hasitem == FALSE && item_used[i]==FALSE) {

					player[j].item = use_item[i];
					//플레이어가 먹은 아이템 번호 기억
					copy_item_num[j] = i;
					
					back_buf[ix[i]][iy[i]] = ' ';
					player[j].hasitem = TRUE;
					item_used[i] = TRUE;
					status_pluse(i, j);
					
				}
				//아이템을 가지고 있는 경우 아이템을 바꿀지 그냥 갈지 선택
				else if(player[j].hasitem == TRUE && item_used[i] == FALSE && item_ask[i] == FALSE) {
					if (j == 0) {
						gotoxy(N_ROW + 2, 0);
						printf(" 0번 아이템 소유중(1번 지나가기, 2번 교환): ");
						scanf_s("%d", &num);
						if (num == 2) {
							status_subtraction(copy_item_num[j], j);
							//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
							use_item[copy_item_num[j]] = use_item[i];

							//먹은 아이템을 플레이어가 가진 아이템으로 변경
							use_item[i] = player[j].item;

							//플레이어의 아이템을 변경
							player[j].item = use_item[copy_item_num[j]];
							status_pluse(copy_item_num[j], j);
							printf("0번 무기 교환");
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
							printf("%d번 무기 교환",j);
							item_ask[i] = TRUE;
						}
					}
				}
				
			}
		}
	}
	
}

void player_adjacency(void) {
	//선택지 받을 변수
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
					//내가 조종하는 플레이어랑 마주치면
					gotoxy(N_ROW + 2, 0);
					printf(" %d번 %d랑 마주침 (1번 강탈, 2번 회유 3번 무시): ",j,i);
					scanf_s("%d", &num);

					if (num == 1) {
						//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
						if (player[j].stamina>0) {
							if (player[j].str > player[i].str) {
								//성공
								//플레이어가 아이템이 있으면 
								if (player[j].hasitem == TRUE) {
									//상대가 아이템이 있을때
									if (player[i].hasitem == TRUE) {
										player[j].stamina = player[j].stamina - 40;

										status_subtraction(copy_item_num[j], j);
										status_subtraction(copy_item_num[i], i);
										//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
										use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

										//먹은 아이템을 플레이어가 가진 아이템으로 변경
										use_item[copy_item_num[i]] = player[j].item;

										//플레이어의 아이템을 변경
										player[j].item = use_item[copy_item_num[j]];
										player[i].item = use_item[copy_item_num[i]];
										status_pluse(copy_item_num[j], j);
										status_pluse(copy_item_num[i], i);

									}
									//상대가 아이템이 없을때
									else {
										printf("교환 실패");
									}
								}
								//플레이어 아이템이 없으면
								else if((player[j].hasitem == FALSE)){
									//상대가 아이템이 있을때
									if (player[i].hasitem == TRUE) {
										status_subtraction(copy_item_num[i], i);
										copy_item_num[j] = copy_item_num[i];
										player[i].hasitem = FALSE;
										player[j].hasitem = TRUE;
										player[j].item = use_item[copy_item_num[j]];
										player[j].stamina = player[j].stamina - 40;
										status_pluse(copy_item_num[j], j);
										printf("강탈 성공");
									}
									//상대가 아이템이 없을때
									else {
										printf("강탈 실패");
									}
								}
							}
							else {//실패
								player[j].stamina = player[j].stamina - 60;
								printf("힘부족");
							}
						}
						else {
							printf("스테미나 부족");
						}
					}
					else if (num == 2) {
						//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
						if (player[j].stamina > 0) {
							if (player[j].intel > player[i].intel) {
								//성공
								//플레이어가 아이템이 있으면 
								if (player[j].hasitem == TRUE) {
									//상대가 아이템이 있을때
									if (player[i].hasitem == TRUE) {
										player[j].stamina = player[j].stamina - 20;

										status_subtraction(copy_item_num[j], j);
										status_subtraction(copy_item_num[i], i);
										//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
										use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

										//먹은 아이템을 플레이어가 가진 아이템으로 변경
										use_item[copy_item_num[i]] = player[j].item;

										//플레이어의 아이템을 변경
										player[j].item = use_item[copy_item_num[j]];
										player[i].item = use_item[copy_item_num[i]];
										status_pluse(copy_item_num[j], j);
										status_pluse(copy_item_num[i], i);

									}
									//상대가 아이템이 없을때
									else {
										printf("교환 실패");
									}
								}
								//플레이어 아이템이 없으면
								else if ((player[j].hasitem == FALSE)) {
									//상대가 아이템이 있을때
									if (player[i].hasitem == TRUE) {
										status_subtraction(copy_item_num[i], i);
										copy_item_num[j] = copy_item_num[i];
										player[i].hasitem = FALSE;
										player[j].hasitem = TRUE;
										player[j].item = use_item[copy_item_num[j]];
										player[j].stamina = player[j].stamina - 20;
										status_pluse(copy_item_num[j], j);
										printf("강탈 성공");
									}
									//상대가 아이템이 없을때
									else {
										printf("강탈 실패");
									}
								}
							}
							else {//실패
								player[j].stamina = player[j].stamina - 40;
								printf("지능부족");
							}
						}
						else {
							printf("스테미나 부족");
						}
					}

					//다시 묻지 않기 위해
					ask_num[i] = TRUE;
				}

				else if(ask_num[i] == FALSE){
					gotoxy(N_ROW + 2, 0);
					printf(" %d번 %d랑 마주침", j, i);
					//다른 플레이어들
					//한명만 무기가 있는 상황
					num_1 = randint(1, 6);
					if (player[i].hasitem == TRUE && player[j].hasitem == FALSE || player[i].hasitem == FALSE && player[j].hasitem == TRUE) {
						if (player[i].hasitem == TRUE) {
							if (num_1 == 1) {
								//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
								if (player[i].stamina > 0) {
									if (player[i].str > player[j].str) {
										//성공
										//플레이어가 아이템이 있으면 
										if (player[i].hasitem == TRUE) {
											//상대가 아이템이 있을때
											if (player[j].hasitem == TRUE) {
												player[i].stamina = player[i].stamina - 40;

												status_subtraction(copy_item_num[i], i);
												status_subtraction(copy_item_num[j], j);
												//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
												use_item[copy_item_num[i]] = use_item[copy_item_num[j]];

												//먹은 아이템을 플레이어가 가진 아이템으로 변경
												use_item[copy_item_num[j]] = player[i].item;

												//플레이어의 아이템을 변경
												player[i].item = use_item[copy_item_num[i]];
												player[j].item = use_item[copy_item_num[j]];
												status_pluse(copy_item_num[i], i);
												status_pluse(copy_item_num[j], j);

											}
											//상대가 아이템이 없을때
											else {
												printf("교환 실패");
											}
										}
										//플레이어 아이템이 없으면
										else if ((player[i].hasitem == FALSE)) {
											//상대가 아이템이 있을때
											if (player[j].hasitem == TRUE) {
												status_subtraction(copy_item_num[j], j);
												copy_item_num[i] = copy_item_num[j];
												player[j].hasitem = FALSE;
												player[i].hasitem = TRUE;
												player[i].item = use_item[copy_item_num[i]];
												player[i].stamina = player[i].stamina - 40;
												status_pluse(copy_item_num[i], i);
												printf("강탈 성공");
											}
											//상대가 아이템이 없을때
											else {
												printf("강탈 실패");
											}
										}
									}
									else {//실패
										player[i].stamina = player[i].stamina - 60;
										printf("힘부족");
									}
								}
								else {
									printf("스테미나 부족");
								}
							}
							else if (num_1 == 2) {
								//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
								if (player[i].stamina > 0) {
									if (player[i].intel > player[j].intel) {
										//성공
										//플레이어가 아이템이 있으면 
										if (player[i].hasitem == TRUE) {
											//상대가 아이템이 있을때
											if (player[j].hasitem == TRUE) {
												player[i].stamina = player[i].stamina - 20;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
												use_item[copy_item_num[i]] = use_item[copy_item_num[j]];

												//먹은 아이템을 플레이어가 가진 아이템으로 변경
												use_item[copy_item_num[j]] = player[i].item;

												//플레이어의 아이템을 변경
												player[i].item = use_item[copy_item_num[i]];
												player[j].item = use_item[copy_item_num[j]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//상대가 아이템이 없을때
											else {
												printf("교환 실패");
											}
										}
										//플레이어 아이템이 없으면
										else if ((player[i].hasitem == FALSE)) {
											//상대가 아이템이 있을때
											if (player[j].hasitem == TRUE) {
												status_subtraction(copy_item_num[j], j);
												copy_item_num[i] = copy_item_num[j];
												player[j].hasitem = FALSE;
												player[i].hasitem = TRUE;
												player[i].item = use_item[copy_item_num[i]];
												player[i].stamina = player[i].stamina - 20;
												status_pluse(copy_item_num[i], i);
												printf("강탈 성공");
											}
											//상대가 아이템이 없을때
											else {
												printf("강탈 실패");
											}
										}
									}
									else {//실패
										player[i].stamina = player[i].stamina - 40;
										printf("지능부족");
									}
								}
								else {
									printf("스테미나 부족");
								}
							}

							//다시 묻지 않기 위해/////////////////
							ask_num[j] = TRUE;
						}
						else if (player[j].hasitem == TRUE) {
							if (num_1 == 1) {
								//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
								if (player[j].stamina > 0) {
									if (player[j].str > player[i].str) {
										//성공
										//플레이어가 아이템이 있으면 
										if (player[j].hasitem == TRUE) {
											//상대가 아이템이 있을때
											if (player[i].hasitem == TRUE) {
												player[j].stamina = player[j].stamina - 40;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
												use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

												//먹은 아이템을 플레이어가 가진 아이템으로 변경
												use_item[copy_item_num[i]] = player[j].item;

												//플레이어의 아이템을 변경
												player[j].item = use_item[copy_item_num[j]];
												player[i].item = use_item[copy_item_num[i]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//상대가 아이템이 없을때
											else {
												printf("교환 실패");
											}
										}
										//플레이어 아이템이 없으면
										else if ((player[j].hasitem == FALSE)) {
											//상대가 아이템이 있을때
											if (player[i].hasitem == TRUE) {
												status_subtraction(copy_item_num[i], i);
												copy_item_num[j] = copy_item_num[i];
												player[i].hasitem = FALSE;
												player[j].hasitem = TRUE;
												player[j].item = use_item[copy_item_num[j]];
												player[j].stamina = player[j].stamina - 40;
												status_pluse(copy_item_num[j], j);
												printf("강탈 성공");
											}
											//상대가 아이템이 없을때
											else {
												printf("강탈 실패");
											}
										}
									}
									else {//실패
										player[j].stamina = player[j].stamina - 60;
										printf("힘부족");
									}
								}
								else {
									printf("스테미나 부족");
								}
							}
							else if (num_1 == 2) {
								//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
								if (player[j].stamina > 0) {
									if (player[j].intel > player[i].intel) {
										//성공
										//플레이어가 아이템이 있으면 
										if (player[j].hasitem == TRUE) {
											//상대가 아이템이 있을때
											if (player[i].hasitem == TRUE) {
												player[j].stamina = player[j].stamina - 20;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
												use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

												//먹은 아이템을 플레이어가 가진 아이템으로 변경
												use_item[copy_item_num[i]] = player[j].item;

												//플레이어의 아이템을 변경
												player[j].item = use_item[copy_item_num[j]];
												player[i].item = use_item[copy_item_num[i]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//상대가 아이템이 없을때
											else {
												printf("교환 실패");
											}
										}
										//플레이어 아이템이 없으면
										else if ((player[j].hasitem == FALSE)) {
											//상대가 아이템이 있을때
											if (player[i].hasitem == TRUE) {
												status_subtraction(copy_item_num[i], i);
												copy_item_num[j] = copy_item_num[i];
												player[i].hasitem = FALSE;
												player[j].hasitem = TRUE;
												player[j].item = use_item[copy_item_num[j]];
												player[j].stamina = player[j].stamina - 20;
												status_pluse(copy_item_num[j], j);
												printf("강탈 성공");
											}
											//상대가 아이템이 없을때
											else {
												printf("강탈 실패");
											}
										}
									}
									else {//실패
										player[j].stamina = player[j].stamina - 40;
										printf("지능부족");
									}
								}
								else {
									printf("스테미나 부족");
								}
							}

							//다시 묻지 않기 위해
							ask_num[j] = TRUE;
						}
					}
					//둘다 무기가 있는 상황
					else if (player[i].hasitem == TRUE && player[j].hasitem == TRUE || player[i].hasitem == FALSE && player[j].hasitem == FALSE) {
						//둘다 무기가 있거나 없는 상황에서 플레이어 번호가 더 큰순이 결정
						if (i > j) {
							if (num_1 == 1) {
								//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
								if (player[i].stamina > 0) {
									if (player[i].str > player[j].str) {
										//성공
										//플레이어가 아이템이 있으면 
										if (player[i].hasitem == TRUE) {
											//상대가 아이템이 있을때
											if (player[j].hasitem == TRUE) {
												player[i].stamina = player[i].stamina - 40;

												status_subtraction(copy_item_num[i], i);
												status_subtraction(copy_item_num[j], j);
												//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
												use_item[copy_item_num[i]] = use_item[copy_item_num[j]];

												//먹은 아이템을 플레이어가 가진 아이템으로 변경
												use_item[copy_item_num[j]] = player[i].item;

												//플레이어의 아이템을 변경
												player[i].item = use_item[copy_item_num[i]];
												player[j].item = use_item[copy_item_num[j]];
												status_pluse(copy_item_num[i], i);
												status_pluse(copy_item_num[j], j);

											}
											//상대가 아이템이 없을때
											else {
												printf("교환 실패");
											}
										}
										//플레이어 아이템이 없으면
										else if ((player[i].hasitem == FALSE)) {
											//상대가 아이템이 있을때
											if (player[j].hasitem == TRUE) {
												status_subtraction(copy_item_num[j], j);
												copy_item_num[i] = copy_item_num[j];
												player[j].hasitem = FALSE;
												player[i].hasitem = TRUE;
												player[i].item = use_item[copy_item_num[i]];
												player[i].stamina = player[i].stamina - 40;
												status_pluse(copy_item_num[i], i);
												printf("강탈 성공");
											}
											//상대가 아이템이 없을때
											else {
												printf("강탈 실패");
											}
										}
									}
									else {//실패
										player[i].stamina = player[i].stamina - 60;
										printf("힘부족");
									}
								}
								else {
									printf("스테미나 부족");
								}
							}
							else if (num_1 == 2) {
								//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
								if (player[i].stamina > 0) {
									if (player[i].intel > player[j].intel) {
										//성공
										//플레이어가 아이템이 있으면 
										if (player[i].hasitem == TRUE) {
											//상대가 아이템이 있을때
											if (player[j].hasitem == TRUE) {
												player[i].stamina = player[i].stamina - 20;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
												use_item[copy_item_num[i]] = use_item[copy_item_num[j]];

												//먹은 아이템을 플레이어가 가진 아이템으로 변경
												use_item[copy_item_num[j]] = player[i].item;

												//플레이어의 아이템을 변경
												player[i].item = use_item[copy_item_num[i]];
												player[j].item = use_item[copy_item_num[j]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//상대가 아이템이 없을때
											else {
												printf("교환 실패");
											}
										}
										//플레이어 아이템이 없으면
										else if ((player[i].hasitem == FALSE)) {
											//상대가 아이템이 있을때
											if (player[j].hasitem == TRUE) {
												status_subtraction(copy_item_num[j], j);
												copy_item_num[i] = copy_item_num[j];
												player[j].hasitem = FALSE;
												player[i].hasitem = TRUE;
												player[i].item = use_item[copy_item_num[i]];
												player[i].stamina = player[i].stamina - 20;
												status_pluse(copy_item_num[i], i);
												printf("강탈 성공");
											}
											//상대가 아이템이 없을때
											else {
												printf("강탈 실패");
											}
										}
									}
									else {//실패
										player[i].stamina = player[i].stamina - 40;
										printf("지능부족");
									}
								}
								else {
									printf("스테미나 부족");
								}
							}

							//다시 묻지 않기 위해/////////////////
							ask_num[j] = TRUE;
						}
						else if (j > i) {
							if (num_1 == 1) {
								//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
								if (player[j].stamina > 0) {
									if (player[j].str > player[i].str) {
										//성공
										//플레이어가 아이템이 있으면 
										if (player[j].hasitem == TRUE) {
											//상대가 아이템이 있을때
											if (player[i].hasitem == TRUE) {
												player[j].stamina = player[j].stamina - 40;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
												use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

												//먹은 아이템을 플레이어가 가진 아이템으로 변경
												use_item[copy_item_num[i]] = player[j].item;

												//플레이어의 아이템을 변경
												player[j].item = use_item[copy_item_num[j]];
												player[i].item = use_item[copy_item_num[i]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//상대가 아이템이 없을때
											else {
												printf("교환 실패");
											}
										}
										//플레이어 아이템이 없으면
										else if ((player[j].hasitem == FALSE)) {
											//상대가 아이템이 있을때
											if (player[i].hasitem == TRUE) {
												status_subtraction(copy_item_num[i], i);
												copy_item_num[j] = copy_item_num[i];
												player[i].hasitem = FALSE;
												player[j].hasitem = TRUE;
												player[j].item = use_item[copy_item_num[j]];
												player[j].stamina = player[j].stamina - 40;
												status_pluse(copy_item_num[j], j);
												printf("강탈 성공");
											}
											//상대가 아이템이 없을때
											else {
												printf("강탈 실패");
											}
										}
									}
									else {//실패
										player[j].stamina = player[j].stamina - 60;
										printf("힘부족");
									}
								}
								else {
									printf("스테미나 부족");
								}
							}
							else if (num_1 == 2) {
								//강탈 선택시 힘 비교 (스테미나가 0보다 크면) 
								if (player[j].stamina > 0) {
									if (player[j].intel > player[i].intel) {
										//성공
										//플레이어가 아이템이 있으면 
										if (player[j].hasitem == TRUE) {
											//상대가 아이템이 있을때
											if (player[i].hasitem == TRUE) {
												player[j].stamina = player[j].stamina - 20;

												status_subtraction(copy_item_num[j], j);
												status_subtraction(copy_item_num[i], i);
												//즉 사용자가 먹은 아이템의 번호를 가지고와 아이템을 먹은 아이템으로 변경
												use_item[copy_item_num[j]] = use_item[copy_item_num[i]];

												//먹은 아이템을 플레이어가 가진 아이템으로 변경
												use_item[copy_item_num[i]] = player[j].item;

												//플레이어의 아이템을 변경
												player[j].item = use_item[copy_item_num[j]];
												player[i].item = use_item[copy_item_num[i]];
												status_pluse(copy_item_num[j], j);
												status_pluse(copy_item_num[i], i);

											}
											//상대가 아이템이 없을때
											else {
												printf("교환 실패");
											}
										}
										//플레이어 아이템이 없으면
										else if ((player[j].hasitem == FALSE)) {
											//상대가 아이템이 있을때
											if (player[i].hasitem == TRUE) {
												status_subtraction(copy_item_num[i], i);
												copy_item_num[j] = copy_item_num[i];
												player[i].hasitem = FALSE;
												player[j].hasitem = TRUE;
												player[j].item = use_item[copy_item_num[j]];
												player[j].stamina = player[j].stamina - 20;
												status_pluse(copy_item_num[j], j);
												printf("강탈 성공");
											}
											//상대가 아이템이 없을때
											else {
												printf("강탈 실패");
											}
										}
									}
									else {//실패
										player[j].stamina = player[j].stamina - 40;
										printf("지능부족");
									}
								}
								else {
									printf("스테미나 부족");
								}
							}

							//다시 묻지 않기 위해
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
		// 같은 자리가 나오면 다시 생성
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
		// 같은 자리가 나오면 다시 생성
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
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// 움직여서 놓일 자리
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny)) {
		return;
	}

	move_tail(0, nx, ny);
}

// 0 <= dir < 4가 아니면 랜덤
void move_random(int player, int dir) {
	int p = player;  // 이름이 길어서...
	int nx, ny;  // 움직여서 다음에 놓일 자리

	// 움직일 공간이 없는 경우는 없다고 가정(무한 루프에 빠짐)	

	do {
		nx = px[p] + randint(-1, 1);
		ny = py[p] + randint(-1, 1);
	} while (!placable(nx, ny));

	move_tail(p, nx, ny);
}

// back_buf[][]에 기록
void move_tail(int player, int nx, int ny) {
	int p = player;  // 이름이 길어서...
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
		// player 0만 손으로 움직임(4방향)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual(key);
			//플레이어의 움직임을 받으면 아이템이 중복일때 질문을 받도록함
			for (int i = 0; i < 4; i++) {
				item_ask[i] = FALSE;
			}
			//플레이어 마주칠때 질문 묻기
			for (int i = 0; i < 6; i++) {
				ask_num[i] = FALSE;
			}
			//움직일때마다 중간에 메세지 사라짐
			gotoxy(N_ROW + 2, 0);
			printf("                                                            \n");
			printf("                                                             ");
		}

		// player 1 부터는 랜덤으로 움직임(8방향)
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