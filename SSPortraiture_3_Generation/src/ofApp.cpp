#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // load "meta" text from file (words and locations)
    myGen.loadMetaTextFromFile("METATEXT ben_test 2015-10-20 22-49-42.csv");
    
    // load REVISED eye and brain data (from analysis program)
    myGen.loadData("SELECT EYE will_Water2.csv", "SELECT BRAIN will_Water2.csv");
    
    // generate probability tree
//    myGen.generateProbTreeA();
    myGen.generateProbTreeB();
    
    // export tree and vocab
//    myGen.exportProbTree("will_water");
    
    string myMarkov = myGen.generateMarkovSentencesWeighted(50);
    
//    cout << myMarkov << endl;
    // export markov to file
    myGen.exportMarkov(myMarkov, "my_markov");
    
    ofExit();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
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
