//
//  textGen.h
//  SSPortraiture_2_Analysis
//
//  Created by Ben Snell on 10/19/15.
//
//

#ifndef __SSPortraiture_2_Analysis__textGen__
#define __SSPortraiture_2_Analysis__textGen__

#include <ofMain.h>
#include "dataPt.h"
#include "textBlock.h"
#include "ofxCsv.h"

// store a word and its metadata (position, etc.)
struct metaWord {
    
public:
    
    string wordRaw;         // with punctuation
    string wordStripped;    // without punctuation and lowercase
    
    // includes punctuation:
    ofPoint position;       // bottom left corner
    ofPoint center;         // center of word
    float width;
    float height;
};

struct metaProb {
    
public:
    float prob = 0.0;
    int nProb = 0;
    
};

class textGen {
    
public:
    
    ofBuffer textBuffer;
    ofTrueTypeFont openSans;
    
    // properties of text sample:
    string fontName;
    string fileName;
    int fontSize;
    int sideMargin;
    int topMargin;
    ofColor textColor;
    float leadingScale;
    float paragraphSpacingScale;
    
    vector<string> rawText;
    
    vector<textBlock> inputText; // text to be read (input into "human system")
    
    // higher level organization of text
    // all information distilled into its necessary components of "meta words" containing word, location and size
    vector<metaWord> metaText;
    
    wng::ofxCsv tCsv;   // csv to hold metatext
    string tFileTitle;  // file name of csv
    
    // ------------------------------------------------------------
    // ---------------------LOAD, SET, DRAW TEXT-------------------
    // ------------------------------------------------------------
    
    // load text from file
    void loadText(string file_name, string font_name = "open_sans_regular.ttf");
    
    // set text style
    void setText(int font_size = 20, int side_margin = 100, int top_margin = 100, ofColor text_color = ofColor::black, float leading_scale = 1.45, float paragraph_spacing_scale = 0.45);
    
    // draw specified text
    void drawText(Boolean drawCenter = false);
    
    Boolean drawTextBool = true;
    
    // refresh (update) text -- based on change to diaply size, etc.
    void refreshText();
    
    // ------------------------------------------------------------
    // -------------------------RECORD TEXT------------------------
    // ------------------------------------------------------------
    
    // record text to store the data in an easily accessible form
    // (call after loading and setting the text to screen)
    // (returns a list of words and associated metadata, as well as saving a csv)
    vector<metaWord> recordText(string tRecordingTitle);
    
    // ------------------------------------------------------------
    // -----------------------LOAD META TEXT-----------------------
    // ------------------------------------------------------------
    
    // holds easy to access line object for working with the meta text
    ofPolyline metaTextPolyline;
    
    // load from file
    void loadMetaTextFromFile(string file_name);
    
    // loads meta text from existing vector
    void loadMetaText();
    
    // draw the meta text polyline
    void drawMetaTextLine(ofColor lineColor = ofColor::black);
    
    // draw the meta text words (close to where they are)
    void drawMetaTextWords(ofColor wordsColor = ofColor::black);
    
    // ------------------------------------------------------------
    // -------------------LOAD EYE & BRAIN DATA--------------------
    // ------------------------------------------------------------
    
    // load data from file
    void loadData(string E_file_name, string B_file_name);
    
    vector<dataPt> eyeData;
    vector<dataPt> brainData;
    
    ofPolyline eyePolyline;
    ofPolyline brainPolyline;
    
    // draw eye line
    void drawEyeLine(ofColor lineColor = ofColor::black);
    
    // draw brain line (not supposed to be to scale)
    void drawBrainLine(ofColor lineColor = ofColor::black);
    
    
    // load data directly from a data set object
    //    void loadData(dataSet thisSet);
    
    // load data from vectors of data points
    //    void loadData(vector<dataPt> EData, vector<dataPt> BData);
    
    // load data from polylines
    //    void loadData(ofPolyline ELine, ofPolyline BLine);
    
    // ------------------------------------------------------------
    // -----------------GENERATE PROBABILITY TREE------------------
    // ------------------(EMOTIONAL VOCABULARY)--------------------
    // ------------------------------------------------------------
    
    // maximum distance for which a word will be associated with a point on line
    float distThreshold = 100;
    
    // vector to hold all known words in the order they "appear"
    // (reference for the rows and columns of the probability tree)
    vector<string> vocab;
    
    // vector of vectors to hold probability tree before exported to csv
    vector< vector<metaProb> > metaProbTree;
    
    // for exporting csv of the tree (only probabilities)
    wng::ofxCsv treeCsv;
    // for exporting csv of the vocab list
    wng::ofxCsv vocabCsv;
    
    // grows metaProbTree by one row and one column
    void growTree();
    
    // from three data sets (text, eye, brain), generate a probability tree
    void generateProbTree(string T_file_name);
};

#endif /* defined(__SSPortraiture_2_Analysis__textGen__) */