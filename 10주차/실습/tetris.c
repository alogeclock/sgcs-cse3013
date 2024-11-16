﻿#include "tetris.h"

static struct sigaction act, oact;

int main() {
  int exit = 0;
  createRankList();
  initscr();
  noecho();
  keypad(stdscr, TRUE);

  srand((unsigned int)time(NULL));

  while (!exit) {
    clear();
    switch (menu()) {
    case MENU_PLAY:
      play();
      break;
    case MENU_RANK:
      rank();
      break;
    case MENU_REC_PLAY:
      recommendedPlay();
      break;
    case MENU_EXIT:
      exit = 1;
      break;
    default:
      break;
    }
  }

  endwin();
  system("clear");
  writeRankFile();
  return 0;
}

void InitTetris() {
  int i, j;

  for (j = 0; j < HEIGHT; j++)
    for (i = 0; i < WIDTH; i++)
      field[j][i] = 0;
	
	for (int i = 0; i < BLOCK_NUM; i++) 
		nextBlock[i] = rand() % 7;

  blockRotate = 0;
  blockY = -3;
  blockX = WIDTH / 2 - 2;
  score = 0;
  gameOver = 0;
  timed_out = 0;

  UpdateRecRoot(); // 추천 시스템의 root 노드 초기화
  DrawOutline();
  DrawField();
	recommend(recRoot);
  // modifiedRecommend(recRoot);
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
  DrawNextBlock(nextBlock);
  PrintScore(score);
}

void DrawOutline() {
  int i, j;
  /* 블럭이 떨어지는 공간의 태두리를 그린다.*/
  DrawBox(0, 0, HEIGHT, WIDTH);

  /* next block을 보여주는 공간의 태두리를 그린다.*/
  move(2, WIDTH + 10);
  printw("NEXT BLOCK");
  for (i = 0; i < BLOCK_NUM - 1; i++) {
    DrawBox(3 + 6 * i, WIDTH + 10, 4, 8);
  }

  /* score를 보여주는 공간의 태두리를 그린다.*/
  move(4 + 6 * (BLOCK_NUM - 1), WIDTH + 10);
  printw("SCORE");
  DrawBox(5 + 6 * (BLOCK_NUM - 1), WIDTH + 10, 1, 8);
}

int GetCommand() {
  int command;
  command = wgetch(stdscr);
  switch (command) {
  case KEY_UP:
    break;
  case KEY_DOWN:
    break;
  case KEY_LEFT:
    break;
  case KEY_RIGHT:
    break;
  case ' ': /* space key*/
    /*fall block*/
    break;
  case 'q':
  case 'Q':
    command = QUIT;
    break;
  default:
    command = NOTHING;
    break;
  }
  return command;
}

int ProcessCommand(int command) {
  int ret = 1;
  int drawFlag = 0;
  switch (command) {
  case QUIT:
    ret = QUIT;
    break;
  case KEY_UP:
    if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4, blockY, blockX)))
      blockRotate = (blockRotate + 1) % 4;
    break;
  case KEY_DOWN:
    if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
      blockY++;
    break;
  case KEY_RIGHT:
    if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX + 1)))
      blockX++;
    break;
  case KEY_LEFT:
    if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX - 1)))
      blockX--;
    break;
  default:
    break;
  }
  if (drawFlag) DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
  return ret;
}

void DrawField() {
  int i, j;
  for (j = 0; j < HEIGHT; j++) {
    move(j + 1, 1);
    for (i = 0; i < WIDTH; i++) {
      if (field[j][i] == 1) {
        attron(A_REVERSE);
        printw(" ");
        attroff(A_REVERSE);
      } else
        printw(".");
    }
  }
}

void PrintScore(int score) {
  move(6 + 6 * (BLOCK_NUM - 1), WIDTH + 11);
  printw("%8d", score);
}

void DrawNextBlock(int *nextBlock) {
  int i, j, k;
  for (k = 0; k < BLOCK_NUM - 1; k++) {
    for (i = 0; i < 4; i++) {
      move(4 + 6 * k + i, WIDTH + 13);
      for (j = 0; j < 4; j++) {
        if (block[nextBlock[k + 1]][0][i][j] == 1) {
          attron(A_REVERSE);
          printw(" ");
          attroff(A_REVERSE);
        } else
          printw(" ");
      }
    }
  }
}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile) {
  int i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) {
      if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
        move(i + y + 1, j + x + 1);
        attron(A_REVERSE);
        printw("%c", tile);
        attroff(A_REVERSE);
      }
    }

  move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width) {
  int i, j;
  move(y, x);
  addch(ACS_ULCORNER);
  for (i = 0; i < width; i++)
    addch(ACS_HLINE);
  addch(ACS_URCORNER);
  for (j = 0; j < height; j++) {
    move(y + j + 1, x);
    addch(ACS_VLINE);
    move(y + j + 1, x + width + 1);
    addch(ACS_VLINE);
  }
  move(y + j + 1, x);
  addch(ACS_LLCORNER);
  for (i = 0; i < width; i++)
    addch(ACS_HLINE);
  addch(ACS_LRCORNER);
}

void play() {
  int command;
  clear();

  if (recRoot != NULL) {
    free(recRoot);
    recRoot = NULL;
  }
  UpdateRecRoot();

  act.sa_handler = BlockDown;
  sigaction(SIGALRM, &act, &oact);
  InitTetris();
  do {
    if (timed_out == 0) {
      alarm(1);
      timed_out = 1;
    }

    command = GetCommand();
    if (ProcessCommand(command) == QUIT) {
      alarm(0);
      DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
      move(HEIGHT / 2, WIDTH / 2 - 4);
      printw("Good-bye!!");
      refresh();
      getch();
      return;
    }
  } while (!gameOver);

  alarm(0);
  getch();
  DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
  move(HEIGHT / 2, WIDTH / 2 - 4);
  printw("GameOver!!");
  refresh();
  getch();
  newRank(score);
}

char menu() {
  printw("1. play\n");
  printw("2. rank\n");
  printw("3. recommended play\n");
  printw("4. exit\n");
  return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char field[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int ty = blockY + i;
      int tx = blockX + j;
      if (block[currentBlock][blockRotate][i][j] == 1) {
        if (ty <= -1) {
          if (tx >= 0 && tx < WIDTH)
            continue;
          else
            return FALSE;
        }
        if (ty >= HEIGHT || tx < 0 || tx >= WIDTH) return FALSE;
        if (field[ty][tx] == 1) return FALSE;
      }
    }
  }
  return TRUE;
}

void DrawChange(char field[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX) {
  // 1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
  // 2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
  // 3. 새로운 블록 정보를 그린다.

  int py = blockY, px = blockX, pr = blockRotate; // prev_y, prev_x, prev_rotate;
  int sy, sx;                                     // shadow_y, shadow_x

  if (command == KEY_UP) pr = (pr + 3) % 4;
  if (command == KEY_DOWN) py--;
  if (command == KEY_LEFT) px++;
  if (command == KEY_RIGHT) px--;

  // 블록 삭제
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      if (block[currentBlock][pr][i][j] == 1 && py + i >= 0) {
        move(i + py + 1, j + px + 1);
        printw(".");
      }
    }

  // 그림자 삭제 (2주차)
  sy = py;
  sx = px;
  while (CheckToMove(field, nextBlock[0], pr, sy + 1, sx))
    sy++;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      if (block[currentBlock][pr][i][j] == 1 && sy + i >= 0) {
        move(i + sy + 1, j + sx + 1);
        printw(".");
      }
    }

  // 추천된 블록 삭제 (3주차)
  
  move(HEIGHT, WIDTH + 10);

  // DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
  DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig) {
  if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX) == TRUE) {
    blockY++;
    DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
  } else {
    if (blockY == -3) {
      gameOver = TRUE;
    } else {
      score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
      score += DeleteLine(field);

      for (int i = 0; i < BLOCK_NUM - 1; i++) {
        nextBlock[i] = nextBlock[i + 1];
      }
      nextBlock[BLOCK_NUM - 1] = rand() % 7;
      blockRotate = 0;
      blockY = -3;
      blockX = WIDTH / 2 - 2;

      DrawNextBlock(nextBlock);
      PrintScore(score);
      DrawField();

      // 추천 블록은 현재 블록 위치가 확정될 때마다 단 한 번만 계산한 뒤, 화면에 출력
      // 이전 블록은 DrawField()에 의해 자동으로 가려지므로 따로 삭제할 필요 X

      UpdateRecRoot();
      recommend(recRoot);
    }
  }
  timed_out = 0;
}

int AddBlockToField(char field[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
  // Block이 추가된 영역의 필드값을 바꾼다.
  int temp = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int ty = blockY + i;
      int tx = blockX + j;
      if (block[currentBlock][blockRotate][i][j] == 1) {
        if (ty >= HEIGHT || tx < 0 || tx >= WIDTH) continue;
        field[ty][tx] = 1;
        if (i == 3 || block[currentBlock][blockRotate][i + 1][j] == 0) {
          if (ty + 1 >= HEIGHT || field[ty + 1][tx] == 1) temp += 10;
        }
      }
    }
  }
  return temp;
}

int DeleteLine(char field[HEIGHT][WIDTH]) {
  // 1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
  // 2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
  int del_line = 0;
  for (int i = 0; i < HEIGHT; i++) {
    int full_line = TRUE;
    for (int j = 0; j < WIDTH; j++) {
      if (field[i][j] == 0) {
        full_line = FALSE;
        break;
      }
    }
    if (full_line == TRUE) {
      del_line++;
      for (int p = i; p >= 0; p--) {
        for (int q = 0; q < WIDTH; q++) {
          field[p][q] = field[p - 1][q];
        }
      }
      for (int q = 0; q < WIDTH; q++)
        field[0][q] = 0;
      i--;
    }
  }

  return del_line * del_line * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID, int blockRotate) {
  // user code
  while (CheckToMove(field, nextBlock[0], blockRotate, y + 1, x))
    y++;
  DrawBlock(y, x, blockID, blockRotate, '/');
}

///////////////////////////////////////////////////////////////////////////

void createRankList() {
  // 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
  // "rank.txt"열기 → 파일에서 랭킹정보 읽어오기 → LinkedList로 저장 → 파일 닫기
  FILE *fp;

  // 1. 파일 열기
  fp = fopen("rank.txt", "r");

  if (fp == NULL) {
    printf("file open error.\n");
    exit(0);
  }

  // 2. 정보읽어오기

  if (fscanf(fp, "%d", &rankNum) != EOF) { // 첫 줄에 데이터 개수 입력받음
    Node *curNode = rankRoot;
    for (int i = 0; i < rankNum; i++) {
      int rank_score;
      char rank_name[NAMELEN + 1];
      fscanf(fp, "%s%d", rank_name, &rank_score);

      Node *newNode = (Node *)malloc(sizeof(Node));
      newNode->score = rank_score;
      newNode->next = NULL;
      strcpy(newNode->name, rank_name);

      if (rankRoot == NULL) {
        rankRoot = newNode;
        curNode = newNode;
      } else {
        curNode->next = newNode;
        curNode = newNode;
      }
    }
  }
  // 4. 파일닫기
  fclose(fp);
}

void rank() {
  // 목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
  // 1. 문자열 초기화
  int X = 1, Y = rankNum, ch, i, j;
  clear();

  // 2. printw()로 3개의 메뉴출력
  printw("1. list ranks from X to Y\n");
  printw("2. list ranks by a specific name\n");
  printw("3. delete a specific rank\n");

  // 3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
  ch = wgetch(stdscr);

  // 4. 각 메뉴에 따라 입력받을 값을 변수에 저장
  // 4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
  echo();
  if (ch == '1') {
    printw("X: ");
    scanw("%d", &X);
    printw("Y: ");
    scanw("%d", &Y);

    if (X < 1) X = 1;
    if (Y > rankNum) Y = rankNum;
    if (X > Y || rankNum == 0) {
      printw("search failure: no rank in the list\n");
      noecho();
    }

    else {
      printw("\n        name        |     score     \n");
      printw("------------------------------------\n");

      int curRank = 1;
      Node *curNode = rankRoot;
      move(8, 0);
      while (curNode != NULL) {
        if (curRank >= X && curRank <= Y) {
          printw("%-19s | %-15d\n", curNode->name, curNode->score);
        }
        curNode = curNode->next;
        curRank++;
      };
    }
  }

  // 4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
  else if (ch == '2') {
    char str[NAMELEN + 1];
    int check = 0;

    printw("input the name: ");
    getstr(str); // 이름을 입력

    move(6, 0);
    printw("        name        |     score     \n");
    printw("------------------------------------\n");
    Node *curNode = rankRoot;
    move(8, 0);
    while (curNode != NULL) {
      if (strcmp(curNode->name, str) == 0) {
        printw("%-19s | %-15d\n", curNode->name, curNode->score);
      }
      curNode = curNode->next;
    }
  }

  // 4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
  else if (ch == '3') {
    int input; // 삭제할 데이터의 rank
    printw("input the rank: ");
    scanw("%d ", &input); // 순위를 입력

    Node *curNode = rankRoot, *prevNode = NULL;
    int curRank = 1;

    if (input < 1 || input > rankNum)
      printw("\nsearch failure: no rank in the list\n");

    else {
      rankNum--;
      while (curNode != NULL) {
        if (curRank == input) {
          printw("\nresult: the rank deleted.\n");
          if (prevNode == NULL) // 1등이 삭제됐다면
            rankRoot = curNode->next;
          else
            prevNode->next = curNode->next;
          free(curNode);
          break;
        }
        curRank++;
        prevNode = curNode;
        curNode = curNode->next;
      }
    }
    writeRankFile();
  }

  noecho();
  getch();
}

void writeRankFile() {
  // 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
  // 1. "rank.txt"를 연다.
  FILE *fp = fopen("rank.txt", "w");

  // 2. 랭킹 정보들의 수를 "rank.txt"에 기록
  fprintf(fp, "%d\n", rankNum);

  // 3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
  Node *curNode = rankRoot;
  Node *prevNode = NULL;
  while (curNode != NULL) {
    fprintf(fp, "%s %d\n", curNode->name, curNode->score);
    prevNode = curNode;
    curNode = curNode->next;
  }

  fclose(fp);
}

void newRank(int score) {
  // 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
  char str[NAMELEN + 1];
  clear();
  echo();

  // 1. 사용자 이름을 입력받음
  printw("your name: ");
  getstr(str); // 이름을 입력
  noecho();

  // 2. 새로운 노드를 생성해 이름과 점수를 저장
  Node *curNode = rankRoot;
  Node *prevNode = NULL;

  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->score = score;
  newNode->next = NULL;
  strcpy(newNode->name, str);

  rankNum++;          // 기록된 순위 개수 증가
  int found_rank = 0; // rank 삽입 성공 시 flag set

  while (curNode != NULL) {
    if (newNode->score >= curNode->score) {
      found_rank = 1;
      newNode->next = curNode;
      if (prevNode == NULL)
        rankRoot = newNode;
      else
        prevNode->next = newNode;
      break;
    }
    prevNode = curNode;
    curNode = curNode->next;
  }
  if (!found_rank) {
    if (prevNode == NULL) // rank에 저장된 데이터가 없을 경우
      rankRoot = newNode;
    else { // 입력된 데이터가 꼴등일 경우
      prevNode->next = newNode;
      newNode->next = curNode;
    }
  }

  writeRankFile();
}

void UpdateRecRoot() {
  // 노드 동적 할당
  if (recRoot == NULL) {
    RecNode *curNode = (RecNode *)malloc(sizeof(RecNode));
    curNode->lv = 0;
	  curNode->score = 0;
    recRoot = curNode;
  }

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      recRoot->f[i][j] = field[i][j];
    } 
  }
}

int recommend(RecNode *root) {
  // recommend()는 BLOCK_NUM을 변경하더라도 문제 없이 작동
  // 현재 블록과 다음 2개의 블록을 고려해서 모든 play sequence를 나타내는 tree 구성
  
  // 1. 블록의 가능한 형태, 놓일 수 있는 위치에 따라 RecNode 생성
  // 2. 현재 lv이 최대 lv보다 작으면, 누적 score 계산을 위해 재귀적 함수 호출
  // 3. 고려하는 lv과 최대 lv이 같으면, accumulatedScore에 점수 저장
  // 4. 최대 누적 점수, 현재 얻어진 점수를 비교해서 블록의 회전수, 위치 갱신

  // 블록의 가능한 형태를 저장하는 배열 blockRotateShape
  // const int blockRotateShape[] = { 2, 4, 4, 4, 1, 2, 2 };
  
  // 현재 추천 블록의 회전, 위치 정보를 기록하는 전역 변수 recommend_
  // int recommendR, recommendY, recommendX; 

  // 백트래킹(backtracking) 방식으로 구현
  
	if (root->lv + 1 >= VISIBLE_BLOCKS) return 0;

  int accumulatedScore = 0; // 최대 누적 점수를 저장하는 변수

  // rr, rx, ry → rec_rotate, rec_x, rec_y: 추천 블록의 회전수, 위치
  for (int rr = 0; rr < blockRotateShape[nextBlock[root->lv]]; rr++) { // 회전수
    for (int rx = -2; rx < 13; rx++) { // 가로 길이는 0, 10 기준 왼쪽/오른쪽 2칸이 남을 수 있음
      if (!CheckToMove(root->f, nextBlock[root->lv], rr, -2, rx)) {
        continue;
      }

      RecNode *curNode = (RecNode *)malloc(sizeof(RecNode));
      curNode->lv = root->lv + 1;

      // root 노드의 필드 복사
      for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
          curNode->f[i][j] = root->f[i][j];

      int ry = -3;
      while (CheckToMove(curNode->f, nextBlock[root->lv], rr, ry + 1, rx))
        ry++;
      
      curNode->score = 10000; // curNode->score가 음수가 될 수 있으므로 10000 초기화
      curNode->score += AddBlockToField(curNode->f, nextBlock[root->lv], rr, ry, rx);
      curNode->score += DeleteLine(curNode->f);

      for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
          if (curNode->f[j][i] == 1) {
            while (j < HEIGHT) {
              j++;
              if (curNode->f[j][i] == 0) curNode->score -= 50;
              curNode->score -= 10 * (HEIGHT - j); // 블록이 높을수록 점수 감소율 ↑
            }
          }
        }
      }

      // 현재 lv이 최대 lv보다 작으면 누적 score 계산을 위해 재귀적 함수 호출

      if (curNode->lv < VISIBLE_BLOCKS) 
        curNode->score += recommend(curNode);

      // 현재 블록의 점수가 가장 높다면 추천 블록의 위치 갱신
      if (curNode->score >= accumulatedScore) {
        accumulatedScore = curNode->score;
        recommendX = rx;
        recommendY = ry;
        recommendR = rr;
      }

      free(curNode);
      curNode = NULL;
    }
  }

  return accumulatedScore;
}

int modifiedRecommend(RecNode *root) {
}

void recommendedPlay() {
  // 추천 시스템에 따라 플레이할 수 있도록 설정{
  // 1. 종료(quit) 제외 유저의 입력을 받지 않고 자동으로 플레이
  /*
  int command;
  clear();
  act.sa_handler = BlockDown;
  sigaction(SIGALRM, &act, &oact);
  InitTetris();
  do {
    if (timed_out == 0) {
      alarm(1);
      timed_out = 1;
    }

    command = GetCommand();
    if (ProcessCommand(command) == QUIT) {
      alarm(0);
      DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
      move(HEIGHT / 2, WIDTH / 2 - 4);
      printw("Good-bye!!");
      refresh();
      getch();

      return;
    }
  } while (!gameOver);

  alarm(0);
  getch();
  DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
  move(HEIGHT / 2, WIDTH / 2 - 4);
  printw("GameOver!!");
  refresh();
  getch();
  newRank(score);
  */
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
  // user code
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawBlock(recommendY, recommendX, blockID, recommendR, 'R');
  DrawShadow(y, x, blockID, blockRotate);
}
