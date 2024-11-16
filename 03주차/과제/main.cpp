#include <iostream>
using namespace std;

#include "Str.h"

int main() {
  Str a("I'm a girl");
  cout << a.contents();
  a = "I'm a boy\n";
  cout << a.contents();
  cout << a.compare("I'm a a") << endl;

  Str b("I'm a good boy\n");
  a = b;
  cout << a.contents();

  return 0;
}