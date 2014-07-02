//
//  projectorControl.h
//  greveGulv
//
//  Created by Tobias Ebsen on 14/05/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxInFocusSerial.h"
#include "ofxUI.h"

class ofxUIProjector : public ofxUISuperCanvas, public ofThread {
public:
    ofxUIProjector();
    ofxUIProjector(string _label, float x, float y);
    
    void setup();

    void threadedFunction();
    
    void uiEvent(ofxUIEventArgs & args);

private:
    ofxInFocusSerial serial;
    ofxUIDropDownList *ports;
};