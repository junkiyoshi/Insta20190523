#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);

	this->cap_size = cv::Size(1280, 720);
	this->cap.open("video.mp4");
	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
	this->shader.load("shader/shader.vert", "shader/shader.frag");
	this->font_size = 80;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	// Read mp4 file.
	cv::Mat src;
	this->cap >> src;
	if (src.empty()) {

		this->cap.set(CV_CAP_PROP_POS_FRAMES, 1);
		return;
	}
	cv::resize(src, this->frame, this->cap_size);
	cv::cvtColor(this->frame, this->frame, CV_BGR2RGB);
	this->image.update();

	// Draw image.
	this->fbo.begin();
	ofClear(0);
	ofBackground(60);
	ofSetColor(255);

	for (int x = 0; x < ofGetWidth(); x += this->font_size) {

		for (int y = this->font_size; y <= ofGetHeight(); y += this->font_size) {

			char noise_value = ofMap(ofNoise(x * 0.005, y * 0.0005 - ofGetFrameNum() * this->font_size * 0.00001), 0, 1, 'A', 'Z');
			this->font.drawString({ noise_value }, x, y);
		}
	}
	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniformTexture("tex1", this->image, 1);
	this->shader.setUniformTexture("tex2", this->fbo.getTexture(), 2);

	ofFill();
	ofSetColor(255);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(1280, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}