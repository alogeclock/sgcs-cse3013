#ifndef __LINKEDLIST__
#define __LINKEDLIST__

#include <iostream>
using namespace std;

// LinkedList Node
template <typename T>
class Node {
 public:
  T data;      // 데이터
  Node *link;  // 다음 노드의 주소 포인터
  Node(T element) : data(element), link(NULL) {}
};

// LinkedList Class
template <typename T>
class LinkedList {
 protected:
  Node<T> *first;    // 첫 번째 노드의 주소를 저장할 포인터
  int current_size;  // 배열 크기

 public:
  LinkedList() : first(NULL), current_size(0) {}  // 생성자
  int GetSize() { return current_size; };         // 노드 개수 리턴
  void Insert(T element);                         // 맨 앞에 원소를 삽입
  virtual bool Delete(T &element);                // 맨 뒤의 원소 삭제
  void Print();                                   // 리스트 출력
};

// 새 노드를 앞에 삽입
template <typename T>
void LinkedList<T>::Insert(T element) {
  Node<T> *newnode = new Node<T>(element);  // 노드 메모리 할당
  newnode->link = first;                    // 첫 번째 노드에 삽입
  first = newnode;                          // 첫 번째 포인터 초기화
  current_size++;                           // 배열 크기 갱신
}

// 마지막 노드의 값을 리턴하면서 메모리에서 할당 해제
template <typename T>
bool LinkedList<T>::Delete(T &element) {
  if (first == 0) return false;  // 배열 비어 있을 경우 0 반환

  Node<T> *current = first;  // 첫 번째 노드를 불러옴
  Node<T> *previous = NULL;

  // 마지막 노드까지 탐색
  while (1) {
    if (current->link == NULL) {
      if (previous)  // 이전 노드가 존재하면 (노드가 1개가 아니면)
        previous->link = current->link;  // 이전 노드의 링크 NULL로
      else
        first = first->link;  // 존재하지 않을 경우 first를 NULL로
      break;
    }
    previous = current;
    current = current->link;
  }
  element = current->data;  // element 값 삭제된 노드의 데이터로 갱신
  delete current;           // 마지막 포인터 메모리 반환(=삭제)
  current_size--;           // 배열 크기 갱신

  return true;
}

// 리스트를 출력하는 Print 함수
template <typename T>
void LinkedList<T>::Print() {
  Node<T> *i;
  int index = 1;

  if (current_size != 0) {
    for (i = first; i != NULL; i = i->link) {
      if (index == current_size) {
        cout << "[" << index << "|" << i->data << "]->";
      } else {
        cout << "[" << index << "|" << i->data << "]->";
        index++;
      }
    }
    cout << endl;
  }
}

#endif
