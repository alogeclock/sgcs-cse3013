#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofSetFrameRate(60);         // �����ӷ��� 60fps�� ����
  ofSetBackgroundAuto(false); // ����� �ڵ� ������ ����
  ofBackground(0);            // ����� �� 0(������)���� ����
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  // ���콺 ���� Ŭ�� ��
  if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
    ofSetRectMode(OF_RECTMODE_CENTER);   // ������ ���� ���� �ƴ� �߾ӿ��� �׷���
    int numRects = 10;                   // ���簢�� 10�� ����
    for (int r = 0; r < numRects; r++) { // ������ŭ ����
      ofSetColor(ofRandom(50, 255));     // �� 50~255 ���� ������ �� ����
      float width = ofRandom(5, 20);     // ���簢�� ����
      float height = ofRandom(5, 20);    // ���簢�� ����
      float xOffset = ofRandom(-40, 40); // ���� x��ǥ ���� ����
      float yOffset = ofRandom(-40, 40); // ���� y��ǥ ���� ����
      // ���簢�� �׸���
      ofDrawRectangle(ofGetMouseX() + xOffset, ofGetMouseY() + yOffset, width,
                      height);
    }
  }
  // ������ Ŭ���� ��� ��� �ʱ�ȭ
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
