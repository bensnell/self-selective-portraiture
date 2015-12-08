//
//  dataSet.cpp
//  SSPortraiture_2_Analysis
//
//  Created by Ben Snell on 10/20/15.
//
//

#include "dataSet.h"

// Get Brain Data (general - only one type)
vector<dataPt> dataSet::getBData(string file_name, string target_type, double start_time, double stop_time){
    
    // clear vector we'll use to store the data
    BData.clear();
    
    // Store reference data
    dataSet::B_file_name = file_name;
    dataSet::B_target_type = target_type;
    dataSet::B_start_time = start_time;
    dataSet::B_stop_time = stop_time; // reassign if not called?
    
    Boolean writeData = false; // T = write data to tempReadings
    Boolean exitWhile = false; // T = exit the while loop
    
    // Reference original data file
    ofFile csv;
    csv.open(ofToDataPath(file_name), ofFile::ReadWrite, false);
    
    // Send data to buffer
    ofBuffer csvBuffer = csv.readToBuffer();
    
    // Get time zero (reference)
    string firstLine = csvBuffer.getFirstLine();
    vector<string> firstItems = ofSplitString(firstLine, ",");
    double timeZero = ofToDouble(firstItems.at(0));
    csvBuffer.resetLineReader(); // reset count
    
    unsigned long lineNow = 0; // current line number read by buffer
    
    // While there is data in the buffer, continue searching for data
    while (! csvBuffer.isLastLine() && !exitWhile) {
        
        // Get next line
        string currentLine = csvBuffer.getNextLine();
        
        // Split line by commas and place into vector
        vector<string> tempItems = ofSplitString(currentLine, ",");
        
        // remove delimiters from beginning and end of path (osc address)
        int beginIndex = tempItems.at(1).find_first_not_of(" \n\r\t");
        int endIndex = tempItems.at(1).find_last_not_of(" \n\r\t") + 1;
        tempItems.at(1) = tempItems.at(1).substr(beginIndex, endIndex);
        
        // at starting and ending times, begin and end writing of data
        if (!writeData) {
            if ((ofToDouble(tempItems.at(0)) - timeZero) >= start_time) {
                writeData = true;
            }
        } else {
            if ((ofToDouble(tempItems.at(0)) - timeZero) > stop_time) {
                exitWhile = true;
                // add stop_time?
            }
        }
        
        // T = continue writing select data to file
        if(writeData) {
            
            // If target phrase is found...
            if (!tempItems.at(1).compare(target_type)) {
                
                int numValues = tempItems.size() - 2;
                
                // Then copy data over to temp object
                // send in the number of arguments to store "values"
                dataPt tempDataPt (numValues);
                tempDataPt.time = ofToDouble(tempItems.at(0)) - timeZero;
                tempDataPt.type = ofToString(tempItems.at(1));
                
                // for each reading, add it to the temporary dataPt object
                for (int i = 0; i < numValues; i++) {
                    tempDataPt.values[i] = ofToDouble(tempItems.at(i + 2));
                }
                
                // add this object to the end of the vector of select readings
                BData.push_back(tempDataPt);
            }
        }
        
        // increase line number
        lineNow++;
    }
    
    // return this data vector for optional use
    return BData;
}

// --------------------------------------------------------------

// Get Eye Data
vector<dataPt> dataSet::getEData(string file_name, double start_time, double stop_time){
    
    // clear vector we'll use to store the data
    EData.clear();
    
    // Store reference data
    dataSet::E_file_name = file_name;
    dataSet::E_target_type = "eye_position";
    dataSet::E_start_time = start_time;
    dataSet::E_stop_time = stop_time; // reassign if not called?
    
    Boolean writeData = false; // T = write data to tempReadings
    Boolean exitWhile = false; // T = exit the while loop
    
    // Reference original data file
    ofFile csv;
    csv.open(ofToDataPath(file_name), ofFile::ReadWrite, false);
    
    // Send data to buffer
    ofBuffer csvBuffer = csv.readToBuffer();
    
    // Get time zero (reference)
    string firstLine = csvBuffer.getFirstLine();
    vector<string> firstItems = ofSplitString(firstLine, ",");
    double timeZero = ofToDouble(firstItems.at(0));
    csvBuffer.resetLineReader(); // reset count
    
    unsigned long lineNow = 0; // current line number read by buffer
    
    // While there is data in the buffer, continue searching for data
    while (! csvBuffer.isLastLine() && !exitWhile) {
        
        // Get next line
        string currentLine = csvBuffer.getNextLine();
        
        // Split line by commas and place into vector
        vector<string> tempItems = ofSplitString(currentLine, ",");
        
        // at starting and ending times, begin and end writing of data
        if (!writeData) {
            if ((ofToDouble(tempItems.at(0)) - timeZero) >= start_time) {
                writeData = true;
            }
        } else {
            if ((ofToDouble(tempItems.at(0)) - timeZero) > stop_time) {
                exitWhile = true;
                // add stop_time?
            }
        }
        
        // T = continue writing select data to file
        if(writeData) {
            
            int numValues = 2; // for x and y coordinate
            
            // Then copy data over to temp object
            // send in the number of arguments to store "values"
            dataPt tempDataPt (numValues);
            tempDataPt.time = ofToDouble(tempItems.at(0)) - timeZero;
            tempDataPt.type = "eye_position";
            
            // add x and y coordinate to values array
            tempDataPt.values[0] = ofToDouble(tempItems.at(1));
            tempDataPt.values[1] = ofToDouble(tempItems.at(2));
            
            // add this object to the end of the vector of select readings
            EData.push_back(tempDataPt);
        }
        
        // increase line number
        lineNow++;
    }
    
    // return this data vector for optional use
    return EData;
}

// --------------------------------------------------------------

// Get average culled value of a type
vector<dataPt> dataSet::getACData(string file_name, string target_type, double start_time, double stop_time) {
    
    // clear vector we'll use to store the data
    ACData.clear();
    
    // Store reference data
    dataSet::AC_file_name = file_name;
    dataSet::AC_target_type = target_type;
    dataSet::AC_start_time = start_time;
    dataSet::AC_stop_time = stop_time;
    
    Boolean writeData = false; // T = write data to tempReadings
    Boolean exitWhile = false; // T = exit the while loop
    
    // Reference original data file
    ofFile csv;
    csv.open(ofToDataPath(file_name), ofFile::ReadWrite, false);
    
    // Send data to buffer
    ofBuffer csvBuffer = csv.readToBuffer();
    
    // Get time zero (reference)
    string firstLine = csvBuffer.getFirstLine();
    vector<string> firstItems = ofSplitString(firstLine, ",");
    double timeZero = ofToDouble(firstItems.at(0));
    csvBuffer.resetLineReader(); // reset count
    
    unsigned long lineNow = 0; // current line number read by buffer
    
    // holds last is_good values
    vector<string> lastIsGood;
    for (int i = 0; i < 4; i++) lastIsGood.push_back("0");
    
    // While there is data in the buffer, continue searching for data
    while (! csvBuffer.isLastLine() && !exitWhile) {
        
        // Get next line
        string currentLine = csvBuffer.getNextLine();
        
        // Split line by commas and place into vector
        vector<string> tempItems = ofSplitString(currentLine, ",");
        
        // remove delimiters from beginning and end of path (osc address)
        int beginIndex = tempItems.at(1).find_first_not_of(" \n\r\t");
        int endIndex = tempItems.at(1).find_last_not_of(" \n\r\t") + 1;
        tempItems.at(1) = tempItems.at(1).substr(beginIndex, endIndex);
        
        // at starting and ending times, begin and end writing of data
        if (!writeData) {
            if ((ofToDouble(tempItems.at(0)) - timeZero) >= start_time) {
                writeData = true;
            }
        } else {
            if ((ofToDouble(tempItems.at(0)) - timeZero) > stop_time) {
                exitWhile = true;
                // add stop_time?
            }
        }
        
        if (!tempItems.at(1).compare("/muse/elements/is_good")) {
            lastIsGood = tempItems;
        }
        
        // T = continue writing select data to file
        if(writeData) {
            
            // If target phrase is found...
            if (!tempItems.at(1).compare(target_type)) {
                
                // Then copy time and type over to temp object
                dataPt tempDataPt (1); // only 1 value to hold average
                tempDataPt.time = ofToDouble(tempItems.at(0)) - timeZero;
                tempDataPt.type = ofToString(tempItems.at(1));
                
                // use the last recorded is_good reading as an indicator of which target_type readings to include in the average
                double sumTT =  0.0;    // sum of target types
                double nOnes = 0;       // number of ones in is_good
                int numValues = tempItems.size() - 2;
                for (int i = 0; i < numValues; i ++) {
                    double thisTT = ofToDouble(tempItems.at(i + 2));
                    double thisIG = ofToDouble(lastIsGood.at(i + 2));
                    sumTT += thisIG * thisTT;
                    nOnes += thisIG;
                }
                double avgTT = sumTT / nOnes;
                
                // add newly caluclated average to tempDataPt object
                tempDataPt.values[0] = avgTT;
                
                // add this object to the end of the vector of select readings
                ACData.push_back(tempDataPt);
            }
        }
        
        // increase line number
        lineNow++;
    }
    
    // return this data vector for optional use
    return ACData;
}

// --------------------------------------------------------------

// Get Brain Data Graph
// default datastream is all of the sets of data within BData (represented as -1)
vector<ofPolyline> dataSet::getBGraph(int dataStream, int graphW, int graphH) {
    
    // temporary vector to hold brain graphs
    vector <ofPolyline> tempGraphs;
    
    // STORE METADATA
    // SKIP FOR NOW
    
    int nGraphs = 1;
    if (dataStream < 0) {
        nGraphs = BData[0].nValues;
        dataStream = 0; // start with zero
    }
    
    // number of readings
    int nReadings = BData.size();
    
    // for each graph
    for (int i = dataStream; i < (dataStream + nGraphs); i++) {
        ofPolyline tempGraph;
        // go through all readings
        for (int j = 0; j < nReadings; j++) {
            double px = j * graphW / (nReadings - 1);
            double py = graphH - BData[j].values[i] * graphH;
            tempGraph.addVertex(px, py);
        }
        tempGraphs.push_back(tempGraph);
    }
    
    BGraph.clear();
    BGraph = tempGraphs;
    allBGraphs.push_back(tempGraphs);
    
    return tempGraphs;
}

// More specific function -- can pass in any Brain graph at end
vector<ofPolyline> dataSet::getBGraph(vector<dataPt> refData, int dataStream, int graphW, int graphH) {
    
    // temporary vector to hold brain graphs
    vector <ofPolyline> tempGraphs;
    
    // STORE METADATA
    // SKIP FOR NOW
    
    int nGraphs = 1;
    if (dataStream < 0) {
        nGraphs = refData[0].nValues;
        dataStream = 0; // start with zero
    }
    
    // number of readings
    int nReadings = refData.size();
    
    // for each graph
    for (int i = dataStream; i < (dataStream + nGraphs); i++) {
        ofPolyline tempGraph;
        // go through all readings
        for (int j = 0; j < nReadings; j++) {
            double px = j * graphW / (nReadings - 1);
            double py = graphH - refData[j].values[i] * graphH;
            tempGraph.addVertex(px, py);
        }
        tempGraphs.push_back(tempGraph);
    }
    
    BGraph.clear();
    BGraph = tempGraphs;
    allBGraphs.push_back(tempGraphs);
    
    return tempGraphs;
}

// --------------------------------------------------------------

// Get Eye Data Graph
ofPolyline dataSet::getEGraph(int startX, int startY, double scaleW, double scaleH) {
    
    ofPolyline tempGraph;
    
    int nReadings = EData.size();
    
    for (int i = 0; i < nReadings; i++) {
        double px = EData[i].values[0] * scaleW + startX;
        double py = EData[i].values[1] * scaleH + startY;
        tempGraph.addVertex(px, py);
    }
    
    EGraph = tempGraph;
    
    return tempGraph;
}

// --------------------------------------------------------------

// get graph of eye data as gradient
void dataSet::getEGraphGradient(ofColor startColor, ofColor stopColor, double cycleRate, int startX, int startY, double scaleW, double scaleH) {
    
    vector<ofPolyline> gradientLines;
    vector<ofColor> gradientColors;
    
    int nReadings = EData.size();
    
    double param = 0;
    double paramIncrement = 1.0 / (double)(nReadings - 1) * cycleRate;
    
    for (int i = 0; i < (nReadings - 1); i++) {
        
        // create a line segment
        ofPolyline lineSegment;
        
        double x1 = EData[i].values[0] * scaleW + startX;
        double y1 = EData[i].values[1] * scaleH + startY;
        double x2 = EData[i + 1].values[0] * scaleW + startX;
        double y2 = EData[i + 1].values[1] * scaleH + startY;
        
        lineSegment.addVertex(x1, y1);
        lineSegment.addVertex(x2, y2);
        
        // add segment to array
        gradientLines.push_back(lineSegment);
        
        // find color at this paramter
        ofColor segmentColor = startColor.getLerped(stopColor, param);
        param += paramIncrement;
        if (param > 1.0 || param < 0) {
            paramIncrement *= -1.0;
            param += paramIncrement;
        }
        
        //        cout << ofToString(nReadings) << "  " << ofToString(i) < "  " << ofToString(param) << "  " <<
        
        // add color to vector
        gradientColors.push_back(segmentColor);
    }
    
    EGraphGradientLines = gradientLines;
    EGraphGradientColors = gradientColors;
}

// --------------------------------------------------------------

// draw last Bgraph
void dataSet::drawBGraphLast(int graphIndex, ofColor graphColor, int startX, int startY) {
    
    int nGraphs = 1;
    if (graphIndex < 0) {
        nGraphs = BGraph.size();
        graphIndex = 0;
    }
    
    ofSetColor(graphColor);
    
    ofPushMatrix();
    ofTranslate(startX, startY);
    for (int i = graphIndex; i < (graphIndex + nGraphs); i++) {
        BGraph[i].draw();
    }
    ofPopMatrix();
}

// --------------------------------------------------------------

// draw allBGraphs or a specific one (by specifying the graphSet index)
void dataSet::drawBGraphAll(int graphSet, ofColor graphColor, int startX, int startY) {
    
    int nSets = 1;
    if (graphSet < 0) {
        nSets = allBGraphs.size();
        graphSet = 0;
    }
    
    ofSetColor(graphColor);
    
    ofPushMatrix();
    ofTranslate(startX, startY);
    // for each set of graphs
    for (int i = graphSet; i < (graphSet + nSets); i++) {
        // find number of graphs within set
        int nGraphs = allBGraphs[i].size();
        // for each graph, draw it
        for (int j = 0; j < nGraphs; j++) {
            allBGraphs[i][j].draw();
        }
    }
    ofPopMatrix();
}

// --------------------------------------------------------------

// draw bGraph vector passed in
void dataSet::drawBGraph(vector<ofPolyline> theseGraphs, int startX, int startY, ofColor graphColor) {
    
    ofSetColor(graphColor);
    
    int nGraphs = theseGraphs.size();
    
    ofPushMatrix();
    ofTranslate(startX, startY);
    for (int i = 0; i < nGraphs; i++) {
        theseGraphs[i].draw();
    }
    ofPopMatrix();
}

// --------------------------------------------------------------

// draw single brain polyline passed in
void dataSet::drawBGraph(ofPolyline thisLine, int startX, int startY, ofColor graphColor) {
    
    ofSetColor(graphColor);
    
    ofPushMatrix();
    ofTranslate(startX, startY);
    thisLine.draw();
    ofPopMatrix();
}

// --------------------------------------------------------------

// draw eye line (graph)
void dataSet::drawEGraphSolid(ofColor graphColor) {
    
    ofSetColor(graphColor);
    
    EGraph.draw();
}

// --------------------------------------------------------------

// draw eye graph with a gradient
void dataSet::drawEGraphGradient() {
    
    int nSegments = EGraphGradientLines.size();
    
    for (int i = 0; i < nSegments; i++) {
        ofSetColor(EGraphGradientColors[i]);
        EGraphGradientLines[i].draw();
    }
}

// --------------------------------------------------------------

// draw the closest point to the eye path
// Note: must run myDataSet.getEGraph() to run this command
void dataSet::drawEClosestPoint() {
    
    int nReadings = EData.size();
    
    ofSetColor(0);
    
    unsigned int closestIndex; // index of closest vertex on polyline
    
    // find closest point to cursor and draw circle at it
    ofPoint cursorPt(ofGetMouseX(), ofGetMouseY());
    ofPoint closestPt = EGraph.getClosestPoint(cursorPt, &closestIndex);
    
    ofPoint closestVertex(EData[closestIndex].values[0], EData[closestIndex].values[1]);
    ofCircle(closestVertex, 2.0);
    
    // find closest Time
    double closestTime = EData[closestIndex].time;
    
    // draw data near the cursor
    stringstream ss;
    ss << "i: " + ofToString(closestIndex) << endl;
    ss << "t: " + ofToString(closestTime);
    ofDrawBitmapStringHighlight(ss.str(), closestVertex.x + 15, closestVertex.y - 30);
}

// --------------------------------------------------------------

// export select data to a csv file
// MUST SPECIFY EYE OR BRAIN!
// order of data: time, type, values (1)
void dataSet::exportData(vector<dataPt> refData, string file_name) {
    
    wng::ofxCsv tempCsv;
    
    string tempFileName = "SELECT " + file_name + ".csv";
    tempCsv.createFile(ofToDataPath(tempFileName));
    
    int nRows = refData.size();
    int nValues = refData[0].nValues;
    
    for (int i = 0; i < nRows; i++) {
        tempCsv.setFloat(i, 0, refData[i].time);
        tempCsv.setString(i, 1, refData[i].type);
        for (int j = 0; j < nValues; j++) {
            tempCsv.setFloat(i, j + 2, refData[i].values[j]);
        }
    }
    
    tempCsv.saveFile(ofToDataPath(tempFileName));
}