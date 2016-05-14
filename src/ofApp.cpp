#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetVerticalSync(true);
    ofSetFrameRate(60);
	//	ofSetFullscreen(true);
	
    ss.str("");
    ss << "images/" << NAME << ".png";
    srcImg.load(ss.str());
    
    float w = srcImg.getWidth();
    float h = srcImg.getHeight();
    
    ofSetWindowShape(1280.0, 1280.0 * (h / w));
    
    
//    ofSetWindowShape(width, height);
                     
                     
	brushPass.allocate(w, h);
    pixels.allocate(w, h, GL_RGB);
    
    receiver.setup(PORT);
    sender.setup("127.0.0.1", 4321);
    
    
    
	
    // set
    brushPass.coat = &srcImg.getTexture();

}

//--------------------------------------------------------------
void ofApp::exit() {
}





//--------------------------------------------------------------
void ofApp::setupGui(){
	ofSetBackgroundColor(0);
	
	
	gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
	gui->setAutoDraw(false);
    
    gui->addFRM();
	
    gui->addBreak()->setHeight(40.0f);
    gui->addSlider("speed", 0, 1)->bind(brushPass.speed);
    gui->addSlider("opacity", 0, 1)->bind(brushPass.opacity);
    gui->addSlider("threshold", 0, 1)->bind(brushPass.threshold);
    gui->addSlider("offset", 0, 1)->bind(brushPass.offset);
    
	
	gui->setTheme(new ofxDatGuiThemeMidnight());
}

//--------------------------------------------------------------
void ofApp::update(){
	
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        if (isUpdate) {
            isError = true;
        }
        
        string address = m.getAddress();
        
        if (address == "/update") {
            isUpdate = true;
            frame = m.getArgAsInt(0);
            brushPass.speed = m.getArgAsFloat(1);
            brushPass.opacity = m.getArgAsFloat(2);
            brushPass.threshold = m.getArgAsFloat(3);
            brushPass.offset = m.getArgAsFloat(4);
            suffix = m.getArgAsString(5);
            
            if (frame != -1) {
                isSave = true;
            }
            
        } else if (address == "/update-shader") {
            brushPass.reload();
        }
    }
	
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (isUpdate) {
        brushPass.update();
    }
    
    ofBackground(0);
	
    // 1. update brush
	
	// 2. update disp
    /*
	dispPass.src = &brushPass.getTexture();
	dispPass.update();
     */

	// 3. rendering
    ofSetColor(255);
	ofPushMatrix();
	ofScale((float)ofGetWidth() / srcImg.getWidth(),
			(float)ofGetHeight() / srcImg.getHeight());
//	ofTranslate(0, -srcImg.getHeight());
    
    brushPass.draw();
    
    ofPopMatrix();
    
    ofSetColor(255, 0, 0);
    ss.str("");
    ss << "FRAME:" << frame;
    ofDrawBitmapString(ss.str(), 20, 20);
    
    if (isError) {
        ss.str("");
        ss << "Error occured";
        ofDrawBitmapString(ss.str(), 20, 40);
    }
    
    // save
    if (isSave) {
        brushPass.getTexture().readToPixels(pixels);
        saveImg.setFromPixels(pixels);
        
        ss.str("");
        ss << "export/" << NAME << "_" << suffix << "/" << NAME << "_" << suffix << "_" << setw(4) << setfill('0') << frame << ".png";
        string filename = ss.str();
        ofLogNotice() << "saved at " << filename;
        saveImg.save(ss.str());
        
        ofxOscMessage m;
        m.setAddress("/save-frame");
        m.addIntArg(frame);
        sender.sendMessage(m);
        
        
    }
    
    
    
    
    isUpdate = false;
    isError = false;
    isSave = false;
    
    
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs & args){
	gui->draw();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){}



//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}

