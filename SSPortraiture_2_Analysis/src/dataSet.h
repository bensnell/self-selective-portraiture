//
//  dataSet.h
//  SSPortraiture_2_Analysis
//
//  Created by Ben Snell on 10/20/15.
//
//  Intended Use: Call a function below to store data in a a temp vector within the class. This is perfectly fine, but when working with a bunch of data from one data set, use the vector returned by these functions and store them in a vector in ofApp.cpp
//  TODO: How do you pass an optional reference (of member's attributes) as an argument to a member function?

//
//

#ifndef __SSPortraiture_2_Analysis__dataSet__
#define __SSPortraiture_2_Analysis__dataSet__

#include <ofMain.h>
#include "dataPt.h"
#include "ofxCsv.h"

/*
 TODO: How do I hold more than one type of reading in BData?
 Solution: In Setup:
 vector <vector <dataPt> > BData;
 Then at the end of every call of getBData:
 BData.pushBack(tempBData);
 Finally to access:
 BData[0];
 When graphing, should default to first vector in  BData:
 void drawBData( int x, int y, int w, int h, color, target_stream = 0);
 */

// TODO: Is it bad practice to make a function return a value but when I use it I don't "catch" that value?

// structure for containing single data set (i.e. brain and eye test)
class dataSet {
    
public:
    
    // ---------------------------------------------
    // ----------------- GET DATA ------------------
    // ---------------------------------------------
    
    // Vector to hold last set of selected data points
    vector<dataPt> BData;   // brain
    vector<dataPt> EData;   // eye
    vector<dataPt> ACData;  // average culled data
    
    // Retrieve BRAIN DATA from a file
    vector<dataPt> getBData(string file_name, string target_type, double start_time = 0.0, double stop_time = 86400.0);
    
    // Retrieve EYE DATA from a file
    vector<dataPt> getEData(string file_name, double start_time = 0.0, double stop_time = 86400.0);
    
    // Finds target type value averaged across all 4 electrodes when is_good = 1
    // must send in data type with 4 values
    vector<dataPt> getACData(string file_name, string target_type, double start_time = 0.0, double stop_time = 86400.0);
    
    // ^
    // file_name        csv file containing data to search within
    // target_type      type of data to look for
    // start_time       start timestamp of desired data interval
    // stop_time        stop timestamp of desired data interval
    
    // Values sent into get_Data (for reference)
    string B_file_name;
    string B_target_type;
    double B_start_time;
    double B_stop_time;
    
    string E_file_name;
    string E_target_type; // always eye_position
    double E_start_time;
    double E_stop_time;
    
    string AC_file_name;
    string AC_target_type;
    double AC_start_time;
    double AC_stop_time;
    
    // ---------------------------------------------
    // ----------------- GET GRAPH -----------------
    // ---------------------------------------------
    
    // Get Brain Data Graph (OVERLOADED)
    // get graph from data in BData
    vector<ofPolyline> getBGraph(int dataStream = -1, int graphW = 800, int graphH = 200);
    // get graph from any vector of data points passed in (ex. ACData)
    vector<ofPolyline> getBGraph(vector<dataPt> refData, int dataStream = -1, int graphW = 800, int graphH = 200);
    
    // holds last vector of brain graphs
    vector<ofPolyline> BGraph;
    // holds all brain graphs made with dataSet as separate vector for each call
    vector< vector <ofPolyline> > allBGraphs;
    
    // ---------------------------------------------
    
    // Get Eye Data Graph
    ofPolyline getEGraph(int startX = 0, int startY = 0, double scaleW = 1.0, double scaleH = 1.0);
    
    // holds Eye Graph for this data set
    ofPolyline EGraph;
    
    // Draw Eye Graph with Gradient
    void getEGraphGradient(ofColor startColor = ofColor::lightGray, ofColor stopColor = ofColor::black, double cycleRate = 1.0, int startX = 0, int startY = 0, double scaleW = 1.0, double scaleH = 1.0);
    
    // holds gradient graph data
    vector<ofPolyline> EGraphGradientLines;
    vector<ofColor> EGraphGradientColors;
    
    // To get the graph of ACDData, simply run it through getBGraph()
    
    // ---------------------------------------------
    // ---------------- DRAW GRAPH -----------------
    // ---------------------------------------------
    
    // draw last BGraph
    void drawBGraphLast(int graphIndex = -1, ofColor graphColor = ofColor::black, int startX = 50, int startY = 50);
    
    // draw allBGraphs or a specific one (by specifying the graphSet index)
    void drawBGraphAll(int graphSet = -1, ofColor graphColor = ofColor::black, int startX = 50, int startY = 50);
    
    // draw bGraph vector passed in
    void drawBGraph(vector<ofPolyline> theseLines, int startX = 50, int startY = 50, ofColor graphColor = ofColor::black);
    
    // draw single brain polyline passed in
    void drawBGraph(ofPolyline thisLine, int startX = 50, int startY = 50, ofColor graphColor = ofColor::black);
    
    // ---------------------------------------------
    
    // draw eye line (graph)
    void drawEGraphSolid(ofColor graphColor = ofColor::black);
    
    // draw eye graph with a gradient
    void drawEGraphGradient();
    
    // ---------------------------------------------
    // ------------ DRAW CLOSEST POINT -------------
    // ---------------------------------------------
    
    void drawEClosestPoint();
    
    // ---------------------------------------------
    // ------------ EXPORT EDITED DATA -------------
    // ---------------------------------------------
    
    // export selected data to csv file
    // MUST SPECIFY EYE OR BRAIN
    void exportData(vector<dataPt> refData, string file_name);
    
    //    void exportBData();
    
    //    void exportEData();
    
};

#endif /* defined(__SSPortraiture_2_Analysis__dataSet__) */