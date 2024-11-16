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

void gotoXY(int x, int y);							// �Ű����� ��ǥ�� �̵�
void textColor(int foreground, int background);		// �ؽ�Ʈ ���� ��ȯ
void cursorHide();									// ȭ���� ����� ����� ���� Ŀ�� �����

void initPacMan();				// �Ѹ� ���� �ʱ�ȭ, �̷� ����
void printUI();					// UI ���
void dfs(int x, int y);			// �̷� ������ ���� recursive-dfs �˰���
void isGameOver();				// ���� ���� ���� Ȯ��
void win();						// �¸� ó��
void lose();					// �й� ó��

void getFood(int x, int y);												 // ������ ȹ�� ó��
bool isMovable(int x, int y, int dir);									 // ��ü�� �̵� ���� ���� �ľ�
void printEntity(int x, int y, const char* string, int color);           // �Էµ� ��ǥ�� ��ü ���
void moveEntity(int& x, int& y, int dir, const char* string, int color); // �Էµ� ��ǥ�� �̵� ���⿡ ��ü �̵�
int direction(int dir);                                                  // dy[], dx[] ���� idx ���� ���� ����� ��ȯ

const int dy[] = { -1, 0, 1, 0 }, dx[] = { 0, 1, 0, -1 }; // UP, RIGHT, DOWN, LEFT
int Map[25][25], vst[25][25];                             // ���� ��, recursive-dfs �̷� ������ ���� �湮 ��
int key_stroke, score, playing_round = 1;				  // ������� Ű �Է�, ���� ����, �÷��� ���� ����
int game_over, game_win;                                  // ���� �й�, ���� �¸� ���� Ȯ�� ����
int SPECIAL_FLAG;										  // ����� ������ ȹ�� ���� ���� ����

// ���� ������ ���� ���� ����
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 3);

// ������ bfs ��ã�⸦ ���� �� ������ �����ϴ� vertex Ŭ����
class Vertex { 
public:
	int x, y, dir; 
	Vertex(int pos_x, int pos_y, int pos_dir) : x(pos_x), y(pos_y), dir(pos_dir) {}
};

// ������ bfs ��ã�⸦ ���� �� ��θ� �����ϴ� Path Ŭ����
class Path {
private:
	int x, y, dir;       // ��ǥ, ����
	vector<Vertex> path; // ��������� ���

public:
	// ��ǥ, ��θ� �Է¹޾� �����ϴ� ������ (���� �Էµ� ������ �Է¹��� ��ο� �߰�)
	Path(int pos_x, int pos_y, int pos_dir, vector<Vertex> prev_path) {
		this->x = pos_x, this->y = pos_y, this->dir = pos_dir;
		this->path = prev_path;
		this->path.push_back(Vertex(x, y, dir));
	}
	// ������ �����ڷ� �Է¹޾� ��� ������ ���� �Է¹��� ������ ����
	void copy(int& pos_x, int& pos_y, vector<Vertex>& cur_path) const {
		pos_x = x;
		pos_y = y;
		cur_path = path;
	}
};