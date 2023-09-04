#pragma once
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define ENTER 13

#define SPADE "|♠    |"
#define CLUB "|♣    |"
#define DIA "|◈    |"
#define HEART "|♥    |"

#define cls system("cls")

enum shape { spade, club, dia, heart };
enum num { jack = 11, queen = 12, king = 13, ace = 1 };

struct Cursor {
	int x, y;
	const char* shape;
	int curPos = 0;		//메인 커서의 위치
};
struct Card {
	int number;
	char shape;
};
struct Player {
	int money;	//가진 현금
	int score;	//21점까지의 점수 리미트
	int moneyHighScore;	//하이스코어
	struct Card cardPlayer[21];
};

struct Card allCard[52];
int next;
int turns = 0;

struct Player player;
void ShowCard() {

	int aceScore = 0;
	int xPos = turns + 1 * 10;
	int yTop = 60;
	int yTopMiddle = 61;
	int yMiddle = 62;
	int yBottomMiddle = 63;
	int yBottom = 64;
	char pShape = player.cardPlayer[turns].shape;
	char pNumber = player.cardPlayer[turns].number;
	ShowBuffer(xPos, yTop, "-------");
	switch (pShape) {
	case spade:
		ShowBuffer(xPos, yTopMiddle, SPADE);
		break;
	case club:
		ShowBuffer(xPos, yTopMiddle, CLUB);
		break;
	case dia:
		ShowBuffer(xPos, yTopMiddle, DIA);
		break;
	case heart:
		ShowBuffer(xPos, yTopMiddle, HEART);
		break;
	default:
		ShowBuffer(xPos, yTopMiddle, "ERROR");
		break;
	}
	switch (pNumber) {
	case ace:
		ShowBuffer(xPos, yMiddle, "|  A  |");
		aceScore = -1;
		break;
	case jack:
		ShowBuffer(xPos, yMiddle, "|  J  |");
		player.score += 10;
		break;
	case queen:
		ShowBuffer(xPos, yMiddle, "|  Q  |");
		player.score += 10;
		break;
	case king:
		ShowBuffer(xPos, yMiddle, "|  K  |");
		player.score += 10;
		break;
	case 2:
		ShowBuffer(xPos, yMiddle, "|  2  |");
		player.score += 2;
		break;
	case 3:
		ShowBuffer(xPos, yMiddle, "|  3  |");
		player.score += 3;
		break;
	case 4:
		ShowBuffer(xPos, yMiddle, "|  4  |");
		player.score += 4;
		break;
	case 5:
		ShowBuffer(xPos, yMiddle, "|  5  |");
		player.score += 5;
		break;
	case 6:
		ShowBuffer(xPos, yMiddle, "|  6  |");
		player.score += 6;
		break;
	case 7:
		ShowBuffer(xPos, yMiddle, "|  7  |");
		player.score += 7;
		break;
	case 8:
		ShowBuffer(xPos, yMiddle, "|  8  |");
		player.score += 8;
		break;
	case 9:
		ShowBuffer(xPos, yMiddle, "|  9  |");
		player.score += 9;
		break;
	case 10:
		ShowBuffer(xPos, yMiddle, "|  10  |");
		player.score += 10;
		break;
	default:
		ShowBuffer(xPos, yTopMiddle, "ERROR");
		break;
	}
	switch (pShape) {
	case spade:
		ShowBuffer(xPos, yBottomMiddle, SPADE);
		break;
	case club:
		ShowBuffer(xPos, yBottomMiddle, CLUB);
		break;
	case dia:
		ShowBuffer(xPos, yBottomMiddle, DIA);
		break;
	case heart:
		ShowBuffer(xPos, yBottomMiddle, HEART);
		break;
	default:
		ShowBuffer(xPos, yTopMiddle, "ERROR");
		break;
	}

	ShowBuffer(xPos, yBottom, "-------");

	return;
}