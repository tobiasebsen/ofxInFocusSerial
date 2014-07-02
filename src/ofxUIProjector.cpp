//
//  projectorControl.cpp
//  greveGulv
//
//  Created by Tobias Ebsen on 14/05/14.
//
//

#include "ofxUIProjector.h"

ofxUIProjector::ofxUIProjector() : ofxUISuperCanvas("PROJECTOR", 20, 20, 200, 200) {
    setup();
}

ofxUIProjector::ofxUIProjector(string _label, float x, float y) : ofxUISuperCanvas(_label, x, y, 200, 200) {
    setup();
}
    
void ofxUIProjector::setup() {

    this->addSpacer();
    
    ports = this->addDropDownList("Port", serial.getPortNames());
    ports->setShowCurrentSelected(true);
    ports->setAutoClose(true);
    
    this->addLabel("Status", "Status: unknown")->setFont( this->getFontSmall() );
    
    this->addToggle("Power", false);
    this->addToggle("Eco", false);
    
    this->autoSizeToFitWidgets();
    
    ofAddListener(this->newGUIEvent, this, &ofxUIProjector::uiEvent);
}

void ofxUIProjector::threadedFunction() {

    while (isThreadRunning()) {

        if (serial.isInitialized()) {

            switch (serial.getSystemState()) {
                case INFOCUS_SYS_STANDBY: ((ofxUILabel*)this->getWidget("Status"))->setLabel("Status: standby");break;
                case INFOCUS_SYS_STARTUP: ((ofxUILabel*)this->getWidget("Status"))->setLabel("Status: started");break;
                case INFOCUS_SYS_COOLING: ((ofxUILabel*)this->getWidget("Status"))->setLabel("Status: cooling");break;
                case INFOCUS_SYS_ERROR: ((ofxUILabel*)this->getWidget("Status"))->setLabel("Status: error");break;
            }

            ((ofxUIToggle*)this->getWidget("Power"))->setValue( serial.isPowerOn() );
            ((ofxUIToggle*)this->getWidget("Eco"))->setValue( serial.isLampEco() );
        }
        ofSleepMillis(500);
    }
}

void ofxUIProjector::uiEvent(ofxUIEventArgs & args) {

    if (args.getName() == "Port") {

        vector<string> names = ports->getSelectedNames();
        if (names.size() > 0) {
            serial.setup(names[0]);
        }
    }
    
    if (args.getName() == "Power") {
        serial.setPowerOn( args.getToggle()->getValue() );
    }
    if (args.getName() == "Eco") {
        serial.setLampEco( args.getToggle()->getValue() );
    }
}