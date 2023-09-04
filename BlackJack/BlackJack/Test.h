#pragma region 게임 실행 테스트
void GameMenuTest() {
	ShowBuffer(10, 4, "HIT");
	ShowBuffer(50, 4, "DOUBLE");
	ShowBuffer(90, 4, "STAND");


}
void Test() {
	ShowBuffer(10, 50, "Test");
}

void TestShowCard() {
	ShowBuffer(11, 30, "--------");
	ShowBuffer(11, 31, "-S Here-");
	ShowBuffer(11, 32, "-Here N-");
	ShowBuffer(11, 33, "-Here S-");
	ShowBuffer(11, 34, "--------");
}

void TestPlay(int turn) {
	int printed = 0;
	if (turn == printed) {
		player.cardPlayer[turn] = Dealing(next++);


		//ShowCard(turn);

		printed++;
	}
}

#pragma endregion#pragma once


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
