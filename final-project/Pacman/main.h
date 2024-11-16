#pragma once
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <stack>
#include <queue>
#include <random>
using namespace std;

#define MAP_POS_X 2
#define MAP_POS_Y 1

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

// map
#define BLANK 0
#define WALL 1
#define FOOD 2
#define SPECIAL 3

// color
#define BLACK 0 
#define BLUE 1 
#define GREEN 2 
#define CYAN 3 
#define RED 4 
#define MAGENTA 5 
#define BROWN 6 
#define LIGHTGRAY 7 
#define DARKGRAY 8 
#define LIGHTBLUE 9 
#define LIGHTGREEN 10 
#define LIGHTCYAN 11 
#define LIGHTRED 12 
#define LIGHTMAGENTA 13 
#define YELLOW 14 
#define WHITE 15 

void gotoXY(int x, int y);							// 매개변수 좌표로 이동
void textColor(int foreground, int background);		// 텍스트 색깔 변환
void cursorHide();									// 화면의 깔끔한 출력을 위한 커서 숨기기

void initPacMan();				// 팩맨 게임 초기화, 미로 생성
void printUI();					// UI 출력
void dfs(int x, int y);			// 미로 생성을 위한 recursive-dfs 알고리즘
void isGameOver();				// 게임 종료 여부 확인
void win();						// 승리 처리
void lose();					// 패배 처리

void getFood(int x, int y);												 // 아이템 획득 처리
bool isMovable(int x, int y, int dir);									 // 개체의 이동 가능 여부 파악
void printEntity(int x, int y, const char* string, int color);           // 입력된 좌표에 개체 출력
void moveEntity(int& x, int& y, int dir, const char* string, int color); // 입력된 좌표와 이동 방향에 개체 이동
int direction(int dir);                                                  // dy[], dx[] 방향 idx 값을 방향 상수로 변환

const int dy[] = { -1, 0, 1, 0 }, dx[] = { 0, 1, 0, -1 }; // UP, RIGHT, DOWN, LEFT
int Map[25][25], vst[25][25];                             // 게임 맵, recursive-dfs 미로 생성을 위한 방문 맵
int key_stroke, score, playing_round = 1;				  // 사용자의 키 입력, 게임 점수, 플레이 중인 라운드
int game_over, game_win;                                  // 게임 패배, 게임 승리 여부 확인 변수
int SPECIAL_FLAG;										  // 스페셜 아이템 획득 여부 판정 변수

// 난수 생성을 위한 전역 변수
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 3);

// 유령의 bfs 길찾기를 위해 각 정점을 저장하는 vertex 클래스
class Vertex { 
public:
	int x, y, dir; 
	Vertex(int pos_x, int pos_y, int pos_dir) : x(pos_x), y(pos_y), dir(pos_dir) {}
};

// 유령의 bfs 길찾기를 위해 각 경로를 저장하는 Path 클래스
class Path {
private:
	int x, y, dir;       // 좌표, 방향
	vector<Vertex> path; // 현재까지의 경로

public:
	// 좌표, 경로를 입력받아 저장하는 생성자 (현재 입력된 정점을 입력받은 경로에 추가)
	Path(int pos_x, int pos_y, int pos_dir, vector<Vertex> prev_path) {
		this->x = pos_x, this->y = pos_y, this->dir = pos_dir;
		this->path = prev_path;
		this->path.push_back(Vertex(x, y, dir));
	}
	// 변수를 참조자로 입력받아 멤버 변수의 값을 입력받은 변수에 복사
	void copy(int& pos_x, int& pos_y, vector<Vertex>& cur_path) const {
		pos_x = x;
		pos_y = y;
		cur_path = path;
	}
};