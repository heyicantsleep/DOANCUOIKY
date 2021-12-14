﻿#pragma comment(lib, "winmm.lib")
#include "cgame.h"
#include <conio.h>
#include <thread>

CGAME cg;
char MOVING;
int speed = 50;
bool IS_RUNNING = true;

void SubThread() {
	while (IS_RUNNING) {
		if (!cg.getPeople().isDead()) {
			cg.updatePosPeople(MOVING);
			MOVING = ' ';
			cg.printScore();
			cg.updatePosVehicle();
			cg.updatePosAnimal();
			cg.drawGame();
			cg.isImpact();
			cg.reset();
			cg.setSpeed(speed);
			if (cg.getPeople().isDead()) {
				cg.drawDie();
				cg.drawContinue();
			}
		}
	}
}



auto l = [](thread& t1) {
	while (true) {
		char temp = toupper(_getch());
		if (!cg.getPeople().isDead()) {
			if (temp == 27) {
				IS_RUNNING = false;
				t1.join();
				cg.resetGame();
				break;
			}
			else if (temp == 'P') {
				cg.pauseGame(t1.native_handle());
			}
			else if (temp == 'L') {
				cg.pauseGame(t1.native_handle());
				cg.saveGame();
				system("cls");
				cg.drawMap();
				cg.resumeGame(t1.native_handle());
			}
			else {
				cg.resumeGame(t1.native_handle());
				MOVING = temp; // Cập nhật bước di chuyển
			}
		}
		else {
			if (temp == 'Y') {
				system("cls");
				cg.drawMap();
				cg.resetGame();
			}
			else {
				IS_RUNNING = false;
				t1.join();
				cg.resetGame();
				break;
			}

		}
	}
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	fixConsole();
	resizeConsole(1120, 630);
	hideCursor();
	PlaySound(TEXT("song1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	const string menu[] = { " New Game  ", " Load Game ", " Settings  ",
						   " Exit      " };
	const string songMenu[] = { " Mute             ", " Tornado Of Souls ", " You Say Run      ", " Back             " };
	int pos = 0;
	int pos1 = 0;
	int key2 = 0;
	const int y = MAP_HEIGHT / 2;
	const int x = MAP_WIDTH / 1.5 + 5;
	while (1) {
		system("cls");
		textColor(14);
		gotoxy(1, 2);
		cout << R"(
            $$$$$$\                                          $$\                            $$$$$$\                                    
           $$  __$$\                                         \__|                          $$  __$$\                                   
           $$ /  \__| $$$$$$\   $$$$$$\   $$$$$$$\  $$$$$$$\ $$\ $$$$$$$\   $$$$$$\        $$ /  \__| $$$$$$\  $$$$$$\$$$$\   $$$$$$\  
           $$ |      $$  __$$\ $$  __$$\ $$  _____|$$  _____|$$ |$$  __$$\ $$  __$$\       $$ |$$$$\  \____$$\ $$  _$$  _$$\ $$  __$$\ 
           $$ |      $$ |  \__|$$ /  $$ |\$$$$$$\  \$$$$$$\  $$ |$$ |  $$ |$$ /  $$ |      $$ |\_$$ | $$$$$$$ |$$ / $$ / $$ |$$$$$$$$ |
           $$ |  $$\ $$ |      $$ |  $$ | \____$$\  \____$$\ $$ |$$ |  $$ |$$ |  $$ |      $$ |  $$ |$$  __$$ |$$ | $$ | $$ |$$   ____|
           \$$$$$$  |$$ |      \$$$$$$  |$$$$$$$  |$$$$$$$  |$$ |$$ |  $$ |\$$$$$$$ |      \$$$$$$  |\$$$$$$$ |$$ | $$ | $$ |\$$$$$$$\ 
            \______/ \__|       \______/ \_______/ \_______/ \__|\__|  \__| \____$$ |       \______/  \_______|\__| \__| \__| \_______|
                                                                           $$\   $$ |                                                  
                                                                           \$$$$$$  |                                                  
                                                                            \______/                                                   )";

		for (int i = 0; i < 4; ++i) {
			if (i == pos) {
				textColor(224);
				gotoxy(x, y + i);
				cout << menu[i];
				textColor(7);
			}
			else {
				gotoxy(x, y + i);
				textColor(14);
				cout << menu[i];
			}
		}
		while (1) {
			if (_kbhit()) {
				char key = _getch();
				if (key == 'W' || key == 'w') {
					if (pos > 0) {
						pos--;
					}
					else {
						pos = 4 - 1;
					}
					break;
				}
				if (key == 'S' || key == 's') {
					if (pos < 4 - 1) {
						pos++;
					}
					else {
						pos = 0;
					}
					break;
				}
				if (key == 13) {
					switch (pos) {
					case 0: {
						system("cls");
						cg.drawMap();
						IS_RUNNING = true;
						thread t1(SubThread);
						l(t1);
						break;
					}
					case 1: {
						cg.loadGame();
						system("cls");
						cg.drawMap();
						IS_RUNNING = true;
						thread t1(SubThread);
						l(t1);
						break;
					}
					case 2: {
						while (1) {

							for (int k = 0; k < 4; ++k) {
								if (k == pos1) {
									textColor(224);
									gotoxy(x, y + k);
									cout << songMenu[k];
									textColor(7);
								}
								else {
									gotoxy(x, y + k);
									textColor(14);
									cout << songMenu[k];
								}
							}
							while (1) {
								if (_kbhit()) {
									char key1 = _getch();
									if (key1 == 'W' || key1 == 'w') {
										if (pos1 > 0) {
											pos1--;
										}
										else {
											pos1 = 4 - 1;
										}
										break;
									}
									else if (key1 == 'S' || key1 == 's') {
										if (pos1 < 4 - 1) {
											pos1++;
										}
										else {
											pos1 = 0;
										}
										break;
									}
									if (key1 == 13) {
										switch (pos1) {
										case 0:
											PlaySound(NULL, 0, 0);
											break;
										case 1:
											PlaySound(TEXT("song1.wav"), NULL,
												SND_FILENAME | SND_ASYNC | SND_LOOP);
											break;
										case 2:
											PlaySound(TEXT("song2.wav"), NULL,
												SND_FILENAME | SND_ASYNC | SND_LOOP);
											break;
										case 3:
											key2 = 1;
											break;
										}
									}
									break;
								}
							}
							if (key2 == 1) {
								key2 = 0;
								break;
							}
						}
						system("cls");
						break;
					}
						  break;
					case 3: {
						system("cls");
						exit(0);
						break;
					}
					}
					break;
				}
			}
		}
	}
}