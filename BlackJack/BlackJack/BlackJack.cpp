#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <stralign.h>

#include "defaultMain.h"
#include "doubleBuffering.h"


#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define ENTER 13

#define SPADE "|♠    |"
#define CLUB "|♣    |"
#define DIA "|◈    |"
#define HEART "|♥    |"

#define RSPADE "|    ♠|"
#define RCLUB "|    ♣|"
#define RDIA "|    ◈|"
#define RHEART "|    ♥|"

#define cls system("cls")

#define MAX_HIGHSCORE 10

enum shape {spade,club,dia,heart};
enum num {jack = 11, queen = 12, king = 13, ace = 1};

struct Cursor {
	int x, y;
	const char* shape;
	int curPos = 0;		//메인 커서의 위치
};
struct Card {
	int number;
	char shape;
};
struct SaveCard {
	int saveNum[21];
	char saveShape[21];
};
struct Player {
	int money = 100;	//가진 현금
	int score = 0 ;	//21점까지의 점수 리미트
	int moneyHighScore = 0;	//하이스코어
	int pTurn = 0;		//플레이어 턴
	int played = 0;
	struct Card cardPlayer[21];		//손패를 나타냄
};

struct Dealer {
	int score;
	int dTurn = 0;
	struct Card cardPlayer[21];
};

struct Card allCard[52];
int next;		//카드 나눠주는 과정 추적

struct Player player;
struct Dealer dealer;
struct SaveCard saveCard;

Cursor cursor = { 7,4,"▶",0 };

typedef struct ScoreEntry {
	char timeString[20];
	int highScore;
};

//메인 함수 내 
int menuPos = 3;	//0 - 게임시작, 1 - 랭크 확인 2 - 게임 종료 3 - 메인메뉴 로고표시, 5~9 - 시작 진입 후 메뉴
					//11~13 - 게임내 선택지, 14 - 게임끝 결과창 , 15 - 버스트로 패배, 16~18 - 게임끝 선택지
int curBet = 0;		//5~8 : 5 10 25 50 달러씩 저장하는 함수
int doubleCount = 0;	//더블을 몇번 했는지 추적
int rLoop = 1;		//리셋 루프
int cLoop = 1;		//카드 주기 루프
int bloop = 1;		//베팅금 루프
int stakes = 0;		//중간책이 가지고있을 돈 (승패에 따라 플레이어 or 딜러에게 돈이 감)

/// <summary>
/// 키보드에 반응에 따라 커서가 움직이는 함수다.
/// 커서가 메뉴판에 따라 또는 플레이어의 돈의 현황에 따라 움직일 수 있는 커서의 범위를 정한다.
/// </summary>
/// <param name="cursor"> 인게임 커서 지정</param>
void MainKeyboard(Cursor* cursor) {
	char key = 0;

	if (_kbhit()) {
		key = _getch();

		if (key == -32) {
			key = _getch();
		}
		switch (key) {
		case RIGHT: if (cursor->curPos == 2) cursor->curPos = 0;
				  else if (cursor->curPos == 9) cursor->curPos = 5;
				  else if (cursor->curPos == 13) cursor->curPos = 11;
				  else if (cursor->curPos == 11 && player.money <= 0) cursor->curPos = 13;
				  else if (cursor->curPos == 18) cursor->curPos = 16;
				  else cursor->curPos += 1;
			break;
		case LEFT: if (cursor->curPos == 0) cursor->curPos = 2;
				 else if (cursor->curPos == 5) cursor->curPos = 9;
				 else if (cursor->curPos == 11) cursor->curPos = 13;
				 else if (cursor->curPos == 13 && player.money <= 0) cursor->curPos = 11;
				 else if (cursor->curPos == 16) cursor->curPos = 18;
				 else cursor->curPos -= 1;
			break;
		}
	}
}

/// <summary>
/// 게임 내 프레임 설정을 설정한다. 이 프레임은 게임 내내 고정되어 움직이지 않는다.
/// </summary>
void GameFrame() {
	if (menuPos == 3) {
		system("title 딜러 블랙 잭 : 메인 화면");
	}
	else if (menuPos == 0) {
		system("title 딜러 블랙 잭 : 판돈 선택");
	}
	else if (menuPos == 1) {
		system("title 딜러 블랙 잭 : 하이스코어 확인");
	}
	else if (menuPos >= 5 && menuPos <= 9) {
		system("title 딜러 블랙 잭 : 플레이어 차례");
	}
	else if (menuPos == 13) {
		system("title 딜러 블랙 잭 : 딜러 차례");
	}
	else if (menuPos == 14 || menuPos == 15) {
		system("title 딜러 블랙 잭 : 결과 확인");
	}
	else if (menuPos == 16) {
		system("title 딜러 블랙 잭 : 다음 판 진행");
	}
#pragma region 더블 버퍼링
	ShowBuffer(0, 0, "●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎");
	ShowBuffer(0, 1, "◎                                                                                                            ●");
	ShowBuffer(0, 2, "●                                                                                                            ◎");
	ShowBuffer(0, 3, "◎                                                                                                            ●");
	ShowBuffer(0, 4, "●                                                                                                            ◎");
	ShowBuffer(0, 5, "◎                                                                                                            ●");
	ShowBuffer(0, 6, "●                                                                                                            ◎");
	ShowBuffer(0, 7, "◎                                                                                                            ●");
	ShowBuffer(0, 8, "●                                                                                                            ◎");
	ShowBuffer(0, 9, "◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●");
	
	for (int i = 10; i < 40; i++) {
		if (i % 2 == 0) {
			ShowBuffer(0, i, "●                                                                                                            ◎");
		}
		else if (i % 2 == 1) {
			ShowBuffer(0, i, "◎                                                                                                            ●");
		}
	}
	ShowBuffer(0, 40, "●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎");

#pragma endregion
	return;
}

#pragma region 메인메뉴
/// <summary>
/// 게임을 첫 실행할 시 보이는 로고를 반환해주는 함수
/// </summary>
void GameLogoMenu() {
	ShowBuffer(2, 14, "                ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ9W          8ZZZZZZZZZZZZZZZZZZZZZZZZZ              ");
	ShowBuffer(2, 15, "               ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ    EZZZZZZZZZZZZZZZZZZZZZZZZZZZZ               ");
	ShowBuffer(2, 16, "              ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ                ");
	ShowBuffer(2, 17, "             EBw,,,,,,,,,,,,,,,,,,,,,,,WyyZZZZZZZZZZZZZZZZZZzjW,,,,,,,,,,  5ZZZZZZZZ                ");
	ShowBuffer(2, 18, "                                            BZZZZZZZZZZZZ                  ZZZZZZZZ                 ");
	ShowBuffer(2, 19, "                        8ZZZZZZZZ            ZZZZZZZZZZ5                  ZZZZZZZZz                 ");
	ShowBuffer(2, 20, "                        ZZZZZZZZ            yZZZZZZZZZZ   EZZZZZZZ        ZZZZZZZZ                  ");
	ShowBuffer(2, 21, "                       ZZZZZZZZ5           5ZZZZZZZZZZZ  ZZZZZZZZZZB     ZZZZZZZZ                   ");
	ShowBuffer(2, 22, "                       ZZZZZZZZ          BZZZZZZZ  ZZZZDZZZZZZZZZZZZ    ,ZZZZZZZZ                   ");
	ShowBuffer(2, 23, "                      ZZZZZZZZ        8ZZZZZZZ     yZZZZZZZZZZZZZZZZ    ZZZZZZZZ                    ");
	ShowBuffer(2, 24, "                     WZZZZZZZZ      ZZZZZZZZ         ZZZZZZZZZZZZZZy   EZZZZZZZ9                    ");
	ShowBuffer(2, 25, "                     ZZZZZZZZ       ZZZZZZZZZ          8ZZZZZZZZZ9     ZZZZZZZZ                     ");
	ShowBuffer(2, 26, "                    ZZZZZZZZD        BZZZZZZZZ                        ZZZZZZZZ                      ");
	ShowBuffer(2, 27, "                    ZZZZZZZZ          ZZZZZZZZ                       ,ZZZZZZZZ                      ");
	ShowBuffer(2, 28, "                   ZZZZZZZZ          DZZZZZZZD                       ZZZZZZZZ                       ");
	ShowBuffer(2, 29, "                  ,ZZZZZZZE         ZZZZZZZZZ                       ZZZZZZZZ                        ");
	ShowBuffer(2, 30, "                  ZZZZZZZZ  yj5ZZZZZZZZZZZZ           WEZZz         ZZZZZZZ                         ");
	ShowBuffer(2, 31, "                 ZZZZZZZZZZZZZZZZZZZZZZZW           ZZZZZZZZZz     ZZZZZZZ                          ");
	ShowBuffer(2, 32, "                 ZZZZZZZZZZZZZZZ95j                ZZZZZZZZZZZZ   ZZZZZZZ                           ");
	ShowBuffer(2, 33, "                                                   ZZZZZZZZZZZZ ZZZZZZZ,                            ");
	ShowBuffer(2, 34, "                                                   ZZZZZZZZZZZZZZZZZZw                              ");
	ShowBuffer(2, 35, "                                                    ZZZZZZZZZZZZZZ8                                 ");
	ShowBuffer(2, 36, "                                                       9ZZZZZZB	                                   ");

	/*    10
		  11
		  12
		  13
		  14
		  15
				ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ9W          8ZZZZZZZZZZZZZZZZZZZZZZZZZ
			   ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ    EZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
			  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
			 EBw,,,,,,,,,,,,,,,,,,,,,,,WyyZZZZZZZZZZZZZZZZZZzjW,,,,,,,,,,  5ZZZZZZZZ
											BZZZZZZZZZZZZ                  ZZZZZZZZ
						8ZZZZZZZZ            ZZZZZZZZZZ5                  ZZZZZZZZz
						ZZZZZZZZ            yZZZZZZZZZZ   EZZZZZZZ        ZZZZZZZZ
					   ZZZZZZZZ5           5ZZZZZZZZZZZ  ZZZZZZZZZZB     ZZZZZZZZ
					   ZZZZZZZZ          BZZZZZZZ  ZZZZDZZZZZZZZZZZZ    ,ZZZZZZZZ
					  ZZZZZZZZ        8ZZZZZZZ     yZZZZZZZZZZZZZZZZ    ZZZZZZZZ
					 WZZZZZZZZ      ZZZZZZZZ         ZZZZZZZZZZZZZZy   EZZZZZZZ9
					 ZZZZZZZZ       ZZZZZZZZZ          8ZZZZZZZZZ9     ZZZZZZZZ
					ZZZZZZZZD        BZZZZZZZZ                        ZZZZZZZZ
					ZZZZZZZZ          ZZZZZZZZ                       ,ZZZZZZZZ
				   ZZZZZZZZ          DZZZZZZZD                       ZZZZZZZZ
				  ,ZZZZZZZE         ZZZZZZZZZ                       ZZZZZZZZ
				  ZZZZZZZZ  yj5ZZZZZZZZZZZZ           WEZZz         ZZZZZZZ
				 ZZZZZZZZZZZZZZZZZZZZZZZW           ZZZZZZZZZz     ZZZZZZZ
				 ZZZZZZZZZZZZZZZ95j                ZZZZZZZZZZZZ   ZZZZZZZ
												   ZZZZZZZZZZZZ ZZZZZZZ,
												   ZZZZZZZZZZZZZZZZZZw
													ZZZZZZZZZZZZZZ8
													   9ZZZZZZB




																							*/
}

/// <summary>
/// 게임을 실행할 때 볼 수있는 게임 메뉴 선택지를 반환하는 함수
/// </summary>
void GameMenu() {
	ShowBuffer(10, 4, "게임 시작");
	ShowBuffer(50, 4, "랭킹 확인");
	ShowBuffer(90, 4, "게임 종료");
}

/// <summary>
/// 게임 시작 후 베팅 금액 패널을 반환하는 함수
/// </summary>
void SelectMoney() {
	ShowBuffer(10, 24, "$5");		//curpos = 5
	ShowBuffer(30, 24, "$10");		//6
	ShowBuffer(50, 24, "$25");		//7	
	ShowBuffer(70, 24, "$50");		//8
	ShowBuffer(90, 24, "EXIT");		//9	-> 다시 0으로		//
}

/// <summary>
/// 임이 끝난 후 랭크를 보여주는 패널(개발해야함)을 반환하는 함수
/// </summary>
void ShowRank() {
	int loop = 1;
	while (loop) {
		FILE* file;
		file = fopen("HighScore.txt", "a+");
		if (file == NULL) {
			ShowBuffer(46, 13, "당신의 최고 기록");
			return;
		}
		char line[100];
		int rCount = 0;

		ShowBuffer(46, 13, "당신의 최고 기록");

		while (rCount < 20 && fgets(line, sizeof(line), file) != NULL) {
			sprintf(line, "%s", line);
			ShowBuffer(40, rCount + 18, line);
			rCount = rCount + 2;
		}

		fclose(file);
		
		loop = 0;
	}
	
}
#pragma endregion


#pragma region 게임 내 함수
/// <summary>
/// 덱에 카드를 만드는 함수. 모양 4개와 숫자 9개 기호카드 4개로 이루어져 있다.
/// </summary>
void Deck(){
	int i = 0;
	for (int j = 0; j < 4; j++) {		//모양 4개
		for (int k = 1; k < 14; k++) {	//숫자 1~13(11~13 = 10)
			allCard[i].number = k;
			allCard[i].shape = j;
			i++;
		}
	}
}

/// <summary>
/// 카드를 랜덤으로 섞어 주는 함수
/// </summary>
void Shuffle() {		//카드 랜덤 셔플
	srand(time(NULL));
	for (int i = 0; i < 52; i++) {
		int index = rand() % 52;
		struct Card temp = allCard[index];
		allCard[index] = allCard[i];
		allCard[i] = temp;
	}
}

struct Card Dealing(int next) {
	return allCard[next];
}

/// <summary>
/// 플레이어의 카드를 반환해주는 함수, 턴이 지날 때 마다 x값이 증가하여 카드를 옆으로 늘여 플레이어에게 보여준다.
/// </summary>
/// <param name="turns">카드를 받은 턴</param>
void ShowCard(int turns) {
	
	int aceScore = 0;
	int xPos = (turns + 1) * 8 ;
	int yTop = 33;
	int yTopMiddle = 34;
	int yMiddle = 35;
	int yBottomMiddle = 36;
	int yBottom = 37;
	char pShape = player.cardPlayer[turns].shape;
	char pNumber = player.cardPlayer[turns].number;




	ShowBuffer(xPos, yTop,"-------");

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
		ShowBuffer(xPos, yMiddle, "|  A   |");
		break;					
	case jack:					
		ShowBuffer(xPos, yMiddle, "|  J   |");
		break;					
	case queen:					
		ShowBuffer(xPos, yMiddle, "|  Q   |");
		break;					
	case king:					
		ShowBuffer(xPos, yMiddle, "|  K   |");
		break;					
	case 2:						
		ShowBuffer(xPos, yMiddle, "|  2   |");
		break;					
	case 3:						
		ShowBuffer(xPos, yMiddle, "|  3   |");
		break;					
	case 4:						
		ShowBuffer(xPos, yMiddle, "|  4   |");
		break;					
	case 5:						
		ShowBuffer(xPos, yMiddle, "|  5   |");
		break;					
	case 6:						
		ShowBuffer(xPos, yMiddle, "|  6   |");
		break;					
	case 7:						
		ShowBuffer(xPos, yMiddle, "|  7   |");
		break;					
	case 8:						
		ShowBuffer(xPos, yMiddle, "|  8   |");
		break;					
	case 9:						
		ShowBuffer(xPos, yMiddle, "|  9   |");
		break;
	case 10:
		ShowBuffer(xPos, yMiddle, "|  10  |");
		break;
	default:
		ShowBuffer(xPos, yMiddle, "|ERROR |");
		break;
	}

	switch (pShape) {
	case spade:
		ShowBuffer(xPos, yBottomMiddle, RSPADE);
		break;					
	case club:					
		ShowBuffer(xPos, yBottomMiddle, RCLUB);
		break;					
	case dia:					
		ShowBuffer(xPos, yBottomMiddle, RDIA);
		break;					
	case heart:					
		ShowBuffer(xPos, yBottomMiddle, RHEART);
		break;
	default:
		ShowBuffer(xPos, yBottomMiddle, "ERROR");
		break;
	}

	ShowBuffer(xPos, yBottom, "-------");

	return;
}

/// <summary>
/// 딜러의 카드를 반환해주는 함수, 턴이 지날 때 마다 x값이 증가하여 카드를 옆으로 늘여 딜러에게 보여준다.
/// </summary>
/// <param name="turns">카드를 받은 턴</param>
void ShowDealerCard(int turns) {

	int aceScore = 0;
	int xPos = (turns + 1) * 8;
	int yTop = 23;
	int yTopMiddle = 24;
	int yMiddle = 25;
	int yBottomMiddle = 26;
	int yBottom = 27;
	char pShape = dealer.cardPlayer[turns].shape;
	char pNumber = dealer.cardPlayer[turns].number;




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
		ShowBuffer(xPos, yMiddle, "|  A   |");
		break;
	case jack:
		ShowBuffer(xPos, yMiddle, "|  J   |");
		break;
	case queen:
		ShowBuffer(xPos, yMiddle, "|  Q   |");
		break;
	case king:
		ShowBuffer(xPos, yMiddle, "|  K   |");
		break;
	case 2:
		ShowBuffer(xPos, yMiddle, "|  2   |");
		break;
	case 3:
		ShowBuffer(xPos, yMiddle, "|  3   |");
		break;
	case 4:
		ShowBuffer(xPos, yMiddle, "|  4   |");
		break;
	case 5:
		ShowBuffer(xPos, yMiddle, "|  5   |");
		break;
	case 6:
		ShowBuffer(xPos, yMiddle, "|  6   |");
		break;
	case 7:
		ShowBuffer(xPos, yMiddle, "|  7   |");
		break;
	case 8:
		ShowBuffer(xPos, yMiddle, "|  8   |");
		break;
	case 9:
		ShowBuffer(xPos, yMiddle, "|  9   |");
		break;
	case 10:
		ShowBuffer(xPos, yMiddle, "|  10  |");
		break;
	default:
		ShowBuffer(xPos, yMiddle, "|NEXT? |");
		break;
	}

	switch (pShape) {
	case spade:
		ShowBuffer(xPos, yBottomMiddle, RSPADE);
		break;
	case club:
		ShowBuffer(xPos, yBottomMiddle, RCLUB);
		break;
	case dia:
		ShowBuffer(xPos, yBottomMiddle, RDIA);
		break;
	case heart:
		ShowBuffer(xPos, yBottomMiddle, RHEART);
		break;
	default:
		ShowBuffer(xPos, yBottomMiddle, "ERROR");
		break;
	}

	ShowBuffer(xPos, yBottom, "-------");

	return;
}

/// <summary>
/// 게임을 첫 실행시 또는 파산 후 첫 게임을 플레이를 할 때 실행되는 함수
/// </summary>
void Reset() {		//게임 첫 시작시 할것
	player.score = 0;		//플레이어 점수 0
	player.pTurn = 0;		//턴 진행수

	dealer.score = 0;		//딜러
	dealer.dTurn = 0;

	player.played = 0;		//게임 플레이 횟수
	player.moneyHighScore = 0;		//돈의 최고점이 점수임(초기화)
	player.money = 100;		//기본 지급하는 돈

	next = 0;				//카드 돌린 횟수
	doubleCount = 0;
	stakes = 0;
	
	Deck();
	Shuffle();
}

/// <summary>
/// 게임의 첫 판이 끝나고 간단하게 초기화를 해주는 함수
/// </summary>
void SoftReset() {
	player.score = 0;		//플레이어
	player.pTurn = 0;

	dealer.score = 0;		//딜러
	dealer.dTurn = 0;

	for (int i = 0; i < 21; i++) {
		player.cardPlayer[i].number = 1;
		dealer.cardPlayer[i].number = 0;
	}

	next = 0;
	doubleCount = 0;
	stakes = 0;

	Deck();
	Shuffle();
}

/// <summary>
/// 게임 중 플레이어가 해야할 행동을 보여주는 함수
/// </summary>
void ChooseAct() {
	ShowBuffer(10, 4, "Hit");
	ShowBuffer(50, 4, "Double");
	ShowBuffer(90, 4, "STOP");	
}

/// <summary>
/// 게임에 첫 입장 시 플레이어의 돈을 차감하면서, 딜러의 돈도 그와 같이 stakes에 추가해주는 함수
/// </summary>
void EntryBet() {
	if (curBet == 5) {
		player.money -= 5;
		stakes += 10;
	}
	else if (curBet == 6) {
		player.money -= 10;
		stakes += 20;
	}
	else if (curBet == 7) {
		player.money -= 25;
		stakes += 50;
	}
	else if (curBet == 8) {
		player.money -= 50;
		stakes += 100;
	}
}

/// <summary>
/// 플레이어의 베팅에 따라 늘어나는 금액에 대한 함수. 첫 Double만 첫 판의 판돈을 더 내며 두번째 Double부터는 그 금액의 2배씩 늘어난다.
/// </summary>
void DoubleBet() {
	if (doubleCount == 1) {
		player.money -= stakes / 2;
		stakes *= 2;
	}
	else if (doubleCount >= 2) {
		player.money -= stakes / 2;
		stakes *= 2;
	}
}

/// <summary>
/// 게임이 끝난 후 베팅금에 따라 플레이어의 돈을 증감하는 함수
/// </summary>
/// <param name="wl">승패 여부(0 = 패배, 1 = 승리, 2 = 무승부) </param>
void BetResult(int wl) {
	//wl == 1 -> 승 , wl == 0 -> 패
	int totBet = 0;		//총 금액
	while (bloop) {
		if (wl == 1) {
			if (player.score == 21 && player.pTurn == 1) {
				player.money += stakes * 2;
			}
			else {
				player.money += stakes;
			}
			if (player.money > player.moneyHighScore) {
				player.moneyHighScore = player.money;
			}
		}
		else if (wl == 0) {
			NULL;
		}
		else if (wl == 2){
			player.money += stakes / 2;
		}
		bloop = 0;
	}
}

/// <summary>
/// 버스트가 아닌 게임의 결과를 반환하는 함수
/// </summary>
/// <param name="loop">를 1로 설정 시 베팅금에 대한 정산을 한번만 할수 있게 함</param>
void ShowResult(int loop) {
	if (player.score > dealer.score && player.score < 22 || dealer.score > 21) {		//딜러보다 점수가 크거나 딜러가 버스트 했을 시
		ShowBuffer(10, 3, "승리했습니다. 게임을 계속 하시겠습니까?");
		while (loop) {
			if (player.money >= 0) {
				BetResult(1);
			}
			loop = 0;
		}
	}
	else if (player.score < dealer.score && dealer.score < 22) {				//플레이어 스코어 보다 딜러 스코어가 커야하고,
		ShowBuffer(10, 3, "패배했습니다. 게임을 계속 하시겠습니까?");				//플레이어는 버스트시 결과를 보지 않고 바로 졌다고 해야함
		while (loop) {
			if (player.money > 0) {
				BetResult(0);
			}
			loop = 0;
		}
	}
	else if (player.score == dealer.score && player.score < 22) {				//비겼을 때
		ShowBuffer(10, 3, "비겼습니다. 판돈은 초기화 됩니다. 게임을 계속 하시겠습니까?");
		while (loop) {
			if (player.money >= 0) {
				BetResult(2);
			}
			loop = 0;
		}
	}
	else {
		ShowBuffer(10, 3, "디버깅중... (오류)게임을 계속 하시겠습니까?");
	}
}

/// <summary>
/// 버스트, 블랙잭으로 인한 게임 결과를 반환하는 함수
/// </summary>
/// <param name="loop">를 1로 설정 시 베팅금에 대한 정산을 한번만 할수 있게 함</param>
void BurstStop(int loop) {
	if (player.score != 21) {
		ShowBuffer(10, 3, "버스트! 패배했습니다. 게임을 계속 하시겠습니까?");
		while (loop) {
			if (player.money > 0) {
				BetResult(0);
			}
			loop = 0;
		}
	}
	else {
		ShowBuffer(10, 3, "블랙잭! 승리했습니다. 게임을 계속 하시겠습니까?");
		while (loop) {
			if (player.money >= 0) {
				BetResult(1);
			}
			loop = 0;
		}
	}
	//SelectRestart();
}

/// <summary>
/// 게임이 끝난 후 플레이어에게 선택지를 주는 함수.
/// </summary>
void SelectRestart() {
	if (player.money > 0) {
		ShowBuffer(10, 4, "계속하기");
	}
	else {
		ShowBuffer(10, 4, "파산! 진행 불가!!");
	}

	ShowBuffer(50, 4, "랭킹 등록 후 나가기");
	ShowBuffer(90, 4, "바로 나가기");
}

/// <summary>
/// 현재 플레이어가 알수 있는 정보를 반환하는 함수
/// </summary>
void InGameInfo() {
	char stakesString[20];
	sprintf(stakesString, "  현재 판돈 : %d", stakes);
	ShowBuffer(4, 12, stakesString);

	char highScoreString[20];
	sprintf(highScoreString, "하이 스코어 : %d", player.moneyHighScore);
	ShowBuffer(4, 13, highScoreString);

	char pMoneyString[20];		//턴 증가 확인
	sprintf(pMoneyString, "현재 소지금 : %d", player.money);
	ShowBuffer(4, 14, pMoneyString);
	
	char pScoreString[20];		//플레이어 점수 증가 확인
	sprintf(pScoreString, "  나의 점수 : %d", player.score);
	ShowBuffer(4, 31, pScoreString);
	if (player.score > 21) {
		ShowBuffer(23, 31, "!!BURST!!");
	}

	char dScoreString[20];		//딜러 점수 증가 확인
	sprintf(dScoreString, "딜러의 점수 : %d", dealer.score);
	ShowBuffer(4, 21, dScoreString);
	if (dealer.score > 21) {
		ShowBuffer(23, 21, "!!BURST!!");
	}

	char playedString[20];
	sprintf(playedString, "총 게임: %d", player.played);
	ShowBuffer(4, 17, playedString);
}

/// <summary>
/// 커서가 어디 있는지 알수 있는 함수. 게임의 흐름도를 디버깅으로 알 수 있게 한다
/// </summary>
void TestCursor() {
	char turnString[20];		//턴 증가 확인
	sprintf(turnString, "Turn: %d", player.pTurn);
	ShowBuffer(4, 13, turnString);

	char cursorString[20];
	sprintf(cursorString, "curpos : %d", cursor.curPos);
	ShowBuffer(4, 18, cursorString);

	char menuPosString[20];
	sprintf(menuPosString, "menuPos : %d", menuPos);
	ShowBuffer(4, 21, menuPosString);
}

/// <summary>
/// 현재 카드를 뿌린 횟수와 판돈을 얼마 걸었는지 디버그로 확인하는 함수.
/// </summary>
void TestTurn() {
	char nextString[20];		//몇장 줬나 확인
	sprintf(nextString, "next : %d", next);
	ShowBuffer(4, 16, nextString);		// 이걸 수정할것 (230901) , rLoop를 걸어놔서 그랬음

	char curBetString[20];
	sprintf(curBetString, "curBet : %d", curBet);
	ShowBuffer(4, 19, curBetString);
}

/// <summary>
/// 카드를 나열하는 함수. 플레이어는 첫 턴에 2장, 딜러는 한장만 보이게 한다.
/// </summary>
void ListingCard() {		//카드 표기 현황
	ShowCard(0);
	for (int i = 1; i <= 10 && player.pTurn >= i; i++) {
		ShowCard(i);
	}
	for (int i = 0; i <= 10 && dealer.dTurn >= i; i++) {
		ShowDealerCard(i);
	}
	InGameInfo();
	//TestTurn();
}

/// <summary>
/// 플레이어가 Hit 했을 시 작동하는 함수
/// </summary>
/// <param name="loop">를 1로 설정 시 엔터 입력 후 단 한번만 실행한다.</param>
void Hit(int loop) {

	while (loop) {
		player.pTurn++;
		menuPos = curBet;
		rLoop = 0;
		cLoop = 1;
		loop = 0;
	}
}

/// <summary>
/// 플레이어가 Double 했을 시 작동하는 함수
/// </summary>
/// <param name="loop">를 1로 설정 시 엔터 입력 후 단 한번만 실행한다.</param>
void Double(int loop) {
	while (loop) {
		cursor.curPos = 11;
		player.pTurn++;
		menuPos = curBet;
		doubleCount++;
		DoubleBet();
		rLoop = 0;
		cLoop = 1;
		loop = 0;
	}
}

/// <summary>
/// 플레이어의 게임이 끝나고 딜러가 플레이 해야할 때 작동하는 함수.
/// 딜러의 특정 조건이 만족할 때 까지 무한루프가 돌 것임.
/// </summary>
void Stop() {		
	int loop = 1;
	while (loop) {
		if (dealer.score <= 16) {		//딜러가 16점 이하일때
			dealer.cardPlayer[dealer.dTurn] = Dealing(next++);		//카드 뽑고
			if (dealer.cardPlayer[dealer.dTurn].number > 10) {		//10점 넘으면
				dealer.score = dealer.score + 10;					//10점만 추가
			}
			else if (dealer.cardPlayer[player.pTurn].number == ace) {
				dealer.score = dealer .score + 11;
				if (dealer.score > 21) {
					dealer.score = dealer.score - 10;
				}
			}
			else {
				dealer.score = dealer.score + dealer.cardPlayer[dealer.dTurn].number;	//그외는 1~9점 추가
			}
			dealer.dTurn++;
		}
		else if (dealer.score >= 17 || player.score > 21) {		//17점 이상이면 카드를 받지 않고 루프가 꺼짐
			menuPos = 14;			//플레이어 버스트가 아닌 결과창으로 간다.
			cursor.curPos = 16;
			bloop = 1;
			loop = 0;
		}
	}
}


/// <summary>
/// 플레이어가 게임의 진행을 하며 다음에 무엇을 해야할지 선택지를 주는 함수이다
/// 처음 리셋루프 (rLoop)에서는 첫 실행시 해야 할 항목에 대해 정리한다
/// cLoop에서는 플레이어의 카드에 대한 결과를 판독한다.
/// </summary>
void Play(int currentBet) {
	curBet = currentBet;
	while (rLoop) {	
		if (player.played == 0) {	//첫 실행 시 리셋
			Reset();			
			EntryBet();
			player.played++;		//플레이 한번 했음
		}
		else if(player.played > 0) {
			SoftReset();			//턴과 점수만 리셋
			EntryBet();
			player.played++;		//플레이 한번 했음
		}
		dealer.cardPlayer[dealer.dTurn] = Dealing(next++);			//딜러에게 카드 주기
		if (dealer.cardPlayer[dealer.dTurn].number > 10) {
			dealer.score = dealer.score + 10;			//플레이어와 같은 점수시스템
		}
		else {
			dealer.score = dealer.score + dealer.cardPlayer[dealer.dTurn].number;
		}
		dealer.dTurn++;			//딜러의 턴을 임시로 늘려줌(카드 엎어둠 표시?)
		rLoop = 0;				//그리고 리셋 루프를 꺼둠
	}
	while (cLoop) {
		player.cardPlayer[player.pTurn] = Dealing(next++);		//루프가 켜져있을 동안 카드를 줌(한번 돌떄 1씩 증가)
		if (player.cardPlayer[player.pTurn].number > 10) {		//점수 계산
			player.score = player.score + 10;
		}
		else if (player.cardPlayer[player.pTurn].number == ace) {
			player.score = player.score + 11;
			if (player.score > 21) {
				player.score = player.score - 10;
			}
		}
		else {
			player.score = player.score + player.cardPlayer[player.pTurn].number;
		}
		if (player.score > 21) {
			
			menuPos = 15;		//버스트 결과화면 보여주고
			cursor.curPos = 16;	//선택창으로 이동시킴	
			bloop = 1;
			cLoop = 0;		//루프는 꺼줌
		}
		//이 구간은 카드를 두장 주기 위해 작성되었음
		if (player.pTurn == 0) {
			cLoop = 1;
			player.pTurn++;
		}
		else {		//Hit,Double을 받을때 까지 0으로 유지 되어있음 (카드를 계속 주지 못하게)
			cLoop = 0;
		}
		if (player.pTurn == 1 && player.score == 21) {		//플레이어 블랙잭 우승 추가
			doubleCount++;
			menuPos = 15;		//블랙잭 승리 결과화면 보여주고
			cursor.curPos = 16;	//선택창으로 이동시킴		
			bloop = 1;
			cLoop = 0;		//루프는 꺼줌
		}
	}
	ListingCard();
}

/// <summary>
/// 게임이 끝난 후 계속하기 선택 시 게임 상단에 보여줄 함수
/// </summary>
void InGameMenu() {
	ShowBuffer(10, 3, "아래의 선택지 중 하나를 선택해 주세요. EXIT는 저장하지 않고 바로 메인 메뉴로 돌아갑니다.");
}

/// <summary>
/// 플레이어의 최고 점수를 현재 시간과 함께 저장하는 함수
/// </summary>
/// <param name="newScore">최고 점수를 받음</param>
void RecordRank(int newScore) {
	FILE* file;
	ScoreEntry scores[MAX_HIGHSCORE];

	file = fopen("HighScore.txt", "r");
	if (file == NULL) {
		return;
	}

	int numScores = 0;
	while (fscanf(file, "%19[^\n] %d%*c", scores[numScores].timeString, &scores[numScores].highScore) == 2) {
		numScores++;
	}

	fclose(file);

	ScoreEntry newEntry;
	time_t curTime;
	struct tm* timeInfo;
	time(&curTime);
	timeInfo = localtime(&curTime);
	strftime(newEntry.timeString, sizeof(newEntry.timeString), "%Y-%m-%d %H:%M:%S", timeInfo);
	newEntry.highScore = newScore;

	// 새로운 점수를 추가하고 순위 정렬
	int insertIndex = numScores;
	for (int i = 0; i < numScores; i++) {
		if (newEntry.highScore > scores[i].highScore) {
			insertIndex = i;
			break;
		}
	}

	if (insertIndex < MAX_HIGHSCORE) {
		for (int i = numScores - 1; i >= insertIndex; i--) {
			if (i + 1 < MAX_HIGHSCORE) {
				scores[i + 1] = scores[i];
			}
		}
		scores[insertIndex] = newEntry;
		numScores++;

		// 최대 기록 개수를 초과한 경우 마지막 기록 삭제
		if (numScores > MAX_HIGHSCORE) {
			numScores = MAX_HIGHSCORE;
		}

		// 파일에 기록 쓰기
		file = fopen("HighScore.txt", "w");
		if (file == NULL) {
			return;
		}
		for (int i = 0; i < numScores; i++) {
			fprintf(file, "%s     %d\n", scores[i].timeString, scores[i].highScore);
		}
		fclose(file);
	}
}
#pragma endregion

int main() {
#pragma region 더블 버퍼링
	Init();
	
	static BOOL bFlagEnter = FALSE;		//프레임마다 엔터를 눌리는 것을 방지

	int loop = 1;
	int cardDrawn = 0; //카드가 뽑혔는지 확인하는 플래그 변수(GPT)

	while (1) {
		//메뉴 프레임 설정
		GameFrame();

		//TestCursor();

		//키입력 설정
		MainKeyboard(&cursor);

		//프레임마다 엔터를 눌리는 것을 방지
		if (GetAsyncKeyState(VK_RETURN)) {		
			if (FALSE == bFlagEnter) {
				menuPos = cursor.curPos;
				if (menuPos == 0 || menuPos == 16) {
					cursor.curPos = 5;
					rLoop = 1;
					cLoop = 1;
				}
				else if (menuPos == 8 && player.money < 50) {
					menuPos = 0;
					cursor.curPos = 7;
				}
				else if (menuPos == 7 && player.money < 25) {
					menuPos = 0;
					cursor.curPos = 6;
				}
				else if (menuPos == 6 && player.money < 10) {
					menuPos = 0;
					cursor.curPos = 5;
				}
				else if (menuPos < 10 && menuPos > 4) {
					cursor.curPos = 11;
					//Play();
				}
				else if (menuPos == 16 && player.money <= 0) {
					cursor.curPos = 17;
				}
				bFlagEnter = TRUE;
			}
		}
		else {
			bFlagEnter = FALSE;
		}
		/*if (menuPos == 0 && GetAsyncKeyState(VK_ESCAPE)) {
			menuPos = 3;
		}*/
		//엔터 입력시 패널의 표시
		switch (menuPos) {
		case 0:				//메뉴 시작시 엔터 입력 후 판돈 표시
			GameMenu();
			SelectMoney();
			break;
		case 1:				//메뉴 시작시 엔터 입력 후 랭크 표시
			GameMenu();
			ShowRank();
			break;
		case 2:				//메뉴 시작시 엔터 입력 후 게임 종료
			exit(1);
			break;
		case 3:				//게임 실행 시 보여줄 로고
			GameMenu();
			GameLogoMenu();
			break;
		case 5:				//판돈 표시 후 5$ 베팅 입장 / 5~8까지
			Play(5);
			ChooseAct();
			break;
		case 6:		
			Play(6);
			ChooseAct();
			break;
		case 7:
			Play(7);
			ChooseAct();
			break;
		case 8:
			Play(8);
			ChooseAct();
			break;
		case 9:				//판돈 표시 후 다시 메뉴로 돌아가기
			GameLogoMenu();
			cursor.curPos = 0;
			menuPos = 3;
			break;
		case 11:			//게임 진행중 히트 입력시 할 행동
			if (player.score < 22) {
				Hit(1);
			}
			else {
				menuPos = 15;
			}
			break;
		case 12:
			if (player.score < 22) {
				if (player.money >= stakes / 2) {
					Double(1);
				}
				else{
					Hit(1);
				}
			}
			else {
				menuPos = 15;
			}
			break;
		case 13:			//게임 진행중 멈추고 싶을때 할 행동
			Stop();
			break;
		case 14:			//게임이 끝나고 결과창 보여주기
			ListingCard();
			ShowResult(1);
			SelectRestart();
			break;
		case 15:			//플레이어 버스트 시 결과창 보여주기
			ListingCard();
			BurstStop(1);
			SelectRestart();
			break;
		case 16:			//계속하기 (돈 유지 + 하이스코어 갱신)
			if (player.money > 0) {			//돈이 조금이라도 있다면
				InGameMenu();
				SelectMoney();	//계속하기 선택시 보여줄 판돈 (다시 5~8로 루프)
			}
			else {
				if (player.score > 21) {
					cursor.curPos = 17;
					menuPos = 15;
				}
				else {
					cursor.curPos = 17;
					menuPos = 14;
				}
			}
			break;
		case 17:			//기록 저장 후 메인 메뉴  (돈 삭제 + 하이스코어 등록)
			RecordRank(player.moneyHighScore);
			Reset();
			GameLogoMenu();
			cursor.curPos = 0;
			menuPos = 3;
			break;
		case 18:			//기록 저장하지 않고 메인 메뉴 (돈 삭제 + 하이스코어 삭제)
			Reset();
			GameLogoMenu();
			cursor.curPos = 0;
			menuPos = 3;
			break;

		}

		//커서의 표시
		switch (cursor.curPos) {
		case 0:
			ShowBuffer(7, 4, "▶");		//메뉴
			break;
		case 1:
			ShowBuffer(47, 4, "▶");
			break;
		case 2:
			ShowBuffer(87, 4, "▶");
			break;
		case 5:
			ShowBuffer(7, 24, "▶");	//베팅금 선택
			break;
		case 6:
			ShowBuffer(27, 24, "▶");
			break;
		case 7:
			ShowBuffer(47, 24, "▶");
			break;
		case 8:
			ShowBuffer(67, 24, "▶");
			break;
		case 9:
			ShowBuffer(87, 24, "▶");	//베팅선택 나가기
			break;
		case 11:
			ShowBuffer(7, 4, "▶");		//베팅
			break;
		case 12:
			ShowBuffer(47, 4, "▶");
			break;
		case 13:
			ShowBuffer(87, 4, "▶");
			break;
		case 16:
			ShowBuffer(7, 4, "▶");		//계속하기
			break;
		case 17:
			ShowBuffer(47, 4, "▶");	//저장후 그만
			break;
		case 18:
			ShowBuffer(87, 4, "▶");	//저장하지 않고 그만
			break;
		default:
			break;
		}

		Flipping();

		Clear();
	}
	ReleaseScreen();
#pragma endregion

	return 0;
}
