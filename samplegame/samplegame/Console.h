#include <stdio.h>
#include <conio.h>
#include <windows.h>

typedef enum {
    false = 0,
	true = (!false)
} BOOL;

extern void Console_CLS();
extern void Console_HOME();
extern void Console_COLOR(unsigned short color);
extern void Console_LOCATE(int x, int y);
extern void Console_DispCursor(BOOL fDisp);
extern void Console_PutText(int x1, int y1, int x2, int y2, CHAR_INFO *data);
