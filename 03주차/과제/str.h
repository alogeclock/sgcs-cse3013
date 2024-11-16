#ifndef __STR__
#define __STR__

class Str {
 private:
  char* str;  // string의 내용
  int len;    // string의 길이

 public:
  Str(int len);       // 길이를 기준으로 생성
  Str(char* neyong);  // 내용을 기준으로 생성
  ~Str();
  int length();                  // string의 길이를 반환
  char* contents();              // string의 내용을 반환
  int compare(Str& a);           // a의 내용과 strcmp
  int compare(char* a);          // a의 내용과 strcmp
  void operator=(char* a);       // string의 값을 a에 대입
  void operator=(class Str& a);  // string의 값을 a에 대입
};
#endif
