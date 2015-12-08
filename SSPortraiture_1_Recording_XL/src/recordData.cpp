//
//  recordData.cpp
//  SSPortraiture_1_Recording_XL
//
//  Created by Ben Snell on 10/20/15. -- changed eye recording
//
//

#include "recordData.h"

// setup specified devices
void recordData::setupDevices(Boolean eyeSetup, Boolean brainSetup) {
    
    if (eyeSetup) eTribe.open(eyePort); // don't have to specify port
    
    if (brainSetup) bReceiver.setup(brainPort);
}

//--------------------------------------------------------------

// record eye data to file
void recordData::recordEyeData(string recording_title) {
    
    eRecordingTitle = recording_title;
    
    if (eRecordingState) {
        
        // record data to csv
        if (eCounter == 0) { // record time zero as reference in first reading
            eTimeZero = eTribe.getTimestamp();
            eCsv.setFloat(eCounter, 0, (eTribe.getTimestamp() - eTimeZero) / 1000.0);
            eCsv.setFloat(eCounter, 1, eTribe.getPoint2dAvg().x);
            eCsv.setFloat(eCounter, 2, (eTribe.getPoint2dAvg().y + scrollDistance));
            eCounter++;
        } else { // other times, just write to eCsv
            float timeNow = (eTribe.getTimestamp() - eTimeZero) / 1000;
            if (timeNow != eCsv.getFloat(eCounter - 1, 0)) { // remove duplicates
                if (eTribe.getPoint2dAvg() != origin) { // remove pts at origin
                    eCsv.setFloat(eCounter, 0, timeNow);
                    eCsv.setFloat(eCounter, 1, eTribe.getPoint2dAvg().x);
                    eCsv.setFloat(eCounter, 2, (eTribe.getPoint2dAvg().y + scrollDistance));
                    eCounter++;
                }
            }
        }
    }
}

//--------------------------------------------------------------

// Write brain data to bCsv
void recordData::recordBrainData(string recording_title) {
    
    bRecordingTitle = recording_title;
    
    // while recording state is false, read messages to keep track of the most recent time code (since only eeg, acc, and eeg/quantization have timestamps
    if (!bRecordingState) {
        while (bReceiver.hasWaitingMessages()) {
            ofxOscMessage lastMess;
            bReceiver.getNextMessage(&lastMess);
            
            // check for eeg or acc message
            string mStr = lastMess.getAddress();
            size_t foundEEG = mStr.find("eeg");
            size_t foundACC = mStr.find("acc");
            // if it is, store the message to potentially retrieve message later
            if (foundEEG != string::npos || foundACC != string::npos) {
                lastTimeMess = lastMess;
            }
        }
    } else { // otherwise, recording state is true, so record brain waves:
        // check for waiting messages
        while (bReceiver.hasWaitingMessages()) {
            
            // for the first reading, find the current time if it's an eeg or acc reading, otherwise use the last recorded time in lastTimeMess
            
            int sec;
            int usec;
            double bTimeNow;
            
            // get current message and check to see if it has EEG or ACC
            ofxOscMessage mess;
            bReceiver.getNextMessage(&mess);
            string mStr = mess.getAddress();
            size_t foundEEG = mStr.find("eeg");
            size_t foundACC = mStr.find("acc");
            
            // for the first reading, find the timestamp
            if (bCounter == 0) {
                
                // if eeg or acc, find bTimeZero from mess
                if (foundEEG != string::npos || foundACC != string::npos) {
                    int tempNumArgs = mess.getNumArgs();
                    sec = mess.getArgAsInt32(tempNumArgs - 2);
                    usec = mess.getArgAsInt32(tempNumArgs - 1);
                } else { // otherwise, use last recorded time from lastTimeMess
                    int tempNumArgs = lastTimeMess.getNumArgs();
                    sec = lastTimeMess.getArgAsInt32(tempNumArgs - 2);
                    usec = lastTimeMess.getArgAsInt32(tempNumArgs - 1);
                }
                // calculate bTimeZero
                bTimeZero = sec + usec / 1000000.0; // cast to double?
                bTimeNow = (sec + usec / 1000000.0) - bTimeZero;
                
            } else { // for all readings after the first, find the timestamp
                
                // if it's EEG or ACC, use the current time
                if (foundEEG != string::npos || foundACC != string::npos) {
                    int tempNumArgs = mess.getNumArgs();
                    sec = mess.getArgAsInt32(tempNumArgs - 2);
                    usec = mess.getArgAsInt32(tempNumArgs - 1);
                    bTimeNow = (sec + usec / 1000000.0) - bTimeZero;
                } else { // reference the last time (bTimeBefore)
                    bTimeNow = bTimeBefore;
                }
            }
            
            // write the data to file, but only if it doesn't contain any of these paths:
            size_t foundFFT = mStr.find("fft");
            size_t foundDRLREF = mStr.find("drlref");
            size_t foundCONFIG = mStr.find("config");
            size_t foundVERSION = mStr.find("version");
            size_t foundBATT = mStr.find("batt");
            if (foundFFT == string::npos && foundDRLREF == string::npos && foundCONFIG == string::npos && foundVERSION == string::npos && foundBATT == string::npos) {
                
                // write the time to file
                bCsv.setFloat(bCounter, 0, bTimeNow);
                
                // write the address (path) to file
                bCsv.setString(bCounter, 1, mStr);
                
                int numArgs = mess.getNumArgs();
                // if eeg or acc, subtract two arguments
                if (foundEEG != string::npos || foundACC != string::npos) {
                    numArgs -= 2;
                }
                
                // write each argument to file
                for (int i = 0; i < numArgs; i++) {
                    // determine what kind of argument it is and write to file accordingly
                    if(mess.getArgType(i) == OFXOSC_TYPE_INT32){
                        bCsv.setInt(bCounter, i + 2, mess.getArgAsInt32(i));
                    }
                    else if(mess.getArgType(i) == OFXOSC_TYPE_FLOAT){
                        bCsv.setFloat(bCounter, i + 2, mess.getArgAsFloat(i));
                    }
                }
                bCounter++;
            }
            bTimeBefore = bTimeNow;
        }
    }
}

//--------------------------------------------------------------

// draw real time gaze
void recordData::drawLiveGaze() {
    
    if (drawLiveGazeBool) {
        if (eTribe.isConnected()) {
            
            ofSetCircleResolution(45); // num edges of regular polygon
            ofFill();
            
            // draw raw gaze point
            ofSetColor(ofColor::red);
            ofCircle(eTribe.getPoint2dRaw(), 5);
            
            // draw smoothed gaze point
            ofSetColor(ofColor::green);
            ofCircle(eTribe.getPoint2dAvg(), 10);
            
            // draw circle in avg point when fixed
            if (eTribe.isFix()) {
                ofSetColor(0);
                ofCircle(eTribe.getPoint2dAvg(), 5);
            }
        }
    }
}

//--------------------------------------------------------------

// record eye data to a path object so it can be drawn
void recordData::recordEyePath() {
    
    // start new path each new recording
    if (eRecordingState) {
        if (eTribe.getPoint2dAvg() != origin) {
            eyePath.lineTo(eTribe.getPoint2dAvg().x, (eTribe.getPoint2dAvg().y + scrollDistance));
        }
    } else if (recordEyePathBool) { // otherwise, manually start new path w/ 'k'
        if (eTribe.getPoint2dAvg() != origin) {
            eyePath.lineTo(eTribe.getPoint2dAvg().x, (eTribe.getPoint2dAvg().y + scrollDistance));
        }
    }
}

//--------------------------------------------------------------

// draw recorded or live eye path to the display window
void recordData::drawEyePath() {
    
    if (drawEyePathBool) { // toggle drawing the eye path
        switch (drawEyePathType) {
            case 0: { // simple line
                eyePath.setFilled(false);
                eyePath.setStrokeWidth(1);
                eyePath.setStrokeColor(0);
                ofPushMatrix();
                ofTranslate(0.0, -scrollDistance);
                eyePath.draw();
                ofPopMatrix();
            }
                break;
            case 1: { // points on line
                eyePath.setFilled(false);
                eyePath.setStrokeWidth(1);
                ofColor col;
                eyePath.setStrokeColor(ofColor(0, 75));
                ofPushMatrix();
                ofTranslate(0.0, -scrollDistance);
                eyePath.draw();
                ofPopMatrix();
                vector<ofPolyline> tempPoly = eyePath.getOutline();
                vector<ofPoint> ptArray = tempPoly[0].getVertices();
                for (int i = 0; i < ptArray.size(); i++) {
                    ofSetColor(0);
                    ofCircle(ptArray[i].x, ptArray[i].y - scrollDistance, 2.5);
                }
            }
                break;
            case 2: { // large transparent circles
                eyePath.setFilled(false);
                eyePath.setStrokeWidth(0);
                vector<ofPolyline> tempPoly = eyePath.getOutline();
                vector<ofPoint> ptArray = tempPoly[0].getVertices();
                for (int i = 0; i < ptArray.size(); i++) {
                    ofSetColor(0, 7);
                    ofCircle(ptArray[i].x, ptArray[i].y - scrollDistance, 35);
                }
            }
                break;
        }
    }
}

//--------------------------------------------------------------

// debug recording information to screen
void recordData::debug() {
    
    if (debugBool) {
        
        ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
        
        stringstream ss;
        ss << "DEBUG";
        if (eRecordingState) ss << endl << "Recording Eye to File";
        if (drawLiveGazeBool) ss << endl << "Drawing Live Gaze";
        if (recordEyePathBool) ss << endl << "Recording Path Only";
        if (drawEyePathBool) ss << endl << "Drawing Recorded Eye Path";
        if (bRecordingState) ss << endl << "Recording Brain to File";
        if (cRecordingState) ss << endl << "Recording BOTH to File";
        ofDrawBitmapStringHighlight(ss.str(), 10, 50);
    }
}