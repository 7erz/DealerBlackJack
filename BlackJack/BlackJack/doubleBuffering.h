#pragma once
#include <stdio.h>
#include <conio.h>
#include <Windows.h>


//HANDLE �ε����� �����ؼ� ���۸� ��ü��Ű�� ����
int screenIndex = 0;

//������ ũ��
int width = 100;
int height = 60;

//���� ����
HANDLE Screen[2];

//[0] : Front Buffer
//[1] : Back Buffer

//���۸� �ʱ�ȭ�ϴ� �Լ�
void Init() {
	system("mode con cols=112 lines=41");
	CONSOLE_CURSOR_INFO cursor;

	//������ ���� ������, ���� ������
	COORD size = { width,height };

	//LEFT, TOP, RIGHT, BOTTOM
	SMALL_RECT rect = { 0,0,width - 1,height - 1 };

	//ȭ�� �ΰ� ����
	//Front Buffer
	Screen[0] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	SetConsoleScreenBufferSize(Screen[0], size);

	SetConsoleWindowInfo(Screen[0], TRUE, &rect);

	//Back Buffer
	Screen[1] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	SetConsoleScreenBufferSize(Screen[1], size);

	SetConsoleWindowInfo(Screen[1], TRUE, &rect);

	//Ŀ���� Ȱ��ȭ ����
	cursor.bVisible = false;

	GetConsoleCursorInfo(Screen[0], &cursor);
	GetConsoleCursorInfo(Screen[1], &cursor);
}

//���۸� ��ü�ϴ� �Լ�
void Flipping() {
	//���۴� �ϳ��� Ȱ��ȭ��ų �� ����
	SetConsoleActiveScreenBuffer(Screen[screenIndex]);

	//true <- false, false <- true ���� �ݺ�

	screenIndex = !screenIndex;
}

//��ü�� ���۸� �����ִ� �Լ�
void Clear() {
	COORD coord = { 0,0 };

	DWORD dw;

	FillConsoleOutputCharacter(
		Screen[screenIndex],
		' ',
		width * height,
		coord,
		&dw
	);
}

//���۸� �����ϴ� �Լ�
void ReleaseScreen() {
	CloseHandle(Screen[0]);
	CloseHandle(Screen[1]);
}

//���۸� �̿��ؼ� ����ϴ� �Լ�
void ShowBuffer(int x, int y, const char* string) {
	COORD cursorPos = { x,y };

	DWORD dw;

	SetConsoleCursorPosition(Screen[screenIndex], cursorPos);

	WriteFile(
		Screen[screenIndex],
		string,
		strlen(string),
		&dw,
		NULL
	);
}