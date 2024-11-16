#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofSetFrameRate(60);         // 프레임률을 60fps로 고정
  ofSetBackgroundAuto(false); // 배경의 자동 갱신을 방지
  ofBackground(0);            // 배경을 명도 0(검정색)으로 고정
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  // 마우스 왼쪽 클릭 시
  if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
    int maxRadius = 100;    // 브러쉬의 크기(=원의 크기)
    int radiusStepSize = 5; // 원을 몇 번 겹칠지 결정
    // radius / radiusStepSize의 값만큼 원을 겹침
    int alpha = 3;               // 브러쉬의 불투명도, 높이면 진해짐
    int maxOffsetDistance = 100; // 원이 흩뿌려지는 정도
    for (int radius = maxRadius; radius > 0; radius -= radiusStepSize) {
      float angle = ofRandom(ofDegToRad(360.0));
      // 원이 어느 방향으로 이동할지에 대한 각도 결정
      // sin함수, cos함수에 angle 값을 넣어야 하므로 ofDegToRad() 함수 사용
      float distance = ofRandom(maxOffsetDistance); // 범위 난수 설정
      float xOffset = cos(angle) * distance;        // x좌표 이동
      float yOffset = sin(angle) * distance;        // y좌표 이동

      // ofColor class 생성자 → 객체명(R값, G값, B값, 투명도(alpha) 값)
      ofColor myOrange(255, 132, 0, alpha); // myOrange 객체 생성
      ofColor myRed(255, 6, 0, alpha);      // myRed 객체
      ofColor inBetween = myOrange.getLerped(myRed, ofRandom(1.0));
      // myOrange, myRed 객체의 색을 무작위 비율(0.0 ~ 1.0)로 섞음
      ofSetColor(inBetween); // 원의 색 초기화

      ofDrawCircle(ofGetMouseX() + xOffset, ofGetMouseY() + yOffset, radius);
    }
  }
  // 오른쪽 클릭할 경우 배경 초기화
  if (ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT))
    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
