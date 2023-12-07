#define _CRT_SECURE_NO_WARNINGS
#include "jebi.h"


void jebi() {
	int a=0;
	for (int i = 0; i < 6; i++) {
		if (player[i].is_alive == true && jul_fal[i] == false) {
			a = a + 1;
		}
	}

	JebiRoundEntity round = init(a);
	JebiInputState state;
	JebiRoundResult roundResult;
	while (round.numberOfSurvivedPlayers > 1) {
		configure(&round);
		showJebiBoardWith(round);
		showInteractiveJebiContentsWith(descriptionDeck(round.playerCardDeck));
		state = becomeFirstResponder(&round);
		if (hasQuitCommand(state)) {
			// savePassedPlayers(&round);
			wrappedEndingForDrawWith(&round);
			return;
		}
		roundResult = manageRound(&round);
		showJebiBoardWithDialog(round);
		showRoundResultContents(descriptionRoundResult(&round, roundResult));
		// check current round's state info. uncommend under func.
		 //descriptionRoundState(round);	
		prepareForReuse(&round, roundResult);

	}
	wrappedEndingForWinnerWith(&round);
}

JebiRoundEntity init(int numberOfEntryPlayers) {
	JebiRoundEntity round;
	round.numberOfSurvivedPlayers = round.numberOfPlayers = round.numberOfCurrentPlayers = numberOfEntryPlayers;
	round.currentRound = 1;
	round.currentPlayer = 0;
	round.survivedPlayers = (bool*)malloc(sizeof(bool) * numberOfEntryPlayers);
	for (int index = 0; index < numberOfEntryPlayers; index++) {
		round.survivedPlayers[index] = true;
	}
	round.playerCardDeck = makeJebiCardDeck(round.numberOfPlayers);
	round.randomCardDeck = makeJebiCardDeck(round.numberOfPlayers);
	return round;
}

void configure(JebiRoundEntity* round) {
	toggle(&round->playerCardDeck->cards[0]);
	toggle(&round->randomCardDeck->cards[0]);
	shuffle(round->randomCardDeck, round->numberOfCurrentPlayers);
}

void prepareForReuse(JebiRoundEntity* nextRound, JebiRoundResult result) {
	releasePrevRound(nextRound);
	switch (result) {
	case JebiRoundPass:
		nextRound->numberOfCurrentPlayers = nextRound->numberOfCurrentPlayers - 1;
		while (nextRound->currentPlayer < nextRound->numberOfPlayers) {
			if (nextRound->survivedPlayers[++nextRound->currentPlayer]) { break; }
		}
		break;
	case JebiRoundFail:
		nextRound->numberOfSurvivedPlayers--;
		nextRound->numberOfCurrentPlayers = nextRound->numberOfPlayers - nextRound->currentRound;
		nextRound->currentRound++;
		nextRound->currentPlayer = 0;
		while (nextRound->currentPlayer < nextRound->numberOfPlayers) {
			if (nextRound->survivedPlayers[nextRound->currentPlayer]) { break; }
			nextRound->currentPlayer++;
		}
		break;
	default: break;
	}
	nextRound->playerCardDeck = makeJebiCardDeck(nextRound->numberOfCurrentPlayers);
	nextRound->randomCardDeck = makeJebiCardDeck(nextRound->numberOfCurrentPlayers);
	Sleep(2500);
}

void releasePrevRound(JebiRoundEntity* round) {
	free(round->playerCardDeck->cards);
	free(round->randomCardDeck->cards);
	free(round->playerCardDeck);
	free(round->randomCardDeck);
}

bool hasQuitCommand(JebiInputState state) {
	return state == JebiGameExit;
}

JebiRoundResult manageRound(JebiRoundEntity* round) {
	int userSelectedIndex = selectedCardIndex(round->playerCardDeck);
	/// 시스템이 선택한 카드(hasSelected == true)는 탈락.
	int systemSelectedIndex = selectedCardIndex(round->randomCardDeck);
	if (userSelectedIndex == systemSelectedIndex) {
		round->survivedPlayers[round->currentPlayer] = false;
		return JebiRoundFail;
	}
	return JebiRoundPass;
}

JebiInputState becomeFirstResponder(JebiRoundEntity* round) {
	JebiInputState state;
	do {
		state = input();
		handlePlayerDeckSelection(round->playerCardDeck, state);
		showInteractiveJebiContentsWith(descriptionDeck(round->playerCardDeck));
	} while (state == LeftJebiCardSelection || state == RightJebiCardSelection);
	return state;
}

JebiInputState input() {
	switch (inputKeyLoop()) {
	case K_LEFT: return LeftJebiCardSelection;
	case K_RIGHT: return RightJebiCardSelection;
	case K_QUIT: return JebiGameExit;
	case K_SPACE: return CurrentJebiCardSelection;
	default: return;
	}
}

void handlePlayerDeckSelection(JebiCardDeck* deck, JebiInputState state) {
	int prevSelectedCardIndex = selectedCardIndex(deck);
	// 기존꺼는 선택 취소.
	toggle(&deck->cards[prevSelectedCardIndex]);
	// 방향키에 따라 새로운 제비카드 선택.
	switch (state) {
	case LeftJebiCardSelection:
		if (prevSelectedCardIndex == deck->firstIndex) {
			toggle(&deck->cards[deck->lastIndex]);
			return;
		}
		toggle(&deck->cards[prevSelectedCardIndex - 1]);
		return;
	case RightJebiCardSelection:
		if (prevSelectedCardIndex == deck->lastIndex) {
			toggle(&deck->cards[deck->firstIndex]);
			return;
		}
		toggle(&deck->cards[prevSelectedCardIndex + 1]);
		return;
	case CurrentJebiCardSelection:
		// 선택취소한거취소.
		toggle(&deck->cards[prevSelectedCardIndex]);
		return;
	default: return;
	}
}

void showJebiBoardWith(JebiRoundEntity round) {
	system("cls");
	printf("******************\n");
	for (int i = 0; i < 3; i++) { printf("*                *\n"); }
	printf("******************\n");
	showRoundInfoWith(round);
}

void showInteractiveJebiContentsWith(char* text) {
	Point start = { 2,2 };
	showContents(text, start);
}

void showJebiBoardWithDialog(JebiRoundEntity round) {
	system("cls");
	printf("******************\n");
	printf("**********************\n");
	printf("**                   *\n");
	printf("**********************\n");
	printf("*                *\n");
	printf("******************\n");
	showRoundInfoWith(round);
}

void showRoundResultContents(char* text) {
	Point start = { 3, 2 };
	showContents(text, start);
}

void showContents(char* text, Point startPoint) {
	int count = strlen(text), index = 0;
	while (index < count) {
		gotoxy(startPoint.y, startPoint.x + index);
		printf("%c", text[index++]);
	}
}

void showRoundInfoWith(JebiRoundEntity round) {
	printf("round %d, turn: player: %d", round.currentRound, round.currentPlayer);
}

void wrappedEndingForWinnerWith(JebiRoundEntity* finalRound) {
	int player = -1;
	bool hasSurvivedPlayerFound = false;
	for (int i = 0; i < PLAYER_MAX;) {
		if (!hasSurvivedPlayerFound && finalRound->survivedPlayers[i]) { player = i; hasSurvivedPlayerFound = true; }
		pass_player[i++] = false;
	}
	ending();
	gotoxy(15, 0);
	printf("winner: %d player", player);
	exit(0);
}

void wrappedEndingForDrawWith(JebiRoundEntity* round) {
	bool hasSurvivedPlayerFound = false;
	int i = 0;
	for (; i < PLAYER_MAX;) {
		pass_player[i++] = false;
	}
	ending();
	gotoxy(15, 0);
	printf("Couldn't determine the winner.. Survivor: ");
	for (i = 0; i < round->numberOfPlayers;) {
		if (round->survivedPlayers[i++]) { printf("%dPlayer", i); }
		if (i != round->numberOfCurrentPlayers) { printf(" | "); }
	}
	exit(0);
}

void toggle(JebiCard* jebiCard) {
	switch (jebiCard->icon) {
	case JebiSelectionIcon:
		jebiCard->icon = JebiDeselectionIcon;
		break;
	case JebiDeselectionIcon:
		jebiCard->icon = JebiSelectionIcon;
		break;
	}
	jebiCard->hasSelected = !jebiCard->hasSelected;
}

char* descriptionDeck(JebiCardDeck* deck) {
	const int spacing = 2;
	const int length = deck->numberOfCards + (deck->numberOfCards - 1) * spacing;
	char* text = malloc(length + 1); memset(text, ' ', length);
	text[length] = '\0';
	for (int index = 0; index < length; index++) {
		if (index % 3 == 0) {
			text[index] = deck->cards[index / 3].icon;
		}
	}
	return text;
}

// 주의: 참가자는 인원 10이하일때 해야 좋음.
char* descriptionRoundResult(JebiRoundEntity* round, JebiRoundResult result) {
	const int length = 1 + 2 + 6 + 3 + 5;
	char* text = malloc(length + 1); memset(text, ' ', length);
	char* res = result == JebiRoundPass ? "pass!" : "fail!";
	text[length] = '\0';
	sprintf(
		text,
		"%d  player %d %s",
		round->playerCardDeck->numberOfCards,
		round->currentPlayer,
		res);
	return text;
}

void shuffle(JebiCardDeck* deck) {
	srand((unsigned int)time(NULL));
	for (int i = deck->numberOfCards - 1; i > 0; --i) {
		int j = rand() % (i + 1);
		JebiCard temp = deck->cards[i];
		deck->cards[i] = deck->cards[j];
		deck->cards[j] = temp;
	}
}

JebiCard* makeJebiCards(int numberOfEntryPlayers) {
	JebiCard* cards = (JebiCard*)malloc(numberOfEntryPlayers * sizeof(JebiCard));
	for (int index = 0; index < numberOfEntryPlayers; ++index) {
		cards[index].icon = JebiDeselectionIcon;
		cards[index].hasSelected = false;
	}
	return cards;
}

JebiCardDeck* makeJebiCardDeck(int numberOfEntryPlayers) {
	JebiCardDeck* deck = (JebiCardDeck*)malloc(sizeof(JebiCardDeck));
	deck->numberOfCards = numberOfEntryPlayers;
	deck->firstIndex = 0;
	deck->lastIndex = numberOfEntryPlayers - 1;
	if (numberOfEntryPlayers == 0) {
		deck->lastIndex = deck->firstIndex = -1;
	}
	deck->cards = makeJebiCards(numberOfEntryPlayers);
	return deck;
}

int selectedCardIndex(JebiCardDeck* deck) {
	for (int index = 0; index < deck->numberOfCards; index++) {
		if (deck->cards[index].hasSelected) { return index; }
	}
}

key_t inputKeyLoop(void) {
	while (true) {
		if (_kbhit()) {
			int key = _getch();
			if (key == K_ARROW) { key = _getch(); }
			switch (key) {
			case K_LEFT: case K_RIGHT: case K_QUIT: case K_SPACE: return key;
			default: break;
			}
		}
	}
}

void savePassedPlayers(JebiRoundEntity* round) {
	int i = 0;
	for (; i < PLAYER_MAX;) {
		pass_player[i++] = false;
	}
	for (i = 0; i < round->numberOfPlayers;) {
		if (round->survivedPlayers[i++]) { pass_player[i] = true; }
	}
}

void descriptionRoundState(JebiRoundEntity round) {
	int idx = 0;
	gotoxy(10, 0);
	printf("=====\n");
	printf("numberOfPlayers: %d\n", round.numberOfPlayers);
	printf("numberOfCurrentPlayers: %d\n", round.numberOfCurrentPlayers);
	printf("currentRound: %d\n", round.currentRound);
	printf("currentPlayer: %d\n", round.currentPlayer);
	printf("player's deck info:\t");
	while (idx < round.numberOfCurrentPlayers) { printf("%c ", round.playerCardDeck->cards[idx++]); } idx = 0; printf("\n\n");
	printf("system's deck info:\t");
	while (idx < round.numberOfCurrentPlayers) { printf("%c ", round.randomCardDeck->cards[idx++]); } printf("\n\n"); idx = 0;
	printf("servived players: ");
	while (idx < round.numberOfPlayers) { printf("%dPlayer  ", idx++); } idx = 0; printf("\n                  ");
	while (idx < round.numberOfPlayers) { printf("%s", round.survivedPlayers[idx++] ? "Survived " : "Died     "); }
	printf("\n\n=====\n");
	Sleep(4000);
}