#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofSetFrameRate(60); // �����ӷ��� 60fps�� ����
  ofBackground(0);    // ����� �� 0(������)���� ����
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  // ���콺 ���� Ŭ�� ��
  if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
    ofSetColor(255);                   // ��� ����
    ofSetRectMode(OF_RECTMODE_CENTER); // ������ ���� ���� �ƴ� �߾ӿ��� �׷���
    // ���콺 ��ǥ���� 50 * 50 ũ���� ���簢�� ����
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
