//
//  Created by Ben Snell and Will Miao on October 20, 2015
//
//  Self-Selective Portraiture: Component 3
//      Load select eye data, select brain data, and meta text (textGen).
//      Generate a probability tree of word relationships
//          based on the attentiveness to each word (textGen).
//      Generate text based on these probabilities and export it (textGen).
//
//

#pragma once

#include "ofMain.h"
#include "dataPt.h"
#include "textGen.h"
#include "textBlock.h"

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
		
    // text generator object
    textGen myGen;
};
