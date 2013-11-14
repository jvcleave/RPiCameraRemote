#pragma once

#include "ofMain.h"
#include "ofxOscParameterSync.h"
#include "ofxGui.h"

class TestBridge
{
public:
    
    TestBridge(){};
    ofxOscParameterSync* sync;
    
    ofParameterGroup parameters;
    
    ofxPanel gui;
    ofParameterGroup* guiParamGroup;
    
    void setup();
    void update();
    
};
