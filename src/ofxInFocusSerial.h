//
//  ofxInFocusSerial.h
//  greveGulv
//
//  Created by Tobias Ebsen on 13/05/14.
//
//

#pragma once

#include "ofMain.h"

#define INFOCUS_SOURCE_VGA1     0
#define INFOCUS_SOURCE_VGA2     1
#define INFOCUS_SOURCE_HDMI     5
#define INFOCUS_SOURCE_COMP     12
#define INFOCUS_SOURCE_SVIDEO   13

#define INFOCUS_SYS_STANDBY     0
#define INFOCUS_SYS_STARTUP     2
#define INFOCUS_SYS_COOLING     12
#define INFOCUS_SYS_ERROR       18

class ofxInFocusSerial {
public:
    
    bool setup(string portName);
    bool isInitialized();

    vector<string> getPortNames();
    
    int getSource();
    
    int getLampHoursNormal();
    int getLampHoursEco();
    
    bool isLampEco();
    void setLampEco(bool eco);
    
    bool isPowerOn();
    void setPowerOn(bool on);
    
    string getFirmwareVersion();
    
    int getSystemState();

protected:

    void write(string str);
    string read();
    
    void commandWrite(string cmd, int value);
    string commandRead(string cmd);
    int getResponseInt();
    string getResponseString();

private:
    ofSerial serial;
    string response;
};