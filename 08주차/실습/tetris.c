#include "tetris.h"

static struct sigaction act, oact;

int main() {
  int exit = 0;

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
    case MENU_EXIT:
      exit = 1;
      break;
    default:
      break;
    }
  }

  endwin();
  system("clear");
  return 0;
}

void InitTetris() {
  int i, j;

  for (j = 0; j < HEIGHT; j++)
    for (i = 0; i < WIDTH; i++)
      field[j][i] = 0;

  nextBlock[0] = rand() % 7;
  nextBlock[1] = rand() % 7;
  nextBlock[2] = rand() % 7;
  blockRotate = 0;
  blockY = -3;
  blockX = WIDTH / 2 - 2;
  score = 0;
  gameOver = 0;
  timed_out = 0;

  DrawOutline();
  DrawField();
  DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate, ' ');
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
  DrawBox(3, WIDTH + 10, 4, 8);

  /* next block을 보여주는 공간의 태두리를 그린다.*/
  move(6, WIDTH + 10);
  printw("NEXT BLOCK");
  DrawBox(7, WIDTH + 10, 4, 8);

  /* score를 보여주는 공간의 태두리를 그린다.*/
  move(9, WIDTH + 10);
  printw("SCORE");
  DrawBox(10, WIDTH + 10, 1, 8);
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
  move(11, WIDTH + 11);
  printw("%8d", score);
}

void DrawNextBlock(int *nextBlock) {
  for (int k = 0; k < BLOCK_NUM - 1; k++) {
    int i, j;
    for (i = 0; i < 4; i++) {
      move(4 * k + i, WIDTH + 13);
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
        if (ty <= -1) continue;
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

  int py, px, pr; // prev_y, prev_x, prev_rotate;

  if (command == KEY_UP) pr = (blockRotate + 3) % 4;
  if (command == KEY_DOWN) py = blockY - 1;
  if (command == KEY_LEFT) px = blockX + 1;
  if (command == KEY_RIGHT) px = blockX - 1;

  // 블록 삭제
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      if (block[currentBlock][blockRotate][i][j] == 1 && py + i >= 0) {
        move(i + py + 1, j + px + 1);
        printw(".");
      }
    }

  // 그림자 삭제

  move(HEIGHT, WIDTH + 10);

  // DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
  DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate, ' ');
}

void BlockDown(int sig) {
  if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX) == TRUE) {
    blockY++;
    DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
  } else {
    if (blockY == -3) {
      gameOver = TRUE;
    } else {
      AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
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
    }
  }
  timed_out = 0;
}

void AddBlockToField(char field[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
  // Block이 추가된 영역의 필드값을 바꾼다.
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int ty = blockY + i;
      int tx = blockX + j;
      if (block[currentBlock][blockRotate][i][j] == 1) {
        if (ty >= HEIGHT || tx < 0 || tx >= WIDTH) continue;
        field[ty][tx] = 1;
      }
    }
  }
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
  y++;
  while (CheckToMove(field, nextBlock[0], blockRotate, y, x))
    y++;
  DrawBlock(y - 1, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
  // user code
  DrawBlock(y, x, blockID, blockRotate);
  DrawShadow(y, x, blockID, blockRotate);
}

void createRankList() {
  // user code
}

void rank() {
  // user code
}

void writeRankFile() {
  // user code
}

void newRank(int score) {
  // user code
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
  // user code
}

int recommend(RecNode *root) {
  int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

  // user code

  return max;
}

void recommendedPlay() {
  // user code
}
