#include "ofApp.h"

void ofApp::setup(){

    mySet.getACData("RAW_BRAIN will_Ivan 2015-10-20 22-49-42.csv", "/muse/elements/gamma_session_score");
//    mySet.getBData("RAW_BRAIN will_Ivan 2015-10-20 22-08-53.csv", "/muse/elements/experimental/concentration");
//    mySet.getBGraph(mySet.BData);
    mySet.getBGraph(mySet.ACData);
    
    mySet.getEData("RAW_EYE will_Ivan 2015-10-20 22-49-42.csv", 30.0);
//    mySet.getEGraph();
//    mySet.getEGraphGradient(ofColor::blue, ofColor::yellow, 10);
    
    // export data -- must specify eye or brain
    mySet.exportData(mySet.ACData, "BRAIN will_Water2");
    mySet.exportData(mySet.EData, "EYE will_Water2");
    
//    myGen.loadText("frank3");
//    myGen.setText(20, 100, 100, ofColor::black, 1.7);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    cout.precision(10);
    
//    myGen.drawText();
    
    mySet.drawBGraphAll();
//    mySet.drawEGraphGradient();
//    mySet.drawEClosestPoint();
    
    // draw framerate
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
        myGen.refreshText();
    }
    
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
