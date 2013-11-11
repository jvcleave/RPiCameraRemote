//
//  RPiOSCBridge.cpp
//  RPiCameraRemote
//
//  Created by jason van cleave on 11/11/13.
//
//

#include "RPiOSCBridge.h"


ofParameterGroup* createParameterGroup(ofXml& xmlParser)
{
    ofParameterGroup* parameterGroup = new ofParameterGroup();
    string elementName = xmlParser.getName();
    parameterGroup->setName(elementName);
    int numElementChildren = xmlParser.getNumChildren();
    for(int j=0; j<numElementChildren; j++)
    {
        
        xmlParser.setToChild(j);
        string nodeType = xmlParser.getAttribute("type");
        string childName = xmlParser.getName();
        if (nodeType == "bool")
        {
            ofParameter<bool> boolItem;
            boolItem.set(childName, xmlParser.getBoolValue());
             parameterGroup->add(boolItem);
        }
        if (nodeType == "int")
        {
            ofParameter<int> intItem;
            string min = xmlParser.getAttribute("min");
            string max = xmlParser.getAttribute("max");
            intItem.set(childName, xmlParser.getIntValue(), ofToInt(min), ofToInt(max));
            parameterGroup->add(intItem);
        }
        
        if (nodeType == "float")
        {
            ofParameter<float> floatItem;
            string min = xmlParser.getAttribute("min");
            string max = xmlParser.getAttribute("max");
            floatItem.set(childName, xmlParser.getFloatValue(), ofToFloat(min), ofToFloat(max));
             parameterGroup->add(floatItem);
        }
        xmlParser.setToParent();
        
    }
    return parameterGroup;
}

RPiOSCBridge::RPiOSCBridge()
{
    localPort = 6667;
    remotePort = 6666;
    serializer = NULL;
    sync = NULL;
}

void RPiOSCBridge::setup(string xmlPath)
{
    string xml;
    if (ofIsStringInString(xmlPath, "http"))
    {
        ofHttpResponse response = ofLoadURL(xmlPath);
        xml= response.data.getText();
    }else
    {
        //untested
        ofBuffer buffer = ofBufferFromFile(xmlPath);
        xml = buffer.getText();
        
    }
    ofXml xmlParser;
    xmlParser.loadFromBuffer(xml);
    
    mainGroup.setName(xmlParser.getName());
    
    int numRootChildren =  xmlParser.getNumChildren();
    for(int i=0; i<numRootChildren; i++)
    {
        xmlParser.setToChild(i);
        string nodeType = xmlParser.getAttribute("type");
        string childName = xmlParser.getName();
        if (nodeType == "group")
        {
            ofParameterGroup* parameterGroup = createParameterGroup(xmlParser);
            mainGroup.add(*parameterGroup);
        }else
        {
            string nodeType = xmlParser.getAttribute("type");
            if (nodeType == "bool")
            {
                ofParameter<bool> boolItem;
                boolItem.set(childName, xmlParser.getBoolValue());
                mainGroup.add(boolItem);
            }
            if (nodeType == "int")
            {
                ofParameter<int> intItem;
                string min = xmlParser.getAttribute("min");
                string max = xmlParser.getAttribute("max");
                intItem.set(childName, xmlParser.getIntValue(), ofToInt(min), ofToInt(max));
                mainGroup.add(intItem);
            }
            
            if (nodeType == "float")
            {
                ofParameter<float> floatItem;
                string min = xmlParser.getAttribute("min");
                string max = xmlParser.getAttribute("max");
                floatItem.set(childName, xmlParser.getFloatValue(), ofToFloat(min), ofToFloat(max));
                mainGroup.add(floatItem);
            }
            
        }
        
        xmlParser.setToParent();
    }
    gui.setup("panel");
    
    gui.add(mainGroup);
    
    string filename = ofToDataPath("myoutput.xml", true);
    serializer = new ofXml();
    serializer->serialize(mainGroup);
    //serializer->save(filename);
    sync = new ofxOscParameterSync();
    
    sync->setup((ofParameterGroup&)gui.getParameter(), localPort, "jvcrpi.local", remotePort);
    
}

void RPiOSCBridge::update()
{
    sync->update();
}