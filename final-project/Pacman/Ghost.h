#pragma once

class Ghost {
private:
	int x, y, dir, time, color, origin_color, isDead;

public:
	// ���� ��ǥ, ���� ���, ������ �Է¹޴� ���� ��ü ������
	Ghost(int pos_x, int pos_y, int pos_dir, int color) {
		x = pos_x;
		y = pos_y;
		dir = pos_dir;
		this->color = color;
		origin_color = color; // ������ ���� ���� ���������� ����
		time = 0;			  // Ÿ�̸� �ʱ�ȭ
		isDead = 0;			  // ���� ���� �ʱ�ȭ
	}
	void print() const { printEntity(x, y, "��", color); } // ������ ���� ��ǥ���� �� ���
	int getX() const { return x; }         // x ��ǥ getter
	int getY() const { return y; }         // y ��ǥ getter
	int updateTime() { return time--; }    // Ÿ�̸� ��� �� ���� Ÿ�̸� ��ȯ
	int getDead() const { return isDead; } // ���� ���� getter

	// ����� �������� ȹ���� ĳ���Ϳ� �浹 �� ���� ��� ����
	void die() {
		x = 23, y = 23;
		isDead = 50;
	}

	// ������ ���� ���� ����
	void update(int character_x, int character_y) {
		time = 2; // Ÿ�̸� �ʱ�ȭ

		// �׾� �ִ� ���¶�� ��Ƴ� ������ ��ã�� ����
		if (isDead-- > 0) {
			x = 23, y = 23;
			color = LIGHTGRAY;
			print();
			return;
		}
		else if (SPECIAL_FLAG) color = BLUE; // ����� ������ �Ծ��ٸ� ���� ����
		else color = origin_color;           // �׷��� �ʴٸ� ���������� ����

		move(character_x, character_y);      // ĳ���� �̵�
	}
	
	// ��ã�⸦ ���� ���ΰ� ĳ���� �Ѹ� Ž��
	void move(int character_x, int character_y) {
		int vst[25][25]; memset(vst, 0x3f, sizeof(vst));
		queue<Path> q; vector<Vertex> initial_path; 

		// �ֺ��� ĳ���Ͱ� �ִ��� BFS Ž��, ĳ������ ��ġ�� ���� dir ����
		q.push(Path(x, y, 0, initial_path));
		vst[y][x] = 1;

		// ǥ������ BFS �˰��򿡼� ��θ� �߰������� ������
		while (q.size()) {
			int cx, cy, cdst; vector<Vertex> path;
			q.front().copy(cx, cy, path); // ��ǥ�� ��� ������ ����
			cdst = vst[cy][cx];
			q.pop(); // ť pop

			// ĳ���͸� Ž������ ���, ĳ���͸� ���� �ִ� ��η� �̵�
			if (cx == character_x && cy == character_y && path.size() > 1) {
				dir = path[1].dir; // ���� ���� ����
				break;
			}

			if (path.size() > 50) break; // �� ��ü�� Ŀ���� �� �ֵ��� ����

			for (int i = 0; i < 4; i++) {
				int ny = cy + dy[i], nx = cx + dx[i], ndst = cdst + 1;    // ���� idx�� ���� ���� ��ǥ ���
				if (nx <= 0 || ny <= 0 || nx >= 24 || ny >= 24) continue; // ��谪 ó��
				if (Map[ny][nx] == WALL || vst[ny][nx] < ndst) continue;  // �̵� �Ұ����ϰų�, �ִ� �Ÿ��� �ƴ� �� ���
				vst[ny][nx] = ndst;                       // �ִ� �Ÿ��� �� �湮 �迭�� ����
				q.push(Path(nx, ny, direction(i), path)); // Path ť�� push
			}
		}

		moveEntity(x, y, dir, "��", color); // �ִ� ��θ� ���� �̵�
	}
};

void meetGhost(int cx, int cy, Ghost& ghost); // ���ɰ� ĳ������ �浹 �� ó��
void printMap(int cx, int cy, Ghost ghost1, Ghost ghost2); // ���� �� ���� ���