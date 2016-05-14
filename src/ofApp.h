#pragma once

#include "ofMain.h"

#include "ofxOSC.h"
#include "ofxSyphon.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"

#include "Config.h"
#include "BrushPass.h"

#include "DispPass.h"

class ofApp : public ofBaseApp{

public:
    void setup();
	void setupGui();
    void update();
    void draw();
	void exit();
	void drawGui(ofEventArgs & args);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
    //---------------------------------------------
    stringstream ss;
	ofxOscReceiver receiver;
    ofxOscSender sender;
	
	// render pass
    BrushPass brushPass;
	DispPass dispPass;
	
	// gui
	ofxDatGui* gui;
	
    ofImage srcImg, saveImg;
    ofPixels pixels;
    
    string suffix;
    
    // parmas
    int frame = 0;
    bool isUpdate = false;
    bool isError = false;
    bool isSave = false;
	

    
};
