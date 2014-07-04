#pragma once

#include "ofMain.h"
#include "ofxFft.h"
#include "ofxUI.h"
//#include "ofxProcessFFT.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void plot(vector<float>& buffer, float scale, float offset);
	void audioReceived(float* input, int bufferSize, int nChannels);
	void draw();
    void update();
    void guiEvent(ofxUIEventArgs &e);

	int plotHeight, bufferSize;
    int tBl;

	ofxFft* fft;
	
	ofMutex soundMutex;
	vector<float> drawBins, middleBins, audioBins;
    list<float> tBuffer1, tBuffer2, tBuffer3, tBuffer4, tBuffer1B, tBuffer2B, tBuffer3B, tBuffer4B;
    list<bool> tBeat;
    void bargraph(list<float> vl, float sca, int px, int py, int w, int h);
    bool isBeat;
    float bcol;
    vector<float> tBuffer3BB, tBuffer4BB;
    
    ofxUICanvas *gui;
    ofxUIBiLabelSlider *vslide;
    ofxUILabel *vlabel;
    float *val;
    float m;
    float w;
    float de;
    float fpsTresh;
//    list<float> tBuffer1B, tBuffer2B, tBuffer3B, tBuffer4B;
    void bargraphB(list<float> vlB, float sca, int px, int py, int w, int h);
 //   ProcessFFT fft2;

 };
