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
    ofSetRectMode(OF_RECTMODE_CENTER);   // 도형이 왼쪽 위가 아닌 중앙에서 그려짐
    int numRects = 10;                   // 직사각형 10개 생성
    for (int r = 0; r < numRects; r++) { // 개수만큼 생성
      ofSetColor(ofRandom(50, 255));     // 명도 50~255 범위 무작위 색 설정
      float width = ofRandom(5, 20);     // 직사각형 넓이
      float height = ofRandom(5, 20);    // 직사각형 길이
      float xOffset = ofRandom(-40, 40); // 생성 x좌표 오차 범위
      float yOffset = ofRandom(-40, 40); // 생성 y좌표 오차 범위
      // 직사각형 그리기
      ofDrawRectangle(ofGetMouseX() + xOffset, ofGetMouseY() + yOffset, width,
                      height);
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
