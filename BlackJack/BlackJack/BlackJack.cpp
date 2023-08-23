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
#pragma region 싱글 버퍼링
	/*//system("mode con cols=112 lines=20");
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
	printf("게임 종료");*/
#pragma endregion

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


	ShowBuffer(10, 4, "게임 시작");
	ShowBuffer(50, 4, "랭킹 확인");
	ShowBuffer(90, 4, "게임 종료");

#pragma endregion
	return;
}
//게임 첫 실행 시 보이는 로고
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


//게임 시작 후 베팅 금액 보이는 패널
void SelectMoney() {
	ShowBuffer(10, 24, "$5");
	ShowBuffer(30, 24, "$10");
	ShowBuffer(50, 24, "$25");
	ShowBuffer(70, 24, "$50");
	ShowBuffer(90, 24, "EXIT");
}

void ShowRank() {
	ShowBuffer(50, 24, "◎...개발중...◎");
}

int main() {
#pragma region 싱글 버퍼링
	/*Cursor cursor = {7,4,"▶",0};

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
			//default: break;
		}
		Sleep(100);
		//cls;
	}*/
#pragma endregion

#pragma region 더블 버퍼링
	Init();
	Cursor cursor = { 7,4,"▶",0 };
	int menuPos = 3;	//0 - 게임시작, 1 - 랭크 확인 2 - 게임 종료 3 - 메인메뉴 로고표시

	while (true) {
		MainMenu();
		MainKeyboard(&cursor);
		if (GetAsyncKeyState(VK_RETURN)) {
			menuPos = cursor.curPos;
		}
		if (menuPos == 0 && GetAsyncKeyState(VK_ESCAPE)) {
			menuPos = 3;
		}
		switch (cursor.curPos) {
		case 0:
			ShowBuffer(7, 4, "▶");
			/*if (GetAsyncKeyState(VK_ESCAPE)) {
				menuPos = 0;
			}*/
			break;
		case 1:
			ShowBuffer(47, 4, "▶");
			/*if (GetAsyncKeyState(VK_ESCAPE)) {
				menuPos = 1;
			}*/
			break;
		case 2:
			ShowBuffer(87, 4, "▶");
			/*if (GetAsyncKeyState(VK_ESCAPE)) {
				menuPos = 2;
			}*/
			break;
		default:
			break;
		}
		switch (menuPos) {
		case 0:
			SelectMoney();
			break;
		case 1:
			ShowRank();
			break;
		case 2:
			exit(1);
			break;
		case 3:
			GameLogoMenu();
		}

		Flipping();

		Clear();
	}
	ReleaseScreen();

#pragma endregion

	


	return 0;
}






/*C:\Users\leesn\OneDrive\바탕 화면\SBS\DealerBlackJack\BlackJack\x64\Debug\BlackJack.exe(프로세스 15084개)이(가) 종료되었습니다(코드: 0개).
디버깅이 중지될 때 콘솔을 자동으로 닫으려면 [도구] -> [옵션] -> [디버깅] > [디버깅이 중지되면 자동으로 콘솔 닫기]를 사용하도록 설정합니다.
이 창을 닫으려면 아무 키나 누르세요...*/