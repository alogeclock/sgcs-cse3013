#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofSetFrameRate(60); // 프레임률을 60fps로 고정
  ofBackground(0);    // 배경을 명도 0(검정색)으로 고정
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  ofBackground(0); // 배경을 명도 0(검정색)으로 고정
  ofSetColor(255); // 흰색 설정

  ofFill();                                    // 도형 안쪽 채우기
  ofDrawRectangle(50, 50, 100, 100);           // (50, 50) 좌표 100*100 크기 사각형
  ofDrawCircle(250, 100, 50);                  // (250, 100) 좌표 반지름 50 원
  ofDrawEllipse(400, 100, 80, 100);            // (400, 100) 좌표 80*100 크기 타원
  ofDrawTriangle(500, 150, 550, 50, 600, 150); // 순서대로 세 점의 x, y좌표
  ofDrawLine(700, 50, 700, 150);               // 순서대로 두 점의 x, y좌표

  ofNoFill();                                   // 도형 안쪽 비우기
  ofDrawRectangle(50, 200, 100, 100);           // (50, 200) 좌표 100*100 크기 사각형
  ofDrawCircle(250, 250, 50);                   // (250, 250) 좌표 반지름 50 원
  ofDrawEllipse(400, 250, 80, 100);             // (400, 250) 좌표 80*100 크기 타원
  ofDrawTriangle(500, 300, 550, 200, 600, 300); // 순서대로 세 점의 x, y좌표
  ofDrawLine(700, 200, 700, 300);               // 순서대로 두 점의 x, y좌표
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
