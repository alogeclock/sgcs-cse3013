#pragma once

class Ghost {
private:
	int x, y, dir, time, color, origin_color, isDead;

public:
	// 현재 좌표, 방향 상수, 색깔을 입력받는 유령 개체 생성자
	Ghost(int pos_x, int pos_y, int pos_dir, int color) {
		x = pos_x;
		y = pos_y;
		dir = pos_dir;
		this->color = color;
		origin_color = color; // 생성될 때의 색을 본래색으로 저장
		time = 0;			  // 타이머 초기화
		isDead = 0;			  // 생존 여부 초기화
	}
	void print() const { printEntity(x, y, "●", color); } // 유령의 현재 좌표에서 값 출력
	int getX() const { return x; }         // x 좌표 getter
	int getY() const { return y; }         // y 좌표 getter
	int updateTime() { return time--; }    // 타이머 계산 및 현재 타이머 반환
	int getDead() const { return isDead; } // 생존 여부 getter

	// 스페셜 아이템을 획득한 캐릭터와 충돌 시 유령 사망 판정
	void die() {
		x = 23, y = 23;
		isDead = 50;
	}

	// 유령의 상태 정보 갱신
	void update(int character_x, int character_y) {
		time = 2; // 타이머 초기화

		// 죽어 있는 상태라면 살아날 때까지 길찾기 정지
		if (isDead-- > 0) {
			x = 23, y = 23;
			color = LIGHTGRAY;
			print();
			return;
		}
		else if (SPECIAL_FLAG) color = BLUE; // 스페셜 음식을 먹었다면 색깔 변경
		else color = origin_color;           // 그렇지 않다면 본래색으로 변경

		move(character_x, character_y);      // 캐릭터 이동
	}
	
	// 길찾기를 통한 주인공 캐릭터 팩맨 탐색
	void move(int character_x, int character_y) {
		int vst[25][25]; memset(vst, 0x3f, sizeof(vst));
		queue<Path> q; vector<Vertex> initial_path; 

		// 주변에 캐릭터가 있는지 BFS 탐색, 캐릭터의 위치에 따라 dir 갱신
		q.push(Path(x, y, 0, initial_path));
		vst[y][x] = 1;

		// 표준적인 BFS 알고리즘에서 경로만 추가적으로 저장함
		while (q.size()) {
			int cx, cy, cdst; vector<Vertex> path;
			q.front().copy(cx, cy, path); // 좌표와 경로 변수에 저장
			cdst = vst[cy][cx];
			q.pop(); // 큐 pop

			// 캐릭터를 탐색했을 경우, 캐릭터를 향한 최단 경로로 이동
			if (cx == character_x && cy == character_y && path.size() > 1) {
				dir = path[1].dir; // 방향 정보 저장
				break;
			}

			if (path.size() > 50) break; // 맵 전체를 커버할 수 있도록 설정

			for (int i = 0; i < 4; i++) {
				int ny = cy + dy[i], nx = cx + dx[i], ndst = cdst + 1;    // 방향 idx에 따른 다음 좌표 계산
				if (nx <= 0 || ny <= 0 || nx >= 24 || ny >= 24) continue; // 경계값 처리
				if (Map[ny][nx] == WALL || vst[ny][nx] < ndst) continue;  // 이동 불가능하거나, 최단 거리가 아닐 시 통과
				vst[ny][nx] = ndst;                       // 최단 거리일 시 방문 배열에 저장
				q.push(Path(nx, ny, direction(i), path)); // Path 큐에 push
			}
		}

		moveEntity(x, y, dir, "●", color); // 최단 경로를 향해 이동
	}
};

void meetGhost(int cx, int cy, Ghost& ghost); // 유령과 캐릭터의 충돌 시 처리
void printMap(int cx, int cy, Ghost ghost1, Ghost ghost2); // 현재 맵 상태 출력