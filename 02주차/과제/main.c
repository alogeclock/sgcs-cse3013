// idea: counting

#include "header.h"

int main() {
  int T;
  scanf("%d", &T);  // 테스트 케이스 입력

  while (T--) {
    int N;  // 페이지 수 입력
    scanf("%d", &N);

    int arr[10] = {0};  // 10억까진 overflow 존재하지 않음
    solve_page(N, arr);
    print(arr);
  }

  return 0;
}