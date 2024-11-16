#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
using namespace std;

int* parent;
char **maze;
int WIDTH, HEIGHT, VERTEX;

// 해당 노드의 부모 노드를 탐색
int find(int x) {
  if (parent[x] == x) return x;
  return parent[x] = find(parent[x]);
}

// cycle table에서 부모의 숫자가 더 작은 쪽으로 노드를 병합
void unite(int u, int v) {
  u = find(u), v = find(v);
  (u < v) ? parent[v] = u : parent[v] = u;
}

struct Edge {
  int start, end, weight;
  Edge(int s, int e, int w) : start(s), end(e), weight(w){};
  bool operator<(Edge &e) { return weight < e.weight; }
};

int main() {
  // 1 ~ 1000 범위의 난수 생성
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dis(1, 1000);
  vector<Edge> edge; // 간선을 저장할 1차원 벡터

  cout << "width: "; cin >> WIDTH;
  cout << "height: "; cin >> HEIGHT;
  VERTEX = WIDTH * HEIGHT;
  parent = new int[VERTEX + 1];

  // 미로 동적 할당
  maze = new char *[2 * HEIGHT + 1];
  for (int i = 0; i < 2 * HEIGHT + 1; i++) {
    maze[i] = new char[2 * WIDTH + 1];
  }

  // 미로의 모든 벽을 간선으로 추가(r: row, c: col)
  for (int r = 1; r <= HEIGHT; r++) {
    for (int c = 1; c <= WIDTH; c++) {
      int u = (r - 1) * WIDTH + c; // 현재 노드

      // 마지막 열/행이 아닐 경우 오른쪽/아래쪽 노드와 연결
      if (c < WIDTH) edge.push_back(Edge(u, u + 1, dis(gen)));
      if (r < HEIGHT) edge.push_back(Edge(u, u + WIDTH, dis(gen)));
    }
  }

  // 각 vector의 부모 노드를 나타내는 parent 배열
  for (int i = 1; i <= VERTEX; i++) {
    parent[i] = i;
  }

  sort(edge.begin(), edge.end());

  for (int i = 0; i <= 2 * HEIGHT; i++) {
    for (int j = 0; j <= 2 * WIDTH; j++) {
      if (i % 2 == 0 && j % 2 == 0)
        maze[i][j] = '+';
      else if (i % 2 == 0) maze[i][j] = '-';
      else if (j % 2 == 0) maze[i][j] = '|';
      else
        maze[i][j] = ' ';
    }
  }

  for (auto e : edge) {
    int u = e.start, v = e.end;
    if (find(u) != find(v)) {
      unite(u, v);
      int x = (u - 1) / WIDTH + (v - 1) / WIDTH + 1;
      int y = (u - 1) % WIDTH + (v - 1) % WIDTH + 1;
      maze[x][y] = ' ';
    }
  }

  // 미로 .maz 파일로 출력
  ofstream out("maze.maz");
  for (int i = 0; i <= 2 * HEIGHT; i++) {
    for (int j = 0; j <= 2 * WIDTH; j++) {
      out << maze[i][j];
    }
    out << '\n';
  }
  out.close();


  delete[] parent;
  for (int i = 0; i < 2 * HEIGHT + 1; i++) {
    delete[] maze[i];
  }
  delete[] maze;
}