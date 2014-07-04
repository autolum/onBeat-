#include "testApp.h"
#define BARK_MAX 25
float bins[BARK_MAX];
float binsold[BARK_MAX];
int bark(float f) {
    float b = 13 * atan(0.00076 * f) + 3.5 * atan(pow(f / 7500.0f, 2));
    return ((int)floor(b));
}
float * fftSmoothed;
float fftmax;
int barkmap[512];


void testApp::setup() {
	ofSetVerticalSync(true);
	
    gui = new ofxUICanvas(0,(ofGetHeight()-220),(ofGetWidth()-10),60);
    vslide = gui->addBiLabelSlider("fpsTresh", "fpsTreshold", "1", 1.0, 1000, &fpsTresh);
    vslide->setColorBack(ofColor(50,0,0,20));
    vslide->setColorFill(ofColor(52,175,195,100));
    vslide->setColorFillHighlight(ofColor(52,175,195,100));
    vlabel = vslide->getRightLabel();
    gui = new ofxUICanvas(0,(ofGetHeight()-200),(ofGetWidth()-10),60);
    vslide = gui->addBiLabelSlider("w", "w", "1", 1.0, 50, &w);
    vslide->setColorBack(ofColor(50,0,0,20));
    vslide->setColorFill(ofColor(52,175,195,100));
    vslide->setColorFillHighlight(ofColor(52,175,195,100));
    vlabel = vslide->getRightLabel();
    gui = new ofxUICanvas(0,(ofGetHeight()-180),(ofGetWidth()-10),60);
    vslide = gui->addBiLabelSlider("m", "m", "1", 1.0, 50, &m);
    vslide->setColorBack(ofColor(50,0,0,20));
    vslide->setColorFill(ofColor(52,175,195,100));
    vslide->setColorFillHighlight(ofColor(52,175,195,100));
    vlabel = vslide->getRightLabel();
    gui = new ofxUICanvas(0,(ofGetHeight()-160),(ofGetWidth()-10),60);
    vslide = gui->addBiLabelSlider("de", "de", "1", 0.01, 1, &de);
    vslide->setColorBack(ofColor(50,0,0,20));
    vslide->setColorFill(ofColor(52,175,195,100));
    vslide->setColorFillHighlight(ofColor(52,175,195,100));
    vlabel = vslide->getRightLabel();
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    
	plotHeight = 128;
    tBl = 256;
	bufferSize = 512;

	//fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
	// To use FFTW, try:
	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HANN, OF_FFT_FFTW);
 
    drawBins.resize(fft->getBinSize());
    middleBins.resize(fft->getBinSize());
    audioBins.resize(fft->getBinSize());
    tBuffer1.resize(tBl,0.0);
    tBuffer2.resize(tBl,0.0);
    tBuffer3.resize(tBl,0.0);
    tBuffer4.resize(tBl,0.0);
    tBeat.resize(tBl,false);
    
    tBuffer3BB.resize(tBl,0.0);
    tBuffer4BB.resize(tBl,0.0);
	
	// 0 output channels,
	// 1 input channel
	// 44100 samples per second
	// [bins] samples per buffer
	// 4 num buffers (latency)
	
	ofSoundStreamSetup(0, 1, this, 44100, bufferSize, 10);
	
	ofBackground(0, 0, 0);
    
    fftSmoothed = new float[8192];
    memset(fftSmoothed, 0x00, sizeof(float) * 8192 );
    float freq_spc = 44100 / (float)bufferSize;

	for (int i = 0; i < bufferSize; i++) {
        int bidx = bark(i * freq_spc);
        barkmap[i] = bidx;
    }
    
    tBuffer1B.resize(tBl,0.0);
    tBuffer2B.resize(tBl,0.0);
    tBuffer3B.resize(tBl,0.0);
    tBuffer4B.resize(tBl,0.0);
}

void testApp::draw() {
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(16, 16);
	
	ofDrawBitmapString("Frequency Domain", 0, 0);
	plot(drawBins, -plotHeight, plotHeight / 2);
	ofPopMatrix();
	string msg = ofToString((int) ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
    ofSetColor(255, 255, 255);
   // bargraph(tBuffer1,10.0, 32, 512, 800, 128);
    bargraph(tBuffer2,50.0, 32, 200, 800, 128);
    ofSetColor(255, 255, 0);
    bargraph(tBuffer3,50.0, 32, 200, 800, 128);
    ofSetColor(255, 0, 0);
    bargraph(tBuffer4,50.0, 32, 200, 800, 128);
    
    ofSetColor(255, 255, 255);
    bargraphB(tBuffer2B,50.0, 32, 400, 800, 128);
    ofSetColor(255, 255, 0);
    bargraphB(tBuffer3B,50.0, 32, 400, 800, 128);
    ofSetColor(255, 0, 0);
    bargraphB(tBuffer4B,50.0, 32, 400, 800, 128);
    
    if(isBeat) bcol=255;
    ofSetColor(bcol);
    ofFill();
    ofRect(32, 650, 50, 50);
    bcol = 0;
}

void testApp::update(){
    
   

 /*    float nrg = 0.0;
    float sdiff = 0.0;
    float diff = 0.0;
   // cout<<sizeof(bins)<<cout;

    for (int i = 0; i < middleBins.size()/45; i++){
        nrg += middleBins[i];
        diff = drawBins[i]-middleBins[i];
        sdiff += (diff + abs(diff))*0.5;
        //sdiff +=
    }

    float summ = 0.0;
    int i = 1;
    list<float>::iterator it;
    it   = tBuffer2.begin();
    for (int i = 0; i < fpsTresh; i++){
        summ += *it;
        it++;
    }
    summ/=fpsTresh;
    
  
   //  cout<<nrg<<"..."<<diff<<cout;
    
    //cout << ' ' << nrg;
    tBuffer1.push_front(nrg);
    tBuffer1.pop_back();
    tBuffer2.push_front(sdiff);
    tBuffer2.pop_back();
    tBuffer3.push_front(summ*2);
    tBuffer3.pop_back();
    float sd =sdiff-(summ*2);
    tBuffer4.push_front(sd+abs(sd));
    tBuffer4.pop_back();
    
    soundMutex.lock();
	drawBins = middleBins;
	soundMutex.unlock();
   */
    val = fft->getAmplitude();
	for (int i = 0; i < tBl/50; i++){
		
		// let the smoothed calue sink to zero:
		fftSmoothed[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i])
            fftSmoothed[i] = val[i];
        
        if (fftSmoothed[i] > fftmax) {
            fftmax = fftSmoothed[i];
        }
	}
    /* update our bark map'd frequency bins */
    memset(bins, 0x00, sizeof(bins));
    for (int i = 0; i < bufferSize; i++) {
        int idx = barkmap[i];
        bins[idx] += fftSmoothed[i] * 20;
        
    }
    
    
    float nrgB = 0.0;
    float sdiffB = 0.0;
    float diffB = 0.0;
    
    
    for (int i = 0; i < 1; i++){
        nrgB += bins[i];
        diffB = binsold[i]-bins[i];
        sdiffB += (diffB + abs(diffB))*1;
        
    }
    
     float summB = 0.0;
    int iB = 1;
    list<float>::iterator itB;
    itB   = tBuffer2B.begin();
    for (int i = 0; i < fpsTresh; i++){
        summB += *itB;
        itB++;
    }
    summB/=fpsTresh;
  
    tBuffer1B.push_front(nrgB);
    tBuffer1B.pop_back();
    tBuffer2B.push_front(sdiffB);
    tBuffer2B.pop_back();
    tBuffer3B.push_front(summB*1);
    tBuffer3B.pop_back();
    float sdB =sdiffB-(summB*1);
    tBuffer4B.push_front(sdB+abs(sdB));
    tBuffer4B.pop_back();
    
    for (int i = 0; i < BARK_MAX; i++){
    binsold[i] = bins[i];
    }
    
    
     float summBB = 0.0;
     list<float>::iterator itBB;
     for (itBB = tBuffer2.begin(); itBB != tBuffer2.end(); ++itBB){
     summBB += *itBB;
     }
     
     summBB/=tBl;
     itBB = tBuffer2B.begin();
     float sq = 0.0;
     for (int i = 0; i < tBl; i++){
     tBuffer3BB[i] = *itBB-summBB;
     itBB++;
     sq += tBuffer3BB[i]*tBuffer3BB[i];
     }
     
     //Varianz auf eins
     for (int i = 0; i < tBl; i++){
     tBuffer3BB[i] /=sqrt(sq);
     }
     
     //int w = 0;
     //int m = 1;
    // float delta = 0.09;
     isBeat = true;
     summBB = 0.0;
     for (int i = -w; i < w*m; i++){
     if (tBuffer3BB[w]<tBuffer3BB[w+i]) isBeat = false;
     summBB += tBuffer3BB[w+i];
     }
     summBB /= (m*w+w+1);
     if(tBuffer3BB[w] < summBB+de) isBeat = false;
   // cout<<"--fpsTresh"<<fpsTresh<<"--w"<<w<<"--m"<<m<<"--delta"<<de<cout;
}

void testApp::plot(vector<float>& buffer, float scale, float offset) {
	ofNoFill();
	int n = buffer.size();
	ofRect(0, 0, n, plotHeight);
	glPushMatrix();
	glTranslatef(0, plotHeight / 2 + offset, 0);
	ofBeginShape();
	for (int i = 0; i < n; i++) {
		ofVertex(i, sqrt(buffer[i]) * scale);
        //ofLine(i,0,i,sqrt(buffer[i]) * scale);
	}
	ofEndShape();
	glPopMatrix();
}

void testApp::bargraph(list<float> vl, float sca, int px, int py, int w, int h) {

    ofNoFill();
    ofRect(px, py, w, h);
    
    int n = vl.size();
    float sum = 0.0;

    int rw = w/(n);
    //int rw = 2;
    //ofSetRectMode(OF_RECTMODE_CENTER);

    ofBeginShape();
    int i = 0;
    for (list<float>::iterator it  = vl.begin(); it != vl.end(); ++it){
        //if(*iit) ofSetColor(255,0,0,100);
        //else ofSetColor(255,100);
        //ofRect(i*rw+px, py + h/2, 1, *it*10*sca);
        ofVertex(i*rw+px, py+h-*it*sca);
        i++;
    }
    ofEndShape();
}

void testApp::bargraphB(list<float> vlB, float sca, int px, int py, int w, int h) {
    
    ofNoFill();
    ofRect(px, py, w, h);
    
    int n = vlB.size();
    float sum = 0.0;
    
    int rw = w/(n);
    //int rw = 2;
    //ofSetRectMode(OF_RECTMODE_CENTER);
    
    ofBeginShape();
    int i = 0;
    for (list<float>::iterator itB  = vlB.begin(); itB != vlB.end(); ++itB){
        //if(*iit) ofSetColor(255,0,0,100);
        //else ofSetColor(255,100);
        //ofRect(i*rw+px, py + h/2, 1, *it*10*sca);
        ofVertex(i*rw+px, py+h-*itB*sca);
        i++;
    }
    ofEndShape();
}

void testApp::audioReceived(float* input, int bufferSize, int nChannels) {	
	float maxValue = 0;
    
    /*
	for(int i = 0; i < bufferSize; i++) {
		if(abs(input[i]) > maxValue) {
			maxValue = abs(input[i]);
		}
	}
	for(int i = 0; i < bufferSize; i++) {
		input[i] /= maxValue;
	}
    */
    
	fft->setSignal(input);
    
//	float* curFft = fft->getAmplitude();
//	memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());
	
    /*
	maxValue = 0;
	for(int i = 0; i < fft->getBinSize(); i++) {
		if(abs(audioBins[i]) > maxValue) {
			maxValue = abs(audioBins[i]);
		}
	}

	for(int i = 0; i < fft->getBinSize(); i++) {
		audioBins[i] /= maxValue;
	}
    */
	
	soundMutex.lock();
	middleBins = audioBins;
	soundMutex.unlock();
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
     if(name == "fpsTresh")
    {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		fpsTresh = slider->getScaledValue();
        char val[5];
        sprintf(val,"%d",(int)fpsTresh);
        vlabel->setLabel(val);
	}
    else if(name == "m")
    {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		m = slider->getScaledValue();
        char val[5];
        sprintf(val,"%d",(int)m);
        vlabel->setLabel(val);
	}
    else if(name == "w")
    {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		w = slider->getScaledValue();
        char val[5];
        sprintf(val,"%d",(int)w);
        vlabel->setLabel(val);
	}
    else if(name == "de")
    {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		de = slider->getScaledValue();
        char val[5];
        sprintf(val,"%d",(int)de);
        vlabel->setLabel(val);
	}
}

