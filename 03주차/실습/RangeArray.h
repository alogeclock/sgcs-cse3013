#ifndef MTT
#define MTT
#include "Array.h"

class RangeArray : public Array {
 protected:
  int low;
  int high;

 public:
  RangeArray(int, int);
  ~RangeArray();

  int baseValue();
  int endValue();

  int& operator[](int);
  int operator[](int) const;  // const, 이 함수 안에선 함수 변경이 이뤄지지 않음
};

#endif
