//
//  Created by Ben Snell and Will Miao on October 20, 2015
//
//  Self-Selective Portraiture: Component 2
//      Load raw eye data and raw brain data (dataSet).
//          Optionally, also load text (textGen).
//      Analyze data through graphs if necessary (dataSet and textGen).
//      Export select brain data and select eye data (dataSet).
//
//

#pragma once

#include "ofMain.h"
#include "dataPt.h"
#include "dataSet.h"
#include "textGen.h"
#include "textBlock.h"
#include "ofxCsv.h"

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
		
    dataSet mySet;
    textGen myGen;
};
