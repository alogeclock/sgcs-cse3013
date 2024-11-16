#include <conio.h>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <windows.h>
using namespace std;

#define MAP_POS_X 2
#define MAP_POS_Y 1

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

#define BLANK 0
#define WALL 1
#define FOOD 2

#define LIGHTCYAN 11
#define LIGHTRED 12
#define YELLOW 14
#define WHITE 15

void gotoXY(int x, int y);
void textColor(int foreground, int background);
void cursorHide();

void meetGhost(int cx, int cy, int gx, int gy);
void getFood(int x, int y);
bool checkMovable(int x, int y, int dir);
void printEntity(int x, int y, const char *string, int color);
void moveEntity(int &x, int &y, int dir, const char *string, int color);
void meetGhost(int cx, int cy, int gx, int gy);
int direction(int dir);

void dfs(int x, int y);
void initPacMan();
void printUI();
void printMap();
void checkGameOver();
void win();
void lose();

const int dy[] = {-1, 0, 1, 0}, dx[] = {0, 1, 0, -1}; // UP, RIGHT, DOWN, LEFT
int Map[25][25], vst[25][25], dst[25][25];
int keyStroke, score, playing_round;

int character_x = 1, character_y = 1;
int gameOver, gameWin;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 3);

struct Vertex {
  int x, y, dir;
  Vertex(int pos_x, int pos_y, int pos_dir) : x(pos_x), y(pos_y), dir(pos_dir) {}
};

class Path {
private:
  int x, y, dir;
  vector<Vertex> path;

public:
  Path(int pos_x, int pos_y, int pos_dir, vector<Vertex> prev_path) {
    this->x = pos_x, this->y = pos_y, this->dir = pos_dir;
    this->path = prev_path;
    this->path.push_back(Vertex(x, y, dir));
  }
  void copy(int &pos_x, int &pos_y, vector<Vertex> &cur_path) const {
    pos_x = x;
    pos_y = y;
    cur_path = path;
  }
};

class Ghost {
private:
  int x, y, dir, time, color;
  int found_character;

public:
  Ghost(int x, int y, int dir, int color) : x(x), y(y), dir(dir), color(color) {
    time = 3, found_character = 0;
  }
  int getX() const { return x; }
  int getY() const { return y; }
  int getTime() {
    time--;
    return time;
  };
  void update() {
    time = 3;

    // 주변에 캐릭터가 있는지 BFS 탐색, 캐릭터의 위치에 따라 dir 갱신
    this->found_character = 0;
    int vst[25][25];
    memset(vst, 0x3f, sizeof(vst));
    queue<Path> q;
    vector<Vertex> initial_path;

    q.push(Path(x, y, 0, initial_path));
    vst[y][x] = 1;

    while (q.size()) {
      int cx, cy, cdst;
      vector<Vertex> path;
      q.front().copy(cx, cy, path);
      cdst = vst[cy][cx];
      q.pop();

      if (cx == x && cy == y && path.size() > 1) {
        this->found_character = 1;
        dir = path[1].dir; // 방향 캐릭터 위치에 따라 전환
        break;
      }

      if (path.size() > 20) break;

      for (int i = 0; i < 4; i++) { // UP, RIGHT, DOWN, LEFT
        int ny = cy + dy[i], nx = cx + dx[i], ndst = cdst + 1;
        if (nx <= 0 || ny <= 0 || nx >= 24 || ny >= 24) continue;
        if (Map[ny][nx] != BLANK || vst[ny][nx] < ndst) continue;
        vst[ny][nx] = ndst;
        q.push(Path(nx, ny, i, path));
      }
    }

    move();
  }
  void move() {
    // 캐릭터가 있다면 캐릭터 쪽으로 이동, 아니라면 방향 전환
    if (checkMovable(x, y, dir)) {
      moveEntity(x, y, direction(dir), "●", color);
      return;
    }

    for (int i = 0; i < 4; i++) {
      if (dir == i) continue;
      if (checkMovable(x, y, direction(i))) {
        dir = i;
        break;
      }
    }

    moveEntity(x, y, direction(dir), "●", color);
  }
};

int main() {
  Ghost ghost1(1, 23, 0, LIGHTCYAN);
  Ghost ghost2(23, 1, 0, LIGHTRED);
  playing_round = 1;
  cursorHide();
  initPacMan();

  while (!gameOver && !gameWin) {
    printUI();
    printMap();

    // 캐릭터, 유령 출력 및 이동
    printEntity(character_x, character_y, "◎", YELLOW);
    printEntity(ghost1.getX(), ghost1.getY(), "●", LIGHTCYAN);
    printEntity(ghost2.getX(), ghost2.getY(), "●", LIGHTRED);

    getFood(character_x, character_y);
    meetGhost(character_x, character_y, ghost1.getX(), ghost1.getY());
    meetGhost(character_x, character_y, ghost2.getX(), ghost2.getY());

    if (_kbhit()) {
      keyStroke = _getch();
      moveEntity(character_x, character_y, keyStroke, "◎", YELLOW);
    }

    if (!ghost1.getTime()) ghost1.update();
    if (!ghost2.getTime()) ghost2.update();

    checkGameOver();

    Sleep(30);
  }

  if (gameWin)
    win();
  else if (gameOver)
    lose();
}

/* =============================== [유틸리티 함수] ============================== */

void cursorHide() {
  CONSOLE_CURSOR_INFO cursor_info = {
      0,
  };
  cursor_info.dwSize = 1;
  cursor_info.bVisible = false;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoXY(int x, int y) {
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos;
  pos.X = x;
  pos.Y = y;
  SetConsoleCursorPosition(handle, pos);
}

void textColor(int foreground, int background) {
  // BLUE = 1, MAGENTA = 5, YELLOW = 14, WHITE = 15
  int color = foreground + background * 16;
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/* =============================== [게임 플레이 함수] ============================== */

int direction(int dir) {
  if (dir == 0) return UP;
  if (dir == 1) return RIGHT;
  if (dir == 2) return DOWN;
  if (dir == 3) return LEFT;
  return 0;
}

void getFood(int x, int y) {
  if (Map[y][x] == FOOD) {
    Map[y][x] = BLANK;
    score += 10;
  }
}

void meetGhost(int cx, int cy, int gx, int gy) {
  if (cx == gx && cy == gy) gameOver = 1;
}

bool checkMovable(int x, int y, int dir) {
  switch (dir) {
  case UP:
    y = max(1, y - 1);
    break;
  case LEFT:
    x = max(1, x - 1);
    break;
  case RIGHT:
    x = min(23, x + 1);
    break;
  case DOWN:
    y = min(23, y + 1);
    break;
  }

  if (Map[y][x] != WALL)
    return true;
  else
    return false;
}

void printEntity(const int x, const int y, const char *string, int color) {
  gotoXY(2 * x + MAP_POS_X, y + MAP_POS_Y);
  textColor(color, 0);
  cout << string;
}

void moveEntity(int &x, int &y, int dir, const char *string, int color) {
  if (checkMovable(x, y, dir)) {
    printEntity(x, y, "  ", 0);
    switch (dir) {
    case UP:
      y = max(1, y - 1);
      break;
    case LEFT:
      x = max(1, x - 1);
      break;
    case RIGHT:
      x = min(23, x + 1);
      break;
    case DOWN:
      y = min(23, y + 1);
      break;
    }
    printEntity(x, y, string, color);
  }
}

/* =============================== [게임 초기화] ============================== */

void dfs(int x, int y) {
  // 난수 생성(생성하지 않을 경우 시드값이 고정)
  vector<int> dir = {0, 1, 2, 3};
  shuffle(dir.begin(), dir.end(), gen);
  Map[y][x] = BLANK, vst[y][x] = 1;

  for (auto i : dir) {
    int nx = x + 2 * dx[i], ny = y + 2 * dy[i];
    if (nx <= 0 || ny <= 0 || nx >= 24 || ny >= 24) continue;
    if (!vst[ny][nx]) {
      Map[y + dy[i]][x + dx[i]] = BLANK;
      dfs(nx, ny);
    }
  }
}

void initPacMan() {
  // 지도에 벽 생성
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 25; x++) {
      Map[y][x] = WALL;
    }
  }

  // 재귀적 백트래킹 방식을 통해 미로 랜덤 생성
  dfs(1, 1);

  // 게임의 편의성을 위해 미로를 불완전 미로로 변형
  for (int y = 1; y < 24; y++) {
    for (int x = 1; x < 24; x++) {
      if (x == 1 || y == 1 || x == 23 || y == 23)
        Map[y][x] = BLANK;
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
}

void printUI() {
  // 점수 출력
  for (int y = 20; y <= 24; y++) {
    for (int x = 27; x <= 37; x++) {
      if (x > 27 && x < 37 && y > 20 && y < 24) continue;
      printEntity(x, y, "▨", 15);
    }
  }
  gotoXY(58 + MAP_POS_X, 22 + MAP_POS_Y);
  cout << "SCORE: " << setw(7) << score;

  // 라운드 출력
  for (int y = 0; y <= 4; y++) {
    for (int x = 27; x <= 37; x++) {
      if (x > 27 && x < 37 && y > 0 && y < 4) continue;
      printEntity(x, y, "▨", 15);
    }
  }

  gotoXY(58 + MAP_POS_X, 2 + MAP_POS_Y);
  cout << "ROUND: " << setw(7) << playing_round;
}

void printMap() {
  textColor(15, 0);
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 25; x++) {
      if (Map[y][x] == WALL) printEntity(x, y, "▦", WHITE);
      if (Map[y][x] == FOOD) printEntity(x, y, "·", YELLOW);
    }
  }

  gotoXY(MAP_POS_X, 26 + MAP_POS_Y);
  cout << "20191150 전현길: PACMAN PROJECT";
}

/* =============================== [승리, 패배 조건] ============================== */

void checkGameOver() {
  int flag = 0;
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 25; x++) {
      if (Map[y][x] == FOOD) flag = 1;
    }
  }

  if (!flag) gameWin = 1;
}

void win() {
  system("cls");
  textColor(YELLOW, 0);

  gotoXY(5, 8);
  cout << "▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦";
  gotoXY(5, 9);
  cout << "▦▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▦";
  gotoXY(5, 10);
  cout << "▦▨　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　▨▦";
  gotoXY(5, 11);
  cout << "▦▨　　　　▨▨　　　▨▨　▨▨▨▨▨　▨▨　　　▨▨　　　▨▨　　▨▨　　　　▨▦";
  gotoXY(5, 12);
  cout << "▦▨　　　　▨▨　　　▨▨　▨▨▨▨▨　▨▨▨　　▨▨　　　▨▨　　▨▨　　　　▨▦";
  gotoXY(5, 13);
  cout << "▦▨　　　　▨▨　　　▨▨　　▨▨▨　　▨▨▨▨　▨▨　　　▨▨　　▨▨　　　　▨▦";
  gotoXY(5, 14);
  cout << "▦▨　　　　▨▨　　　▨▨　　▨▨▨　　▨▨▨▨▨▨▨　　　▨▨　　▨▨　　　　▨▦";
  gotoXY(5, 15);
  cout << "▦▨　　　　▨▨　▨　▨▨　　▨▨▨　　▨▨　▨▨▨▨　　　　　　　　　　　　　▨▦";
  gotoXY(5, 16);
  cout << "▦▨　　　　▨▨▨▨▨▨▨　▨▨▨▨▨　▨▨　　▨▨▨　　　▨▨　　▨▨　　　　▨▦";
  gotoXY(5, 17);
  cout << "▦▨　　　　　▨▨　▨▨　　▨▨▨▨▨　▨▨　　　▨▨　　　▨▨　　▨▨　　　　▨▦";
  gotoXY(5, 18);
  cout << "▦▨　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　▨▦";
  gotoXY(5, 19);
  cout << "▦▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▦";
  gotoXY(5, 20);
  cout << "▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦";
  gotoXY(-1, -1);

  for (double cur = 0; cur < 5.0; cur += 0.1) {
    Sleep(100);
  }
}

void lose() {
  system("cls");
  textColor(LIGHTRED, 0);

  gotoXY(5, 8);
  cout << "▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦";
  gotoXY(5, 9);
  cout << "▦▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▦";
  gotoXY(5, 10);
  cout << "▦▨　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　▨▦";
  gotoXY(5, 11);
  cout << "▦▨　　　▨▨　　　　　▨▨▨　　▨▨▨▨▨　▨▨▨▨▨　　　　　　　　　　　　　▨▦";
  gotoXY(5, 12);
  cout << "▦▨　　　▨▨　　　　▨▨▨▨▨　▨▨▨▨▨　▨▨▨▨▨　　　　　　　　　　　　　▨▦";
  gotoXY(5, 13);
  cout << "▦▨　　　▨▨　　　　▨▨　▨▨　▨▨　　　　▨▨　　　　　　　　　　　　　　　　▨▦";
  gotoXY(5, 14);
  cout << "▦▨　　　▨▨　　　　▨▨　▨▨　▨▨▨▨▨　▨▨▨▨▨　　　　　　　　　　　　　▨▦";
  gotoXY(5, 15);
  cout << "▦▨　　　▨▨　　　　▨▨　▨▨　　　　▨▨　▨▨　　　　　　　　　　　　　　　　▨▦";
  gotoXY(5, 16);
  cout << "▦▨　　　▨▨▨▨▨　▨▨▨▨▨　▨▨▨▨▨　▨▨▨▨▨　　▨▨　▨▨　▨▨　　　▨▦";
  gotoXY(5, 17);
  cout << "▦▨　　　▨▨▨▨▨　　▨▨▨　　▨▨▨▨▨　▨▨▨▨▨　　▨▨　▨▨　▨▨　　　▨▦";
  gotoXY(5, 18);
  cout << "▦▨　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　▨▦";
  gotoXY(5, 19);
  cout << "▦▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▦";
  gotoXY(5, 20);
  cout << "▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦▦";
  gotoXY(-1, -1);

  for (double cur = 0; cur < 5.0; cur += 0.1) {
    Sleep(100);
  }
}