//
//  Created by Ben Snell and Will Miao on October 20, 2015
//
//  Self-Selective Portraiture: Component 1
//      Display text to screen (textGen).
//      While person is reading it, record eye path with EyeTribe
//          and brainwaves with the Muse Headband (recordData)
//      Export raw eye data, raw brain data, and meta text to file (recordData).
//
//

#pragma once

#include "ofMain.h"
#include "ofxEyeTribe.h"
#include "ofxCsv.h"
#include "ofxOsc.h"
#include "recordData.h"
#include "textGen.h"
#include "textBlock.h"
#include "dataPt.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // recorder object handles recording
    recordData myRec;
    
    // object to display text
    textGen myGen;
    Boolean textBool = true;
    
    Boolean recordTextBool = false;
    Boolean prevRecordTextBool = false;
    
};

