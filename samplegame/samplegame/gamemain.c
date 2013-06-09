#include "gamemain.h"

int bullet_flg[BULLET_MAX] = { 0 };
struct BULLET bul[BULLET_MAX];
int bullet_num = 0;

int chara_x = 30;
int chara_y = 20;

int main()
{
	gameinit();
	printf("start\n");
	gameloop();
}

void gameinit() {
	Console_DispCursor(false);

	Console_LOCATE(30, 15);
	printf("■シューティング\n");
	Console_LOCATE(25, 16);
	printf("= start to any keys. =\n");

	_getch();
	Console_HOME();
}

void gameloop() {
	while(1) {
		draw();
		bulletmove();
		keycheck();
		Sleep(16);
	}
}

void draw() {
	int i = 0;
	Console_CLS();

	Console_LOCATE(chara_x, chara_y);
	printf(PLAYER);

	for (i = 0; i < BULLET_MAX; i++) {
		if (bul[i].flag == 1) {
			Console_LOCATE(bul[i].x, bul[i].y);
			printf(BULLET_STR);
		}
	}
}

void bulletshoot() {

	if (bul[bullet_num].flag == 0) {
		bul[bullet_num].flag = 1;
		bul[bullet_num].x = chara_x;
		bul[bullet_num].y = chara_y - 1;
	}

	bullet_num++;

	if (bullet_num == BULLET_MAX) {
		bullet_num = 0;
	}
}

void bulletmove() {
	int i;
	for (i = 0; i < BULLET_MAX; i++) {
		if (bul[i].flag == 1) {
			bul[i].y--;

			if (bul[i].y < 0) {
				bul[i].flag = 0;
			}
		}
	}
}
void keycheck() {
	int input_key = 0;
	if (_kbhit())input_key = _getch();
	
	switch (input_key) {
	case VK_Q:
		Console_HOME();
		printf(" end game...\n");
		printf(" plz any key to exit\n");
		_getch();
		exit(0);
	case VK_UP:
		chara_y--;
		break;
	case VK_DOWN:
		chara_y++;
		break;
	case VK_LEFT:
		chara_x--;
		break;
	case VK_RIGHT:
		chara_x++;
		break;
	case VK_SPACE:
		bulletshoot();
	}
}