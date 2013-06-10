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
#define MONSTER_STR "Çƒ" 
#define MON_BULLET_STR "ÅE"

// item
#define BULLET_MAX 20
#define MON_MAX 1
#define MON_BULLET_MAX 1

// bullet struct
struct BULLET {
	int x;
	int y;
	int flag;
};

struct MONSTER {
	int x;
	int y;
	int dx;
	int dy;
	int flag;
};

void attackcheck();
void monstermake();
void monstermove();
void bulletshoot();
void bulletmove();
void gameinit();
void gameloop();
int endcheck();
void keycheck();
void draw();