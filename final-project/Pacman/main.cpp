
#include "main.h"
#include "Ghost.h"

int main() {
	int character_x = 1, character_y = 1, character_color = YELLOW; // ĳ���� ��ǥ, ���� �ʱ�ȭ
	Ghost ghost1(1, 23, direction(2), LIGHTCYAN);                   // ���� 1 �ʱ�ȭ
	Ghost ghost2(23, 1, direction(0), LIGHTRED);                    // ���� 2 �ʱ�ȭ
	initPacMan();                                                   // �Ѹ� ���� �ʱ�ȭ
	
	while (!game_over && !game_win) {
		printUI();                                          // UI ���
		printMap(character_x, character_y, ghost1, ghost2); // �� ���

		// ĳ���� ��� ó��, ���� ����� ���� �������� ����
		if (SPECIAL_FLAG) { SPECIAL_FLAG--; character_color = LIGHTMAGENTA; }
		else character_color = YELLOW;
		printEntity(character_x, character_y, "��", character_color);
		
		// ���� ��� ���ο� ���� ��� �� �̵� ó��
		if (!ghost1.getDead()) ghost1.print();
		if (!ghost2.getDead()) ghost2.print();

		// ĳ���� ������ ȹ�� �� ���� �浹 ó��
		getFood(character_x, character_y);
		meetGhost(character_x, character_y, ghost1), meetGhost(character_x, character_y, ghost2);
		
		// �÷��̾� �Է� �� �Է¿� ���� �̵� ó��
		if (_kbhit()) {
			key_stroke = _getch();
			moveEntity(character_x, character_y, key_stroke, "��", character_color);
		}

		// ������ Ÿ�̸ӿ� ���� �̵� ó��
		if (ghost1.updateTime() < 0) ghost1.update(character_x, character_y);
		if (ghost2.updateTime() < 0) ghost2.update(character_x, character_y);

		isGameOver(); // ���� ���� ���� Ȯ��
		Sleep(30);    // �ð� ����
	}

	if (game_win) win();        // �¸�
	else if (game_over) lose(); // �й�
}

/* =============================== [��ƿ��Ƽ �Լ�] ============================== */

// ȭ���� ����� ����� ���� Ŀ�� ����� �Լ�
void cursorHide() {
	CONSOLE_CURSOR_INFO cursor_info = { 0, };
	cursor_info.dwSize = 1;
	cursor_info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

// ������ ����� ���� ��ǥ �̵� �Լ�
void gotoXY(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos; pos.X = x; pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

// �Է¹��� �� ����� ���� ���� ���� ��ȯ�ϴ� �Լ�
void textColor(int foreground, int background) {
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/* =============================== [���� �÷��� �Լ�] ============================== */

// dy[], dx[] ���� idx ���� ���� ����� ��ȯ
int direction(int dir) {
	if (dir == 0) return UP;
	else if (dir == 1) return RIGHT;
	else if (dir == 2) return DOWN;
	else if (dir == 3) return LEFT;
	else return 0;
}

// �÷��̾��� ������ ȹ�� ó��
void getFood(int x, int y) {

	// �Է¹��� ��ǥ�� ������ �ִٸ� ������ �԰� ���� ����
	if (Map[y][x] == FOOD) {
		Map[y][x] = BLANK;
		score += 10;
	}

	// �Է¹��� ��ǥ�� ����� �������� �ִٸ� ����� �������� �԰� ���� ����
	if (Map[y][x] == SPECIAL) {
		Map[y][x] = BLANK;
		SPECIAL_FLAG = 100; // ����� �������� Ÿ�̸� ����
		score += 1000;
	}
}

// ĳ���� ��ǥ, ���� ��ǥ�� �Է¹޾� ���ɰ� �÷��̾��� �浹 ó��
void meetGhost(int cx, int cy, Ghost& ghost) {
	// ������ ��ǥ�� �÷��̾��� ��ǥ�� ���ٸ�
	if (cx == ghost.getX() && cy == ghost.getY()) {
		if (!SPECIAL_FLAG) game_over = 1; // ����� �������� ���� �� ���� ����
		else ghost.die();                 // ����� �������� ���� �� ������ �����߸�
	}
}

// ��ǥ�� �̵� ���� ����� �Է¹޾� �̵� ���θ� ���
bool isMovable(int x, int y, int dir) {
	// �Է¹��� �̵� ���⿡ ���� ��ǥ �̵�
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

	// �̵� ������ ���̶�� �̵� �Ұ���, ���� �ƴ϶�� �̵� ����
	if (Map[y][x] != WALL) return true;
	else return false;
}

// ��ǥ�� ���ڿ�, ���� ���� �Է¹޾� ��ü ���
void printEntity(const int x, const int y, const char *string, int color) {
	gotoXY(2 * x + MAP_POS_X, y + MAP_POS_Y); // ��ǥ�� �̵�
	textColor(color, BLACK);                  // �Է¹��� ������ ���ڿ� ����
	cout << string;                           // ���ڿ� ���
}

// ��ǥ�� ���� ����� �Է¹޾� �̵� ���θ� �Ǵ��� �� �̵� ó��
void moveEntity(int& x, int& y, int dir, const char *string, int color) {
	if (isMovable(x, y, dir)) {
		printEntity(x, y, "  ", 0); // ���� ��ǥ�� ��µ� ���� ����

		// ������ ��ǥ �̵�
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

		printEntity(x, y, string, color); // �̵��� ��ǥ���� ���� ��ü ���
	}
}

/* =============================== [ȭ��, UI, �ʱ�ȭ �Լ�] ============================== */

// �̷� ������ ���� recursive-dfs �˰���
void dfs(int x, int y) {
	vector<int> dir = { 0, 1, 2, 3 }; // ���� idx 1���� ���� ����
	shuffle(dir.begin(), dir.end(), gen); // ���� ������ �������� shuffle�� ������ȭ
	Map[y][x] = BLANK, vst[y][x] = 1; // ���� ��ǥ�� ����, �湮 ó����

	// DFS ó��
	for (auto i : dir) { // ������ ���� idx�� dir ���Ϳ��� ���� ��
		int nx = x + 2 * dx[i], ny = y + 2 * dy[i]; // �ش� �������� 2ĭ �̵�
		if (nx <= 0 || ny <= 0 || nx >= 24 || ny >= 24) continue; // ��谪 ó�� 

		// �湮���� ���� ����� �߰��� ���� �μ��� dfs(nx, ny) ����� ����
		if (!vst[ny][nx]) {
			Map[y + dy[i]][x + dx[i]] = BLANK;
			dfs(nx, ny);
		}
	}
}

// ���� �ʱ�ȭ �� �̷� ���� �Լ�
void initPacMan() {
  cursorHide(); // Ŀ�� ������ ����

  // ������ �� ����
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 25; x++) {
			Map[y][x] = WALL;
		}
	}

	// recursive-dfs ����� ���� �̷� ���� ����
	dfs(1, 1);

	// ������ ���Ǽ��� ���� �̷θ� �ҿ��� �̷η� ����
	for (int y = 1; y < 24; y++) {
		for (int x = 1; x < 24; x++) {
			// ���� �׵θ��� ���� ���
			if (x == 1 || y == 1 || x == 23 || y == 23)
				Map[y][x] = BLANK;

			// x % 2 == 1, y % 2 == 1�� �ʿ� �ִ� ���� 25% Ȯ���� �ı�
			if (x % 2 || y % 2) {
				if (dis(gen) == 1) Map[y][x] = BLANK; 
			}
		}
	}

	// �Ծ�� �ϴ� ������ ����
	for (int y = 1; y < 24; y++) {
		for (int x = 1; x < 24; x++) {
			if (!Map[y][x]) Map[y][x] = FOOD;
		}
	}

	// ����� Ǫ�� ����
	Map[23][23] = SPECIAL;
}

// UI ��� �Լ�
void printUI() {
	// ���� UI ���
	for (int y = 20; y <= 24; y++) {
		for (int x = 27; x <= 37; x++) {
			if (x > 27 && x < 37 && y > 20 && y < 24) continue;
			printEntity(x, y, "��", 15);
		}
	}

	// ���� ���
	gotoXY(58 + MAP_POS_X, 22 + MAP_POS_Y);
	cout << "SCORE: " << setw(7) << score;

	// ���� UI ���
	for (int y = 0; y <= 4; y++) {
		for (int x = 27; x <= 37; x++) {
			if (x > 27 && x < 37 && y > 0 && y < 4) continue;
			printEntity(x, y, "��", 15);
		}
	}

	// ���� ���
	gotoXY(58 + MAP_POS_X, 2 + MAP_POS_Y);
	cout << "ROUND: " << setw(7) << playing_round;

	// ����� ������ ȹ�� ��
	if (SPECIAL_FLAG > 0) {
		gotoXY(54 + MAP_POS_X, 6 + MAP_POS_Y);
		textColor(YELLOW, BLACK);       // ���ڻ� ����
		cout << "GOT SPECIAL ITEM !!!"; // ����� ������ ȹ�� �޽��� ���
		textColor(WHITE, BLACK);						// ���ڻ� �ʱ�ȭ
	}
}

// �� ���
void printMap(int cx, int cy, Ghost ghost1, Ghost ghost2) {
	textColor(WHITE, BLACK);
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 25; x++) {
			
			// ������ �������� ���̱� ���� ĳ������ ��ǥ, ������ ��ǥ�� ��� skip 
			if (y == cy && x == cx) continue;
			if (y == ghost1.getY() && x == ghost1.getX()) continue;
			if (y == ghost2.getY() && x == ghost2.getX()) continue;

			if (Map[y][x] == WALL) printEntity(x, y, "��", WHITE);           // �� ���
			if (Map[y][x] == FOOD) printEntity(x, y, "��", YELLOW);           // ���� ���
			if (Map[y][x] == SPECIAL) printEntity(x, y, "��", LIGHTMAGENTA); // ����� ������ ���
		}
	}

	// ������Ʈ �޽��� ���
	gotoXY(MAP_POS_X, 26 + MAP_POS_Y); cout << "20191150 ������: PACMAN PROJECT";
}

void isGameOver() {
	// ���� ���� ���� Ȯ��
	int flag = 0; // flag reset
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 25; x++) {
			if (Map[y][x] == FOOD) flag = 1; // �ʿ� ������ ���� �� flag set
		}
	}

	if (!flag) game_win = 1; // �ʿ� ������ ���ٸ� ���� �¸� ����
}

// �¸��� ���� �޽��� ���
void win() {
	textColor(YELLOW, BLACK);

	gotoXY(8, 9);  cout << "�ˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢ�";
	gotoXY(8, 10); cout << "�ˡ�������������������������������������������������������������";
	gotoXY(8, 11); cout << "�ˡ����ɢɡ������ɢɡ��ɢɢɢɢɡ��ɢɡ����ɢɡ��ɡ��ɡ��ɡ�����";
	gotoXY(8, 12); cout << "�ˡ����ɢɡ������ɢɡ����ɢɡ������ɢɢɡ��ɢɡ��ɡ��ɡ��ɡ�����";
	gotoXY(8, 13); cout << "�ˡ����ɢɡ������ɢɡ����ɢɡ������ɢɢɢɢɢɡ��ɡ��ɡ��ɡ�����";
	gotoXY(8, 14); cout << "�ˡ����ɢɡ��ɡ��ɢɡ����ɢɡ������ɢɡ��ɢɢɡ�����������������";
	gotoXY(8, 15); cout << "�ˡ������ɢɡ��ɢɡ����ɢɢɢɢɡ��ɢɡ����ɢɡ��ɡ��ɡ��ɡ�����";
	gotoXY(8, 16); cout << "�ˡ�������������������������������������������������������������";
	gotoXY(8, 17); cout << "�ˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢ�";
	gotoXY(-1, -1);

	Sleep(5000); // �޽��� ��� �ð�
}

// �й迡 ���� �޽��� ���
void lose() {
	textColor(LIGHTRED, BLACK);

	gotoXY(6, 9);  cout << "�ˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢ�";
	gotoXY(6, 10); cout << "�ˡ�������������������������������������������������������������������";
	gotoXY(6, 11); cout << "�ˡ����ɢɡ����������ɢɢɡ����ɢɢɢɢɡ��ɢɢɢɢɡ�����������������";
	gotoXY(6, 12); cout << "�ˡ����ɢɡ��������ɢɡ��ɢɡ��ɢɡ��������ɢɡ�����������������������";
	gotoXY(6, 13); cout << "�ˡ����ɢɡ��������ɢɡ��ɢɡ��ɢɢɢɢɡ��ɢɢɢɢɡ�����������������";
	gotoXY(6, 14); cout << "�ˡ����ɢɡ��������ɢɡ��ɢɡ��������ɢɡ��ɢɡ�����������������������";
	gotoXY(6, 15); cout << "�ˡ����ɢɢɢɢɡ����ɢɢɡ����ɢɢɢɢɡ��ɢɢɢɢɡ��ɡ��ɡ��ɡ�����";
	gotoXY(6, 16); cout << "�ˡ�������������������������������������������������������������������";
	gotoXY(6, 17); cout << "�ˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢˢ�";
	gotoXY(76, 25);

  Sleep(5000); // �޽��� ��� �ð�
}
