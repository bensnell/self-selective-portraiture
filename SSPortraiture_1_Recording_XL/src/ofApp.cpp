#include "ofApp.h"

// remember to press R when finished!!!!

// EYE STARTS RECORDING AT STARTUP

//--------------------------------------------------------------
void ofApp::setup(){
    
    myRec.setupDevices(true, true);
    
    // load and set text sample
    myGen.loadText(ofToDataPath("thisiswater"));
    myGen.setText(20, 200, 200, ofColor::black, 2);
    //    myGen.recordText("wase");
    //    ofExit();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Write the EyeTribe data to file ('e' to record eye data)
    myRec.recordEyeData("will_Ivan");
    
    // Write the Muse Headband data to file ('b' to record brain data)
    myRec.recordBrainData("will_Ivan");
    
    // record path to draw without writing to file ('k')
    myRec.recordEyePath();
    
    // record meta text
    if (recordTextBool && !prevRecordTextBool) myGen.recordText("ben_test");
    prevRecordTextBool = recordTextBool;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255, 255, 255);
    ofDisableAntiAliasing();
    
    // draw text sample to screen
    if (textBool) myGen.drawText(myRec.scrollDistance);
    
    // draw live gaze data ('g' to toggle)
    myRec.drawLiveGaze();
    
    // draw eye path ('l' to draw, 'k' to record new path, 'j' for drawing type)
    myRec.drawEyePath();
    
    // write debug messages to screen
    myRec.debug();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'f') {
        ofToggleFullscreen();
        myGen.refreshText(); // refresh text alignment
    }
    if (key == 'o') myRec.eTribe.open();
    if (key == 'c') myRec.eTribe.close();
    if (key == 's') myRec.eTribe.startServer();
    if (key == 'e' || key == 'r') { // 'e' toggles eye recording -- writes to new file ea. time
        if (!myRec.eRecordingState) {
            myRec.eRecordingState = true;
            myRec.eyePath.clear();
            myRec.recordEyePathBool = false;
            myRec.drawEyePathBool = false;
            
            recordTextBool = true;
            
            stringstream eStreamTitle;
            eStreamTitle << ofGetYear() << "-";
            eStreamTitle <<  setw(2) << setfill('0') << ofGetMonth() << "-";
            eStreamTitle <<  setw(2) << setfill('0') << ofGetDay() << " ";
            eStreamTitle <<  setw(2) << setfill('0') << ofGetHours() << "-";
            eStreamTitle <<  setw(2) << setfill('0') << ofGetMinutes() << "-";
            eStreamTitle <<  setw(2) << setfill('0') << ofGetSeconds();
            myRec.efileTitle = "RAW_EYE " + myRec.eRecordingTitle + " " + eStreamTitle.str() + ".csv";
            myRec.eCsv.createFile(ofToDataPath(myRec.efileTitle));
        } else {
            myRec.eRecordingState = false;
            //            drawEyePathBool = false;
            myRec.eCsv.saveFile(ofToDataPath(myRec.efileTitle));
            myRec.eCounter = 0;
            
            recordTextBool = false;
        }
    }
    if (key == 'b' || key == 'r') {
        if (!myRec.bRecordingState) { // start recording brain waves
            myRec.bRecordingState = true;
            
            recordTextBool = true;
            
            stringstream bStreamTitle;
            bStreamTitle << ofGetYear() << "-";
            bStreamTitle <<  setw(2) << setfill('0') << ofGetMonth() << "-";
            bStreamTitle <<  setw(2) << setfill('0') << ofGetDay() << " ";
            bStreamTitle <<  setw(2) << setfill('0') << ofGetHours() << "-";
            bStreamTitle <<  setw(2) << setfill('0') << ofGetMinutes() << "-";
            bStreamTitle <<  setw(2) << setfill('0') << ofGetSeconds();
            myRec.bfileTitle = "RAW_BRAIN " + myRec.bRecordingTitle + " " + bStreamTitle.str() + ".csv";
            myRec.bCsv.createFile(ofToDataPath(myRec.bfileTitle));
        } else { // stop recording brain waves
            myRec.bRecordingState = false;
            myRec.bCsv.saveFile(ofToDataPath(myRec.bfileTitle));
            
            myRec.bCounter = 0;
            
            recordTextBool = false;
        }
    }
    if (key == 'l') { // toggle drawing the most recently recorded eye path
        if (!myRec.drawEyePathBool) {
            myRec.drawEyePathBool = true;
        } else {
            myRec.drawEyePathBool = false;
        }
    }
    if (key == 'k') { // manually record (just the) eye path -- not to myRec.eCsv
        if (myRec.eRecordingState == false && myRec.recordEyePathBool == false) {
            myRec.eyePath.clear();
            myRec.recordEyePathBool = true;
            myRec.drawEyePathBool = false;
        } else {
            myRec.recordEyePathBool = false;
        }
    }
    if (key == 'j' ) { // cycle through the different ways the eye path can be drawn
        if (myRec.drawEyePathBool) {
            myRec.drawEyePathType = (myRec.drawEyePathType + 1) % 3;
        }
    }
    if (key == 'g') {
        myRec.drawLiveGazeBool = !myRec.drawLiveGazeBool;
    }
    if (key == 'd') {
        myRec.debugBool = !myRec.debugBool;
    }
    if (key == 't') {
        textBool = !textBool;
    }
    //    if (key == 'r') {
    //        if (!myRec.cRecordingState) {
    ////            myRec.eRecordingState = false;
    ////            myRec.bRecordingState = false;
    //            myRec.cRecordingState = true;
    ////            myRec.eRecordingState = true;
    ////            myRec.bRecordingState = true;
    //
    //        } else {
    ////            myRec.eRecordingState = false;
    ////            myRec.bRecordingState = false;
    //            myRec.cRecordingState = false;
    //
    //
    //        }
    //    }
    
    if (key == OF_KEY_UP) {
        myRec.scrollDistance += 10;
    }
    if (key == OF_KEY_DOWN) {
        myRec.scrollDistance -= 10;
    }
    myRec.scrollDistance = MAX(myRec.scrollDistance, 0);

    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}