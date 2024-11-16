
#include "main.h"
#include "Ghost.h"

int main() {
	int character_x = 1, character_y = 1, character_color = YELLOW; // 캐릭터 좌표, 색깔 초기화
	Ghost ghost1(1, 23, direction(2), LIGHTCYAN);                   // 유령 1 초기화
	Ghost ghost2(23, 1, direction(0), LIGHTRED);                    // 유령 2 초기화
	initPacMan();                                                   // 팩맨 게임 초기화
	
	while (!game_over && !game_win) {
		printUI();                                          // UI 출력
		printMap(character_x, character_y, ghost1, ghost2); // 맵 출력

		// 캐릭터 출력 처리, 이중 출력을 통해 깜빡임을 줄임
		if (SPECIAL_FLAG) { SPECIAL_FLAG--; character_color = LIGHTMAGENTA; }
		else character_color = YELLOW;
		printEntity(character_x, character_y, "◎", character_color);
		
		// 유령 사망 여부에 따른 출력 및 이동 처리
		if (!ghost1.getDead()) ghost1.print();
		if (!ghost2.getDead()) ghost2.print();

		// 캐릭터 아이템 획득 및 유령 충돌 처리
		getFood(character_x, character_y);
		meetGhost(character_x, character_y, ghost1), meetGhost(character_x, character_y, ghost2);
		
		// 플레이어 입력 및 입력에 따른 이동 처리
		if (_kbhit()) {
			key_stroke = _getch();
			moveEntity(character_x, character_y, key_stroke, "◎", character_color);
		}

		// 유령의 타이머에 따른 이동 처리
		if (ghost1.updateTime() < 0) ghost1.update(character_x, character_y);
		if (ghost2.updateTime() < 0) ghost2.update(character_x, character_y);

		isGameOver(); // 게임 종료 여부 확인
		Sleep(30);    // 시간 진행
	}

	if (game_win) win();        // 승리
	else if (game_over) lose(); // 패배
}

/* =============================== [유틸리티 함수] ============================== */

// 화면의 깔끔한 출력을 위한 커서 숨기기 함수
void cursorHide() {
	CONSOLE_CURSOR_INFO cursor_info = { 0, };
	cursor_info.dwSize = 1;
	cursor_info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

// 게임의 출력을 위한 좌표 이동 함수
void gotoXY(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos; pos.X = x; pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

// 입력받은 색 상수에 따라 글자 색을 변환하는 함수
void textColor(int foreground, int background) {
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/* =============================== [게임 플레이 함수] ============================== */

// dy[], dx[] 방향 idx 값을 방향 상수로 변환
int direction(int dir) {
	if (dir == 0) return UP;
	else if (dir == 1) return RIGHT;
	else if (dir == 2) return DOWN;
	else if (dir == 3) return LEFT;
	else return 0;
}

// 플레이어의 아이템 획득 처리
void getFood(int x, int y) {

	// 입력받은 좌표에 음식이 있다면 음식을 먹고 점수 가산
	if (Map[y][x] == FOOD) {
		Map[y][x] = BLANK;
		score += 10;
	}

	// 입력받은 좌표에 스페셜 아이템이 있다면 스페셜 아이템을 먹고 점수 가산
	if (Map[y][x] == SPECIAL) {
		Map[y][x] = BLANK;
		SPECIAL_FLAG = 100; // 스페셜 아이템의 타이머 가산
		score += 1000;
	}
}

// 캐릭터 좌표, 유령 좌표를 입력받아 유령과 플레이어의 충돌 처리
void meetGhost(int cx, int cy, Ghost& ghost) {
	// 유령의 좌표와 플레이어의 좌표가 같다면
	if (cx == ghost.getX() && cy == ghost.getY()) {
		if (!SPECIAL_FLAG) game_over = 1; // 스페셜 아이템이 없을 시 게임 오버
		else ghost.die();                 // 스페셜 아이템이 있을 시 유령을 쓰러뜨림
	}
}

// 좌표와 이동 방향 상수를 입력받아 이동 여부를 출력
bool isMovable(int x, int y, int dir) {
	// 입력받은 이동 방향에 따라 좌표 이동
	switch (dir) {
	case UP: 
		y = max(1, y - 1); break;
	case LEFT:
		x = max(1, x - 1); break;
	case RIGHT:
		x = min(23, x + 1); break;
	case DOWN:
		y = min(23, y + 1); break;
	}

	// 이동 방향이 벽이라면 이동 불가능, 벽이 아니라면 이동 가능
	if (Map[y][x] != WALL) return true;
	else return false;
}

// 좌표와 문자열, 문자 색을 입력받아 개체 출력
void printEntity(const int x, const int y, const char *string, int color) {
	gotoXY(2 * x + MAP_POS_X, y + MAP_POS_Y); // 좌표로 이동
	textColor(color, BLACK);                  // 입력받은 색으로 문자열 변경
	cout << string;                           // 문자열 출력
}

// 좌표와 방향 상수를 입력받아 이동 여부를 판단한 뒤 이동 처리
void moveEntity(int& x, int& y, int dir, const char *string, int color) {
	if (isMovable(x, y, dir)) {
		printEntity(x, y, "  ", 0); // 기존 좌표에 출력된 값은 삭제

		// 적절히 좌표 이동
		switch (dir) {
		case UP:
			y = max(1, y - 1); break;
		case LEFT:
			x = max(1, x - 1); break;
		case RIGHT:
			x = min(23, x + 1); break;
		case DOWN:
			y = min(23, y + 1); break;
		}

		printEntity(x, y, string, color); // 이동한 좌표에서 새로 개체 출력
	}
}

/* =============================== [화면, UI, 초기화 함수] ============================== */

// 미로 생성을 위한 recursive-dfs 알고리즘
void dfs(int x, int y) {
	vector<int> dir = { 0, 1, 2, 3 }; // 방향 idx 1차원 벡터 생성
	shuffle(dir.begin(), dir.end(), gen); // 난수 변수를 바탕으로 shuffle해 무작위화
	Map[y][x] = BLANK, vst[y][x] = 1; // 시작 좌표를 비우고, 방문 처리함

	// DFS 처리
	for (auto i : dir) { // 무작위 방향 idx를 dir 벡터에서 꺼낸 뒤
		int nx = x + 2 * dx[i], ny = y + 2 * dy[i]; // 해당 방향으로 2칸 이동
		if (nx <= 0 || ny <= 0 || nx >= 24 || ny >= 24) continue; // 경계값 처리 

		// 방문하지 않은 노드라면 중간의 벽을 부수고 dfs(nx, ny) 재귀적 실행
		if (!vst[ny][nx]) {
			Map[y + dy[i]][x + dx[i]] = BLANK;
			dfs(nx, ny);
		}
	}
}

// 게임 초기화 및 미로 생성 함수
void initPacMan() {
  cursorHide(); // 커서 깜빡임 삭제

  // 지도에 벽 생성
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 25; x++) {
			Map[y][x] = WALL;
		}
	}

	// recursive-dfs 방식을 통해 미로 랜덤 생성
	dfs(1, 1);

	// 게임의 편의성을 위해 미로를 불완전 미로로 변형
	for (int y = 1; y < 24; y++) {
		for (int x = 1; x < 24; x++) {
			// 맵의 테두리를 전부 비움
			if (x == 1 || y == 1 || x == 23 || y == 23)
				Map[y][x] = BLANK;

			// x % 2 == 1, y % 2 == 1의 맵에 있는 벽을 25% 확률로 파괴
			if (x % 2 || y % 2) {
				if (dis(gen) == 1) Map[y][x] = BLANK; 
			}
		}
	}

	// 먹어야 하는 아이템 생성
	for (int y = 1; y < 24; y++) {
		for (int x = 1; x < 24; x++) {
			if (!Map[y][x]) Map[y][x] = FOOD;
		}
	}

	// 스페셜 푸드 생성
	Map[23][23] = SPECIAL;
}

// UI 출력 함수
void printUI() {
	// 점수 UI 출력
	for (int y = 20; y <= 24; y++) {
		for (int x = 27; x <= 37; x++) {
			if (x > 27 && x < 37 && y > 20 && y < 24) continue;
			printEntity(x, y, "▨", 15);
		}
	}

	// 점수 출력
	gotoXY(58 + MAP_POS_X, 22 + MAP_POS_Y);
	cout << "SCORE: " << setw(7) << score;

	// 라운드 UI 출력
	for (int y = 0; y <= 4; y++) {
		for (int x = 27; x <= 37; x++) {
			if (x > 27 && x < 37 && y > 0 && y < 4) continue;
			printEntity(x, y, "▨", 15);
		}
	}

	// 라운드 출력
	gotoXY(58 + MAP_POS_X, 2 + MAP_POS_Y);
	cout << "ROUND: " << setw(7) << playing_round;

	// 스페셜 아이템 획득 시
	if (SPECIAL_FLAG > 0) {
		gotoXY(54 + MAP_POS_X, 6 + MAP_POS_Y);
		textColor(YELLOW, BLACK);       // 글자색 변경
		cout << "GOT SPECIAL ITEM !!!"; // 스페셜 아이템 획득 메시지 출력
		textColor(WHITE, BLACK);						// 글자색 초기화
	}
}

// 맵 출력
void printMap(int cx, int cy, Ghost ghost1, Ghost ghost2) {
	textColor(WHITE, BLACK);
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 25; x++) {
			
			// 게임의 깜빡임을 줄이기 위해 캐릭터의 좌표, 유령의 좌표일 경우 skip 
			if (y == cy && x == cx) continue;
			if (y == ghost1.getY() && x == ghost1.getX()) continue;
			if (y == ghost2.getY() && x == ghost2.getX()) continue;

			if (Map[y][x] == WALL) printEntity(x, y, "▦", WHITE);           // 벽 출력
			if (Map[y][x] == FOOD) printEntity(x, y, "·", YELLOW);           // 음식 출력
			if (Map[y][x] == SPECIAL) printEntity(x, y, "★", LIGHTMAGENTA); // 스페셜 아이템 출력
		}
	}

	// 프로젝트 메시지 출력
	gotoXY(MAP_POS_X, 26 + MAP_POS_Y); cout << "20191150 전현길: PACMAN PROJECT";
}

void isGameOver() {
	// 게임 종료 여부 확인
	int flag = 0; // flag reset
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 25; x++) {
			if (Map[y][x] == FOOD) flag = 1; // 맵에 음식이 있을 시 flag set
		}
	}

	if (!flag) game_win = 1; // 맵에 음식이 없다면 게임 승리 판정
}

// 승리에 따른 메시지 출력
void win() {
	textColor(YELLOW, BLACK);

	gotoXY(8, 9);  cout << "▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦";
	gotoXY(8, 10); cout << "▦　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　▦";
	gotoXY(8, 11); cout << "▦　　▨▨　　　▨▨　▨▨▨▨▨　▨▨　　▨▨　▨　▨　▨　　▦";
	gotoXY(8, 12); cout << "▦　　▨▨　　　▨▨　　▨▨　　　▨▨▨　▨▨　▨　▨　▨　　▦";
	gotoXY(8, 13); cout << "▦　　▨▨　　　▨▨　　▨▨　　　▨▨▨▨▨▨　▨　▨　▨　　▦";
	gotoXY(8, 14); cout << "▦　　▨▨　▨　▨▨　　▨▨　　　▨▨　▨▨▨　　　　　　　　▦";
	gotoXY(8, 15); cout << "▦　　　▨▨　▨▨　　▨▨▨▨▨　▨▨　　▨▨　▨　▨　▨　　▦";
	gotoXY(8, 16); cout << "▦　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　▦";
	gotoXY(8, 17); cout << "▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦";
	gotoXY(-1, -1);

	Sleep(5000); // 메시지 출력 시간
}

// 패배에 따른 메시지 출력
void lose() {
	textColor(LIGHTRED, BLACK);

	gotoXY(6, 9);  cout << "▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦";
	gotoXY(6, 10); cout << "▦　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　▦";
	gotoXY(6, 11); cout << "▦　　▨▨　　　　　▨▨▨　　▨▨▨▨▨　▨▨▨▨▨　　　　　　　　▦";
	gotoXY(6, 12); cout << "▦　　▨▨　　　　▨▨　▨▨　▨▨　　　　▨▨　　　　　　　　　　　▦";
	gotoXY(6, 13); cout << "▦　　▨▨　　　　▨▨　▨▨　▨▨▨▨▨　▨▨▨▨▨　　　　　　　　▦";
	gotoXY(6, 14); cout << "▦　　▨▨　　　　▨▨　▨▨　　　　▨▨　▨▨　　　　　　　　　　　▦";
	gotoXY(6, 15); cout << "▦　　▨▨▨▨▨　　▨▨▨　　▨▨▨▨▨　▨▨▨▨▨　▨　▨　▨　　▦";
	gotoXY(6, 16); cout << "▦　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　▦";
	gotoXY(6, 17); cout << "▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦";
	gotoXY(76, 25);

  Sleep(5000); // 메시지 출력 시간
}
