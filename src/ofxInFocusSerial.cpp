//
//  ofxInFocusSerial.cpp
//  greveGulv
//
//  Created by Tobias Ebsen on 13/05/14.
//
//

#include "ofxInFocusSerial.h"

bool ofxInFocusSerial::setup(string portName) {
    
    bool result = ofSerial::setup(portName, 19200);

    // Disable hardware flow control
    struct termios options;
	tcgetattr(fd, &options);
    options.c_cflag &= ~CRTSCTS;
    tcsetattr(fd, TCSANOW, &options);

    return result;
}

vector<string> ofxInFocusSerial::getPortNames() {
    vector<string> names;
    vector<ofSerialDeviceInfo> list = ofSerial::getDeviceList();
    vector<ofSerialDeviceInfo>::iterator it = list.begin();
    for (; it!=list.end(); ++it) {
        names.push_back(it->getDeviceName());
    }
    return names;
}

bool ofxInFocusSerial::isPowerOn() {
    commandRead("PWR");
    return getResponseInt() == 1;
}

void ofxInFocusSerial::setPowerOn(bool on) {
    commandWrite("PWR", on ? 1 : 0);
}

int ofxInFocusSerial::getLampHoursNormal() {
    commandRead("LMO");
    return getResponseInt();
}

int ofxInFocusSerial::getLampHoursEco() {
    commandRead("LME");
    return getResponseInt();
}

bool ofxInFocusSerial::isLampEco() {
    commandRead("IPM");
    return getResponseInt() == 1;
}

void ofxInFocusSerial::setLampEco(bool eco) {
    commandWrite("IPM", eco ? 1 : 0);
}

string ofxInFocusSerial::getFirmwareVersion() {
    commandRead("FVS");
    return getResponseString();
}

int ofxInFocusSerial::getSystemState() {
    commandRead("SYS");
    return getResponseInt();
}

void ofxInFocusSerial::write(string str) {
    ofSerial::writeBytes((unsigned char*)str.c_str(), str.length());
}

string ofxInFocusSerial::read() {
    char str[64];
    int n = ofSerial::readBytes((unsigned char*)str, sizeof(str));
    if (n > 0)
        str[n] = 0;
    else
        str[0] = 0;
    return str;
}

void ofxInFocusSerial::commandWrite(string cmd, int value) {
    write("(" + cmd + ofToString(value) + ")");
    ofSerial::drain();
}

string ofxInFocusSerial::commandRead(string cmd) {
    write("(" + cmd + "?)");
    ofSerial::drain();
    unsigned long long before = ofGetElapsedTimeMillis();
    response = read();
    while (response.find(")") == -1 && ofGetElapsedTimeMillis() - before < 2000) {
        ofSleepMillis(1);
        response.append(read());
    };
    return response;
}

int ofxInFocusSerial::getResponseInt() {
    int b = response.find(",");
    int e = response.find(")");
    if (b != -1 && e != -1) {
        string str = response.substr(b+1,e-b-1);
        return  ofToInt(str);
    }
    return -1;
}

string ofxInFocusSerial::getResponseString() {
    int b = response.find("\"");
    int e = response.find_last_of('\"');
    if (b != -1 && e != -1)
        return response.substr(b+1, e-b-1);
    else
        return "";
}