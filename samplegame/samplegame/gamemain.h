#include "Console.h"

// key codes
#define VK_SPACE 32
#define VK_UP 72
#define VK_DOWN 80
#define VK_LEFT 75
#define VK_RIGHT 77
#define VK_Q 113

// user graphics
#define PLAYER "Å°" 
#define BULLET_STR "Åb"

// item
#define BULLET_MAX 20

// bullet struct
struct BULLET {
	int x;
	int y;
	int flag;
};

void bulletshoot();
void bulletmove();
void gameinit();
void gameloop();
void keycheck();
void draw();