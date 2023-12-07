#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "keyin.h"
#include "canvas.h"
#include "jjuggumi.h"

#ifndef _JEBI_
#define _JEBI_

// TODO:- keyin.h파일과 keyin.c파일에 스페이스바, q키를 새로 추가했음. 나중에 코드 합칠때 확인해야함.

// @ 앞 ? 뒤
typedef enum JebiIconAssets {
	JebiSelectionIcon = '@',
	JebiDeselectionIcon = '?'
}JebiIcon;

//JebiInputState 제비의 입력이 들어올때의 상태
typedef enum {
	LeftJebiCardSelection = 0,
	RightJebiCardSelection = 1,
	CurrentJebiCardSelection = 2,
	JebiGameExit = 3
}JebiInputState;

typedef enum {
	JebiRoundPass = 0,
	JebiRoundFail = 1
}JebiRoundResult;

typedef struct JebiCard {
	JebiIcon icon;
	bool hasSelected;
}JebiCard;

typedef struct JebiCardDeck {
	int numberOfCards;
	int firstIndex;
	int lastIndex;

	JebiCard* cards;
}JebiCardDeck;

typedef struct {
	/// if card's hasSelected == true, it indicates a JebiRoundFail as JebiRoundResult type.
	JebiCardDeck* randomCardDeck;
	JebiCardDeck* playerCardDeck;

	bool* survivedPlayers;
	/// Players who survive round
	int numberOfSurvivedPlayers;
	int numberOfPlayers;
	int numberOfCurrentPlayers;

	/// Player who execute current jebi round
	int currentPlayer;
	int currentRound;
}JebiRoundEntity;



void jebi();

// Prepare and configure for prev and next round.
JebiRoundEntity init(int numberOfEntryPlayers);
void configure(JebiRoundEntity* jebiRound);
void prepareForReuse(JebiRoundEntity* nextRound, JebiRoundResult result);
void releasePrevRound(JebiRoundEntity* round);

// User interactive and round managing functions
bool hasQuitCommand(JebiInputState state);
JebiRoundResult manageRound(JebiRoundEntity* round);

JebiInputState becomeFirstResponder(JebiRoundEntity* round);
JebiInputState input();

void handlePlayerDeckSelection(JebiCardDeck* deck, JebiInputState state);

/** ****  Draw Functions  **** **/
void showJebiBoardWith(JebiRoundEntity round);
void showInteractiveJebiContentsWith(char* text);

void showJebiBoardWithDialog(JebiRoundEntity round);
void showRoundResultContents(char* text);

/** ****  Draw common functions  **** **/
void showContents(char* text, Point startPoint);
void showRoundInfoWith(JebiRoundEntity round);

void wrappedEndingForWinnerWith(JebiRoundEntity* finalRound);
void wrappedEndingForDrawWith(JebiRoundEntity* round);


/** ****  Common functions  **** **/
void toggle(JebiCard* jebiCard);
char* descriptionDeck(JebiCardDeck* deck);
char* descriptionRoundResult(JebiRoundEntity* round, JebiRoundResult result);
void shuffle(JebiCardDeck* deck);
JebiCard* makeJebiCards(int numberOfEntryPlayers);
JebiCardDeck* makeJebiCardDeck(int numberOfEntryPlayers);
int selectedCardIndex(JebiCardDeck* deck);
key_t inputKeyLoop(void);

/// If you press 'q' when there are two or more players and want to proceed with another minigame,
///   use this function to save the players who survived the current round.
void savePassedPlayers(JebiRoundEntity* round);

/// test.
/// if wanna see current round struct's state, uncomment in jebi() func's descriptionRoundState(round);
void descriptionRoundState(JebiRoundEntity round);
#endif _JEBI_