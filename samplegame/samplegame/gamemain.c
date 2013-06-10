#include "gamemain.h"

int bullet_flg[BULLET_MAX] = { 0 };
int mon_bullet_flg[MON_BULLET_MAX] = { 0 };
struct MONSTER mon[MON_MAX];
struct BULLET bul[BULLET_MAX];
struct BULLET mon_bul[MON_BULLET_MAX];

int bullet_num = 0;
int mon_bullet_num = 0;

int chara_x = 30;
int chara_y = 20;

int mon_default_x = 30;
int mon_default_y = 0;

int mon_attack_cnt = 0;

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
	printf("= plz any keys to start game. =\n");

	_getch();
	Console_HOME();
}

void gameloop() {
	while(1) {
		draw();
		monstermove();
        monstermake();
		bulletmove();
        attackcheck();
		keycheck();
		Sleep(16);

		if(endcheck() == 1) {
			printf("clear!\n");
			printf("game end...");
  			_getch();
			exit(0);
		}
	}
}

void draw() {
	int i = 0;
	Console_CLS();

	for (i = 0; i < MON_MAX; i++) {
		if (mon[i].flag == 1) {
			Console_LOCATE(mon[i].x, mon[i].y);
			printf(MONSTER_STR);
		}
	}


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

void attackcheck() {
	int i, j;
	for (i = 0; i < BULLET_MAX; i++) {
		if (bul[i].flag == 1) {
			for (j = 0; j < MON_MAX; j++) {
				if (bul[i].x == mon[j].x && bul[i].y == mon[j].y) {
					bul[i].flag = 0;
					mon[j].flag = 0;
					mon_attack_cnt++;
				}
			}
		}
	}
}

void monstermove() {
    int i;
	for (i = 0; i < MON_MAX; i++) {
		if (mon[i].flag == 1) {
			mon[i].x += mon[i].dx;

			if (mon[i].x < 0) {
				mon[i].dx = 1;
			} else if (mon[i].x > 60) {
				mon[i].dx = -1;
			}
		}
	}
}

void monstermake() {
    int i;
	for (i = 0; i < MON_MAX; i++) {
		if (mon[i].flag == 0) {
			mon[i].x = mon_default_x;
			mon[i].y = mon_default_y;
			mon[i].dx = -1;
			mon[i].flag = 1;
		}
	}
}

int endcheck() {
		if (mon_attack_cnt == 3) {
			return 1;
		}
		return 0;
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