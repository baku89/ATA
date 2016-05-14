//
//  BrushPass.hpp
//  of
//
//  Created by Baku Hashimoto on 3/12/16.
//
//

#pragma once

#include <math.h>
#include "ofMain.h"
#include "ofxSwapBuffer.h"

#include "Config.h"



#define STRINGIFY(A) #A

#define MIN_FLOW 0.3
#define SPEED_AMP 0.05


class BrushPass {
public:
    
	void allocate(int w, int h);
	void reload();
	void setSize(int w, int h);
	void update();
    
	ofTexture& getTexture();
    
    float speed = 0.0;
    float opacity = 0.0;
    float threshold = 0.0;
    float offset = 0.0;
	
	void draw();
	
    ofShader shader;
    ofxSwapBuffer pingPong;
    int width, height;
	
	ofTexture *coat;
	
    
};


