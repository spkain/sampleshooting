#include "Console.h"

CONSOLE g_console;

void Console_Init()
{
	int i;
	for (i = 0; i < CONSOLE_SCREEN_MAX; i++) {
		g_console.hScreen[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	}
	g_console.hScreenIndex = 0;
	g_console.hOut = g_console.hWork = g_console.hScreen[g_console.hScreenIndex];
	SetConsoleActiveScreenBuffer(g_console.hOut);
	g_console.hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
}

void Console_Close()
{
	int i;
	for (i = 0; i < CONSOLE_SCREEN_MAX; i++) {
		CloseHandle(g_console.hScreen[i]);
		g_console.hScreen[i] = NULL;
	}

	g_console.hOut = g_console.hWork = NULL;
	SetConsoleActiveScreenBuffer(g_console.hStdOut);
	CloseHandle(g_console.hStdOut);
	g_console.hStdOut = NULL;
}

void Console_Flip()
{
	g_console.hOut = g_console.hWork; // 仕事を終えたハンドルを表示用ハンドルに
	g_console.hScreenIndex = (g_console.hScreenIndex + 1) % CONSOLE_SCREEN_MAX;
	g_console.hWork = g_console.hScreen[g_console.hScreenIndex]; // 次の仕事用バッファ

	SetConsoleActiveScreenBuffer(g_console.hOut); // 表示用のバッファをアクティブ化
}

int Print(const char * str, int x, int y)
{
	COORD coPos = {(SHORT)x, (SHORT)y};
	unsigned int cell = 0;
	SetConsoleCursorPosition(g_console.hWork, coPos);
	WriteConsole(g_console.hWork, str, strlen(str), &cell, NULL);

	return (int)cell;
}

void Console_COLOR(unsigned short color)
{
	WORD wAttributes;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	g_console.hWork = GetStdHandle(STD_OUTPUT_HANDLE);
	
	GetConsoleScreenBufferInfo(g_console.hWork, &csbi);
	wAttributes = color;

	if (color & 0x08) wAttributes |= FOREGROUND_INTENSITY;

	SetConsoleTextAttribute(g_console.hWork, wAttributes);
}

void Console_LOCATE(int x, int y)
{
	COORD co;
	CONSOLE_SCREEN_BUFFER_INFO cinf;

	g_console.hWork = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(g_console.hWork, &cinf);
	co.X = (unsigned short)x;
	co.Y = (unsigned short)y
#ifndef LOCATE_ABSORUTE
		+ cinf.srWindow.Top
#endif
		;
	g_console.hWork = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(g_console.hWork, co);
}

void Console_CLS()
{
	COORD coset;
	DWORD dwWritten;
	WORD wAttribute;

#ifdef LOCATE_ABSORUTE
	SMALL_RECT WindowCorner;
#endif
	CONSOLE_SCREEN_BUFFER_INFO cinf;
	g_console.hWork = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(g_console.hWork, &cinf);

	coset.X = 0;
	coset.Y = 0;
	wAttribute = 0x000f;

	if (FillConsoleOutputCharacter(g_console.hWork, ' ', cinf.dwSize.X * cinf.dwSize.Y, coset, &dwWritten)) {
		SetConsoleTextAttribute(g_console.hWork, wAttribute);
		FillConsoleOutputAttribute(g_console.hWork, wAttribute, cinf.dwSize.X * cinf.dwSize.Y, coset, &dwWritten);

#ifdef LOCATE_ABSORUTE
		WindowCorner.Top = 0;
		WindowCorner.Left = 0;
		WindowCorner.Right = cinf.srWindow.Right;
		WindowCorner.Bottom = cinf.srWindow.Bottom - cinf.srWindow.Top;
		SetConsoleWindowInfo(g_console.hWork, TRUE, &WindowCorner);
#endif

		Console_LOCATE(0,0);
	}
}

void Console_HOME()
{

#ifdef LOCATE_ABSORUTE
	SMALL_RECT WindowCorner;
#endif

	CONSOLE_SCREEN_BUFFER_INFO cinf;
	g_console.hWork = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(g_console.hWork, &cinf);
#ifdef LOCATE_ABSORUTE
	WindowCorner.Top = 0;
	WindowCorner.Left = 0;
	WindowCorner.Right = cinf.srWindow.Right;
	WindowCorner.Bottom = cinf.srWindow.Bottom - cinf.srWindow.Top;
	SetConsoleWindowInfo(g_console.hWork, TRUE, &WindowCorner);
#endif
	Console_LOCATE(0, 0);
}

void Console_DispCursor(BOOL fDisp)
{
	CONSOLE_SCREEN_BUFFER_INFO cinf;
	CONSOLE_CURSOR_INFO curinf;
	COORD cod = { MAX_WIDTH, MAX_HEIGHT };
	SMALL_RECT sr = { 0, 0, (MAX_WIDTH-1), (MAX_HEIGHT-1) };

	g_console.hWork = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(g_console.hWork, &cinf);

	if (GetConsoleCursorInfo(g_console.hWork, &curinf)) {
		curinf.bVisible = fDisp;
		SetConsoleCursorInfo(g_console.hWork, &curinf);
	}

	SetConsoleScreenBufferSize(g_console.hWork, cod);
	SetConsoleWindowInfo(g_console.hWork, TRUE, &sr);
}

void Console_PutText(int x1, int y1, int x2, int y2, CHAR_INFO *data)
{
	CONSOLE_SCREEN_BUFFER_INFO cinf;
	COORD dwBufferSize;
	COORD dwBufferCoord;
	SMALL_RECT WriteRegion;

	g_console.hWork = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(g_console.hWork, &cinf);

#ifndef LOCATE_ABSORUTE
	y1 += cinf.srWindow.Top;
	y2 += cinf.srWindow.Top;
#endif
	g_console.hWork = GetStdHandle(STD_OUTPUT_HANDLE);

	dwBufferSize.X = x2 - x1 + 1;
	dwBufferSize.Y = y2 - y1 + 1;
	dwBufferCoord.X = 0;
	dwBufferCoord.Y = 0;

	WriteRegion.Top = y1;
	WriteRegion.Left = x1;
	WriteRegion.Bottom = y2;
	WriteRegion.Right = x2;

	WriteConsoleOutput(
		g_console.hWork,
		data,
		dwBufferSize,
		dwBufferCoord,
		&WriteRegion
	);
}
