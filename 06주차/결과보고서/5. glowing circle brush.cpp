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
    int maxRadius = 100;    // �귯���� ũ��(=���� ũ��)
    int radiusStepSize = 5; // ���� �� �� ��ĥ�� ����
    // radius / radiusStepSize�� ����ŭ ���� ��ħ
    int alpha = 3;               // �귯���� ������, ���̸� ������
    int maxOffsetDistance = 100; // ���� ��ѷ����� ����
    for (int radius = maxRadius; radius > 0; radius -= radiusStepSize) {
      float angle = ofRandom(ofDegToRad(360.0));
      // ���� ��� �������� �̵������� ���� ���� ����
      // sin�Լ�, cos�Լ��� angle ���� �־�� �ϹǷ� ofDegToRad() �Լ� ���
      float distance = ofRandom(maxOffsetDistance); // ���� ���� ����
      float xOffset = cos(angle) * distance;        // x��ǥ �̵�
      float yOffset = sin(angle) * distance;        // y��ǥ �̵�

      // ofColor class ������ �� ��ü��(R��, G��, B��, ����(alpha) ��)
      ofColor myOrange(255, 132, 0, alpha); // myOrange ��ü ����
      ofColor myRed(255, 6, 0, alpha);      // myRed ��ü
      ofColor inBetween = myOrange.getLerped(myRed, ofRandom(1.0));
      // myOrange, myRed ��ü�� ���� ������ ����(0.0 ~ 1.0)�� ����
      ofSetColor(inBetween); // ���� �� �ʱ�ȭ

      ofDrawCircle(ofGetMouseX() + xOffset, ofGetMouseY() + yOffset, radius);
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
