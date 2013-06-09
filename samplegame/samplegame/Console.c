#include "Console.h"

#define _WIN32_WINNT 0x0501
#define LOCATE_ABSORUTE

void Console_COLOR(unsigned short color)
{
	HANDLE hStdout;
	WORD wAttributes;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(hStdout, &csbi);

	wAttributes = color;

	if (color & 0x08) wAttributes |= FOREGROUND_INTENSITY;

	SetConsoleTextAttribute(hStdout, wAttributes);
}

void Console_LOCATE(int x, int y)
{
	HANDLE hOut;
	COORD co;
	CONSOLE_SCREEN_BUFFER_INFO cinf;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hOut, &cinf);

	co.X = (unsigned short)x;
	co.Y = (unsigned short)y
#ifndef LOCATE_ABSORUTE
		+ cinf.srWindow.Top
#endif
		;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, co);
}

void Console_CLS()
{
	COORD coset;
	HANDLE hOut;
	DWORD dwWritten;
	WORD wAttribute;

#ifdef LOCATE_ABSORUTE
	SMALL_RECT WindowCorner;
#endif

	CONSOLE_SCREEN_BUFFER_INFO cinf;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hOut, &cinf);

	coset.X = 0;
	coset.Y = 0;
	wAttribute = 0x000f;

	if (FillConsoleOutputCharacter(hOut, ' ', cinf.dwSize.X * cinf.dwSize.Y, coset, &dwWritten)) {
		SetConsoleTextAttribute(hOut, wAttribute);
		FillConsoleOutputAttribute(hOut, wAttribute, cinf.dwSize.X * cinf.dwSize.Y, coset, &dwWritten);

#ifdef LOCATE_ABSORUTE
		WindowCorner.Top = 0;
		WindowCorner.Left = 0;
		WindowCorner.Right = cinf.srWindow.Right;
		WindowCorner.Bottom = cinf.srWindow.Bottom - cinf.srWindow.Top;
		SetConsoleWindowInfo(hOut, TRUE, &WindowCorner);
#endif
		Console_LOCATE(0,0);
	}
}

void Console_HOME()
{
	HANDLE hOut;

#ifdef LOCATE_ABSORUTE
	SMALL_RECT WindowCorner;
#endif

	CONSOLE_SCREEN_BUFFER_INFO cinf;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hOut, &cinf);

#ifdef LOCATE_ABSORUTE
	WindowCorner.Top = 0;
	WindowCorner.Left = 0;
	WindowCorner.Right = cinf.srWindow.Right;
	WindowCorner.Bottom = cinf.srWindow.Bottom - cinf.srWindow.Top;
	SetConsoleWindowInfo(hOut, TRUE, &WindowCorner);
#endif
	Console_LOCATE(0, 0);
}

void Console_DispCursor(BOOL fDisp)
{
	HANDLE hOut;
	CONSOLE_SCREEN_BUFFER_INFO cinf;
	CONSOLE_CURSOR_INFO curinf;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hOut, &cinf);

	if (GetConsoleCursorInfo(hOut, &curinf)) {
		curinf.bVisible = fDisp;
		SetConsoleCursorInfo(hOut, &curinf);
	}
}

void Console_PutText(int x1, int y1, int x2, int y2, CHAR_INFO *data)
{
	HANDLE hOut;
	CONSOLE_SCREEN_BUFFER_INFO cinf;
	COORD dwBufferSize;
	COORD dwBufferCoord;
	SMALL_RECT WriteRegion;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hOut, &cinf);

#ifndef LOCATE_ABSORUTE
	y1 += cinf.srWindow.Top;
	y2 += cinf.srWindow.Top;
#endif
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	dwBufferSize.X = x2 - x1 + 1;
	dwBufferSize.Y = y2 - y1 + 1;
	dwBufferCoord.X = 0;
	dwBufferCoord.Y = 0;

	WriteRegion.Top = y1;
	WriteRegion.Left = x1;
	WriteRegion.Bottom = y2;
	WriteRegion.Right = x2;

	WriteConsoleOutput(
		hOut,
		data,
		dwBufferSize,
		dwBufferCoord,
		&WriteRegion
	);
}