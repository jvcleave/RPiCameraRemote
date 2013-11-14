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
    localPort = 7777;
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
                intItems.push_back(&intItem);
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
    gui.setup("camera");
    
    gui.add(mainGroup);
    
    string filename = ofToDataPath("gui.xml", true);
    serializer = new ofXml();
    
    sync = new ofxOscParameterSync();
    guiParamGroup = (ofParameterGroup*)&gui.getParameter();
    
   
    ofParameterGroup * parent = guiParamGroup->getGroup("root").get("contrast").getParent();
    
    guiParamGroup->getGroup("root").get("contrast").setParent(guiParamGroup);
    ofLogVerbose() << "guiParamGroup-> name: " << guiParamGroup->getName();
    if (parent)
    {
        ofLogVerbose() << "myContrast parent is: " << parent->getName();
        //myContrast.setParent(guiParamGroup);
    }else{
        ofLogVerbose() << "myContrast has no parent ";
    }
    
    sync->setup(*guiParamGroup, localPort, "jvcrpi.local", remotePort);
    serializer->serialize(*guiParamGroup);
    serializer->save(filename);
    
    
    
}

void RPiOSCBridge::update()
{
    sync->update();
}