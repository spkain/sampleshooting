#include "gamemain.h"

#define DEBUG

int bullet_flg[BULLET_MAX] = { 0 };
int mon_bullet_flg[MON_BULLET_MAX] = { 0 };
struct MONSTER mon[MON_MAX];
struct BULLET bul[BULLET_MAX];
struct BULLET mon_bul[MON_BULLET_MAX];
clock_t t, start, end;
int count = 0;

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
	Print("start\n", 0, 0);
	gameloop();
}

void gameinit() {
	Console_Init();
	Console_DispCursor(false);

	Print("■シューティング\n", 30, 15);
	Print("= plz any keys to start game. =\n", 25, 16);
	Console_Flip();

	_getch();
	Console_HOME();
	Console_Flip();
	start = clock();
}

void gameloop() {

	while(1) {
		fps();
		count++;
		monstermove();
        monstermake();
		bulletmove();
        attackcheck();
		keycheck();
		draw();
		Console_Flip();
		gamewait();

		if(endcheck() == 1) {
			Print("clear!\n", 10, 5);
			Print("game end...", 10, 6);
  			_getch();
			Console_Close();
			exit(0);
		}
	}
}

void draw() {
	int i = 0;
	Console_CLS();

	for (i = 0; i < MON_MAX; i++) {
		if (mon[i].flag == 1) {
			Print(MONSTER_STR, mon[i].x, mon[i].y);
		}
	}


	Print(PLAYER, chara_x, chara_y);

	for (i = 0; i < BULLET_MAX; i++) {
		if (bul[i].flag == 1) {
			Print(BULLET_STR, bul[i].x, bul[i].y);
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

void fps() {
	char buff[64] = {0};
	int i;
	static int t = 0, average = 0, fpsc[60];
	fpsc[count%60]= GetNowCount() - t; 	
	t = GetNowCount();

	if ( count % 60 == 59 ) {
		average = 0;
		for (i = 0; i < 60; i++) {
			average += fpsc[i];
		}
		average /= 60;
	}
#ifdef DEBUG
	if ( average != 0 ) {
		Console_HOME();
		Console_COLOR(FOREGROUND_BLUE|BACKGROUND_INTENSITY);
		sprintf(buff, "%.1fFPS", 1000.0/(double)average);
		Print(buff, 0, 0);
		Console_COLOR(WHITE|BG_BLACK);
	}
#endif
	return;
}

void gamewait() {

	int term = 0;

	term = GetNowCount();
	(1000 / 60 > term)? Sleep(1000 / 60 - term) : Sleep(0);

	start = clock();
}

double fpscount() {
	return 0.0;
}

int GetNowCount() {
	end = clock();
	return (end - start);
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
		Print(" end game...\n", 0, 0);
		Print(" plz any key to exit\n", 0, 1);
		Console_Flip();
		_getch();
		Console_Close();
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