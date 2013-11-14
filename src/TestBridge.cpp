//
//  TestBridge.cpp
//  RPiCameraRemote
//
//  Created by jason van cleave on 11/14/13.
//
//

#include "TestBridge.h"


void TestBridge::setup()
{
    parameters.setName("parameters");
	ofParameter<float> size;
	size.set("size",10,0,100);
	parameters.add(size);
	gui.setup(parameters);
	guiParamGroup = (ofParameterGroup*)&gui.getParameter();
    
	sync = new ofxOscParameterSync();
    
    
	sync->setup(*guiParamGroup,7777,"jvcrpi.local",6666);
    
	ofSetVerticalSync(true);
}

void TestBridge::update()
{
    sync->update();
}