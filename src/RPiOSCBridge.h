#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscParameterSync.h"

class RPiOSCBridge
{
public:
    RPiOSCBridge();
    void setup(string xmlPath);
    void update();
    
    ofXml* serializer;
    ofParameterGroup mainGroup;
    ofxOscParameterSync* sync;
    
    int localPort;
    int remotePort;
    
    ofxPanel gui;

};