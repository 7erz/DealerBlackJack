#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <stralign.h>

#include "defaultMain.h"


#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define ENTER 13

#define cls system("cls")



struct Cursor {
	int x, y;
	const char* shape;
	int curPos = 0;		//메인 커서의 위치
};

void MainKeyboard(Cursor* cursor) {
	char key = 0;

	if (_kbhit()) {
		key = _getch();

		if (key == -32) {
			key = _getch();
		}
		switch (key) {
		case RIGHT: if (cursor->curPos == 2) cursor->curPos = 0;
				  else cursor->curPos += 1;
			break;
		case LEFT: if (cursor->curPos == 0) cursor->curPos = 2;
				 else cursor->curPos -= 1;
			break;
		}
	}
}

void MainMenu() {
	system("title 딜러 블랙 잭 : 메인 화면");
	system("mode con cols=112 lines=20");
	printf("●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎\n");
	printf("◎                                                                                                            ●\n");
	printf("●                                                                                                            ◎\n");
	printf("◎                                                                                                            ●\n");
	printf("●                                                                                                            ◎\n");
	printf("◎                                                                                                            ●\n");
	printf("●                                                                                                            ◎\n");
	printf("◎                                                                                                            ●\n");
	printf("●                                                                                                            ◎\n");
	printf("◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●◎●\n");

	gotoXY(10, 4);
	printf("게임 시작");

	gotoXY(50, 4);
	printf("랭킹 확인");

	gotoXY(90, 4);
	printf("게임 종료");

	return;
}

int main() {
	
	Cursor cursor = { 7,4,"▶",0};

	while (1) {
		MainMenu();
		MainKeyboard(&cursor);
		switch (cursor.curPos) {
		case 0:
			gotoXY(7, 4);
			printf("%s", cursor.shape);
			break;
		case 1:
			gotoXY(47, 4);
			printf("%s", cursor.shape);
			break;
		case 2:
			gotoXY(87, 4);
			printf("%s", cursor.shape);
			break;
		default: break;
		}
		Sleep(100);
		//cls;
	}


	return 0;
}






/*C:\Users\leesn\OneDrive\바탕 화면\SBS\DealerBlackJack\BlackJack\x64\Debug\BlackJack.exe(프로세스 15084개)이(가) 종료되었습니다(코드: 0개).
디버깅이 중지될 때 콘솔을 자동으로 닫으려면 [도구] -> [옵션] -> [디버깅] > [디버깅이 중지되면 자동으로 콘솔 닫기]를 사용하도록 설정합니다.
이 창을 닫으려면 아무 키나 누르세요...*/