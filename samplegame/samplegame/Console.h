#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define CONSOLE_SCREEN_MAX 2

typedef enum {
    false = 0,
	true = (!false)
} BOOL;

typedef struct TAG_CONSOLE {
	HANDLE hStdOut;
	int hScreenIndex;
	HANDLE hScreen[CONSOLE_SCREEN_MAX];
	HANDLE hWork;
	HANDLE hOut;
} CONSOLE;

extern CONSOLE g_console;
extern void Console_Init();
extern void Console_Flip();
extern void Console_Close();
extern int  Print(const char * str, int x, int y);
extern void Console_CLS();
extern void Console_HOME();
extern void Console_COLOR(unsigned short color);
extern void Console_LOCATE(int x, int y);
extern void Console_DispCursor(BOOL fDisp);
extern void Console_PutText(int x1, int y1, int x2, int y2, CHAR_INFO *data);

#endif // __CONSOLE_H__