#include "Str.h"

#include <cstdlib>
#include <cstring>

Str::Str(int _len) {  // 문자열 길이를 입력받고
  len = _len;         // 입력받은 길이를 멤버 변수 len에 초기화
  str = new char[len + 1];  // 길이 + 널 문자만큼 메모리 할당
  str[0] = '\0';            // 첫 글자 널 문자 입력
}

Str::Str(char* neyong) {  // string 내용 인수로 받고
  len = strlen(neyong);  // 길이를 strlen()으로 확인한 후 멤버 변수 len에 초기화
  str = new char[len + 1];  // 길이만큼 메모리를 할당

  strcpy(str, neyong);  // strcpy()로 멤버 변수 str로 복사한 뒤
  str[len] = '\0';      // 마지막에 널 문자 입력
}

Str::~Str() {
  delete[] str;  // 메모리 할당 해제
}

int Str::length() {
  return len;  // len 변수(=길이) 반환
}

char* Str::contents() {
  return str;  // Str 변수의 내용 반환
}

int Str::compare(Str& a) {
  return strcmp(str, a.str);  // 멤버 변수 str과 a의 내용 a.str strcmp()
}

int Str::compare(char* a) {
  return strcmp(str, a);  // 멤버 변수 str과 char* 변수 a strcmp()
}

void Str::operator=(char* a) {  // 연산자 오버로딩
  len = strlen(a);              // strlen(a)로 길이를 확인 후 복사
  delete[] str;  // str의 메모리 할당 해제(메모리 낭비 또는 경계 초과 방지)

  str = new char[len + 1];  // a의 길이 + 널 문자만큼 메모리 할당
  strcpy(str, a);  // strcpy()로 내용 복사(strcpy는 널 문자까지 복사)
}

void Str::operator=(Str& a) {  // 연산자 오버로딩, 깊은 복사
  len = a.len;                 // 멤버 변수 len(문자열 길이)를 복사
  delete[] str;                // str의 메모리 할당 해제

  str = new char[len + 1];  // a의 길이 + 널 문자만큼 메모리 할당
  strcpy(str, a.str);       // strcpy()로 내용 복사
}