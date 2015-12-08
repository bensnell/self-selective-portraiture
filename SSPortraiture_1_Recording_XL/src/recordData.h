//
//  recordData.h
//  SSPortraiture_1_Recording_XL
//
//  Created by Ben Snell on 10/20/15.
//
//

#ifndef __SSPortraiture_1_Recording_XL__recordData__
#define __SSPortraiture_1_Recording_XL__recordData__

#include <ofMain.h>
#include "ofxEyeTribe.h"
#include "ofxCsv.h"
#include "ofxOsc.h"

class recordData {
    
public:
    
    // --------------------------------------------------
    // ----------------- SETUP DEVICES ------------------
    // --------------------------------------------------
    
    ofxEyeTribe eTribe;
    int eyePort = 6555;
    
    ofxOscReceiver bReceiver;
    int brainPort = 5000;
    
    // open connections to these devices
    void setupDevices(Boolean eyeSetup, Boolean brainSetup);
    
    // --------------------------------------------------
    // ----------------- EYE RECORDING ------------------
    // --------------------------------------------------
    
    wng::ofxCsv eCsv;
    
    float eTimeZero;
    int eCounter = 0;
    ofPoint origin = ofVec3f(0.0, 0.0);
    
    void recordEyeData(string recording_title);
    
    string eRecordingTitle = "default_title"; // eye only recording title
    Boolean eRecordingState = false;
    string efileTitle;
    
    // --------------------------------------------------
    // ---------------- EYE PATH ANALYSIS ---------------
    // --------------------------------------------------
    
    void drawLiveGaze();
    Boolean drawLiveGazeBool = true;
    
    ofPath eyePath;
    void recordEyePath();
    Boolean recordEyePathBool = false; // toggle with 'k' for Klear
    
    void drawEyePath();
    Boolean drawEyePathBool = false; // toggle with 'l' for draw Line
    int drawEyePathType = 0; // cycle with 'j'
    
    // --------------------------------------------------
    // ---------------- BRAIN RECORDING -----------------
    // --------------------------------------------------
    
    wng::ofxCsv bCsv;
    
    ofxOscMessage lastTimeMess;
    double bTimeZero;
    double bTimeBefore;
    long bCounter = 0; // line counter
    
    void recordBrainData(string recording_title);
    
    string bRecordingTitle = "default_title"; // brain only recording title
    Boolean bRecordingState = false;
    string bfileTitle;
    
    // --------------------------------------------------
    // -------------- COMBINED RECORDING ----------------
    // --------------------------------------------------
    
    Boolean cRecordingState = false; // Combined
    
    // --------------------------------------------------
    // ------------------- UTILITIES --------------------
    // --------------------------------------------------
    
    // write recording info to screen
    void debug();
    Boolean debugBool = true;
    
    // Overall control for scrolling with large files
    // Text (displayed): translated upward (y value)
    // Metatext: unchanged
    // Eye data: translated upward (y value)
    // Brain data: unchanged
    int scrollDistance = 0;
    
};

#endif /* defined(__SSPortraiture_1_Recording_XL__recordData__) */