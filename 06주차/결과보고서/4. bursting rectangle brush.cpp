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
  // 마우스 왼쪽 클릭 시
  if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
    ofSetColor(255);                   // 흰색 설정
    ofSetRectMode(OF_RECTMODE_CENTER); // 도형이 왼쪽 위가 아닌 중앙에서 그려짐
    // 마우스 좌표에서 50 * 50 크기의 정사각형 생성
    ofDrawRectangle(ofGetMouseX(), ofGetMouseY(), 50, 50);
  }
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
