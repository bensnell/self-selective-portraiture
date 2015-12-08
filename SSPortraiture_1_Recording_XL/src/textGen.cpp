//
//  textGen.cpp
//  SSPortraiture_1_Recording_XL
//
//  Created by Ben Snell on 10/20/15.
//
//
// TODO: Also remove brackets from text when recording it


#include "textGen.h"

// Load a text file
// TIPS: Empty lines are deleted, paragraph structure is maintained, paragraphs are spaced apart
void textGen::loadText(string file_name, string font_name) {
    
    // clear vector if called second time
    rawText.clear();
    
    fontName = font_name;
    fileName = file_name;
    
    // load buffer
    textBuffer = ofBufferFromFile(file_name);
    
    // store each paragraph in the rawText vector
    while (!textBuffer.isLastLine()) {
        string thisLine = textBuffer.getNextLine();
        if (!thisLine.empty()) {
            // if the line is empty, don't include it
            rawText.push_back(thisLine);
        }
    }
}

//---------------------------------------------------------------------

// set text style
void textGen::setText(int font_size, int side_margin, int top_margin, ofColor text_color, float leading_scale, float paragraph_spacing_scale) {
    
    // clear vector if fullscreen is called
    inputText.clear();
    
    fontSize = font_size;
    sideMargin = side_margin;
    topMargin = top_margin;
    textColor = text_color;
    leadingScale = leading_scale;
    paragraphSpacingScale = paragraph_spacing_scale;
    
    // find number of paragraphs
    int nParagraphs = rawText.size();
    
    // put each paragraph in a textBlock class and set its content and style
    for (int i = 0; i < nParagraphs; i++) {
        
        textBlock tempTextBlock;
        
        tempTextBlock.init(fontName, font_size);
        tempTextBlock.setText(rawText[i]);
        tempTextBlock.setColor(text_color.r, text_color.g, text_color.b, text_color.a); // must set after setting text
        tempTextBlock.setLineHeight(leading_scale * (float)font_size);
        
        inputText.push_back(tempTextBlock);
    }
    
    // set left and right (side) margins
    float tempTextWidth = ofGetWidth() - side_margin * 2;
    for (int i = 0; i < nParagraphs; i++) {
        inputText[i].wrapTextX(tempTextWidth);
    }
    
    // set the top margin and subsequent paragraph vertical placement
    float tempTopMargin = (float)top_margin;
    float paragraphSpacing = (float)font_size * (leading_scale + paragraph_spacing_scale);
    // find the lowest point of each paragraph and use it to set the top margin of the next paragraph
    for (int i = 0; i < nParagraphs; i ++) {
        inputText[i].paragraphX = (float)side_margin;
        inputText[i].paragraphY = tempTopMargin;
        
        tempTopMargin += inputText[i].getHeight() + paragraphSpacing;
    }
    
    // "ghost draw" the text to screen to record the coordinates of each word within the objects themselves. This is done with ghostDrawLeft or drawLeft (or just draw)
    for (int i = 0; i < inputText.size(); i++) {
        inputText[i].ghostDrawLeft(inputText[i].paragraphX, inputText[i].paragraphY);
    }
}

//---------------------------------------------------------------------

// draw text to screen
void textGen::drawText(int yTranslate, Boolean drawCenter) {
    
    if (drawTextBool) {
        
        int nParagraphs = inputText.size();
        
        // for now only use default justified draw
        for (int i = 0; i < nParagraphs; i++) {
            
            inputText[i].draw(inputText[i].paragraphX, inputText[i].paragraphY - yTranslate);
        }
        
        if (drawCenter) {
            for (int i = 0; i < metaText.size(); i++) {
                ofSetColor(ofColor::red);
                ofCircle(metaText[i].center.x, metaText[i].center.y, 5);
            }
        }
    }
}

//---------------------------------------------------------------------

// record text with current orientation
vector<metaWord> textGen::recordText(string tRecordingTitle) {
    
    // clear metaText if refreshing object
    metaText.clear();
    
    // loop through every paragraph
    int nParagraphs = inputText.size();
    for (int i = 0; i < nParagraphs; i++) {
        
        // loop through all words
        int nWords = inputText[i].words.size();
        for (int j = 0; j < nWords; j++) {
            
            // if string isn't just whitespace, then continue
            string tempWord = inputText[i].words[j].rawWord;
            if (tempWord.find_first_not_of(' ') != string::npos) {
                
                metaWord tempMetaWord;
                
                // store raw word
                tempMetaWord.wordRaw = tempWord;
                
                // remove delimiters from beginning and end of path (osc address)
                int beginIndex = tempWord.find_first_not_of(" \n\r\t\".,:;/!?");
                int endIndex = tempWord.find_last_not_of(" \n\r\t\".,:;/!?") + 1;
                tempWord = tempWord.substr(beginIndex, endIndex);
                // make it all lowercase
                tempWord = ofToLower(tempWord);
                
                // store "clean" stripped word
                tempMetaWord.wordStripped = tempWord;
                
                // store position data
                ofPoint tempPosition;
                tempPosition.x = inputText[i].words[j].wordX;
                tempPosition.y = inputText[i].words[j].wordY;
                tempMetaWord.position = tempPosition;
                
                // store width and height
                tempMetaWord.width = inputText[i].words[j].width;
                tempMetaWord.height = inputText[i].words[j].height;
                
                // store the center of the word
                // Note: Center isn't at center of height, but a third the distance up the word height
                ofPoint tempCenter;
                tempCenter.x = tempPosition.x + tempMetaWord.width / 2;
                tempCenter.y = tempPosition.y - (float)fontSize * 2/5; // 2/5 words well
                tempMetaWord.center = tempCenter;
                
                // add meta word to vector
                metaText.push_back(tempMetaWord);
            }
        }
    }
    
    // add data to tCsv
    stringstream tStreamTitle;
    tStreamTitle << ofGetYear() << "-";
    tStreamTitle <<  setw(2) << setfill('0') << ofGetMonth() << "-";
    tStreamTitle <<  setw(2) << setfill('0') << ofGetDay() << " ";
    tStreamTitle <<  setw(2) << setfill('0') << ofGetHours() << "-";
    tStreamTitle <<  setw(2) << setfill('0') << ofGetMinutes() << "-";
    tStreamTitle <<  setw(2) << setfill('0') << ofGetSeconds();
    tFileTitle = "METATEXT " + tRecordingTitle + " " + tStreamTitle.str() + ".csv";
    tCsv.createFile(ofToDataPath(tFileTitle));
    
    for (int i = 0; i < metaText.size(); i++) {
        //        tCsv.setString(i, 0, metaText[i].wordRaw); // WRITING THIS TO FILE WILL CAUSE ALL SORTS OF PROBLEMAS (since the commas, etc. are maintained)
        tCsv.setString(i, 0, metaText[i].wordStripped);
        tCsv.setFloat(i, 1, metaText[i].position.x);
        tCsv.setFloat(i, 2, metaText[i].position.y);
        tCsv.setFloat(i, 3, metaText[i].center.x);
        tCsv.setFloat(i, 4, metaText[i].center.y);
        tCsv.setFloat(i, 5, metaText[i].width);
        tCsv.setFloat(i, 6, metaText[i].height);
    }
    
    tCsv.saveFile(ofToDataPath(tFileTitle));
    
    return metaText;
}

//---------------------------------------------------------------------

// call if attributes have changed
void textGen::refreshText() {
    
    rawText.clear();
    inputText.clear();
    metaText.clear();
    
    loadText(fileName, fontName);
    setText(fontSize, sideMargin, topMargin, textColor, leadingScale, paragraphSpacingScale);
    // REMOVED RECORD TEXT HERE
}

//---------------------------------------------------------------------

// load meta text from file
void textGen::loadMetaTextFromFile(string file_name) {
    
    metaText.clear();
    metaTextPolyline.clear();
    
    // load CSV
    tCsv.loadFile(ofToDataPath(file_name));
    int nRows = tCsv.numRows;
    
    for (int i = 0; i < nRows; i++) {
        metaWord tempMetaWord;
        
        //        tempMetaWord.wordRaw = tCsv.getString(i, 0);
        tempMetaWord.wordStripped = tCsv.getString(i, 0);
        tempMetaWord.position.x = tCsv.getFloat(i, 1);
        tempMetaWord.position.y = tCsv.getFloat(i, 2);
        tempMetaWord.center.x = tCsv.getFloat(i, 3);
        tempMetaWord.center.y = tCsv.getFloat(i, 4);
        tempMetaWord.width = tCsv.getFloat(i, 5);
        tempMetaWord.height = tCsv.getFloat(i, 6);
        
        // add points to line
        metaTextPolyline.addVertex(tempMetaWord.center);
        
        metaText.push_back(tempMetaWord);
    }
}

//---------------------------------------------------------------------

// load meta text from existing vector
void textGen::loadMetaText() {
    
    metaTextPolyline.clear();
    
    // add each point to line
    for (int i = 0; i < metaText.size(); i++) {
        metaTextPolyline.addVertex(metaText[i].center);
    }
}

//---------------------------------------------------------------------

void textGen::drawMetaTextLine(ofColor lineColor) {
    
    ofSetColor(lineColor);
    metaTextPolyline.draw();
}

//---------------------------------------------------------------------

void textGen::drawMetaTextWords(ofColor wordsColor) {
    
    ofSetColor(wordsColor);
    for (int i = 0; i < metaText.size(); i++) {
        ofDrawBitmapString(metaText[i].wordRaw, metaText[i].position);
    }
}

//---------------------------------------------------------------------

// load eye and brain data into vectors of dataPt objects
// data order: time, type, values (1)
void textGen::loadData(string E_file_name, string B_file_name) {
    
    eyeData.clear();
    brainData.clear();
    
    wng::ofxCsv tempCsv;
    
    // load eye data
    tempCsv.loadFile(ofToDataPath(E_file_name));
    
    // write eye data to accessible vector and polyline
    int nRows = tempCsv.numRows;
    for (int i = 0; i < nRows; i++) {
        
        dataPt tempDataPt(2);
        
        tempDataPt.time = tempCsv.getFloat(i, 0);
        tempDataPt.type = tempCsv.getString(i, 1);
        tempDataPt.values[0] = tempCsv.getFloat(i, 2);
        tempDataPt.values[1] = tempCsv.getFloat(i, 3);
        
        eyePolyline.addVertex(tempDataPt.values[0], tempDataPt.values[1]);
        
        eyeData.push_back(tempDataPt);
    }
    
    tempCsv.clear();
    
    // load brain data
    tempCsv.loadFile(ofToDataPath(B_file_name));
    
    // write brain data to accessible vector and polyline
    nRows = tempCsv.numRows;
    for (int i = 0; i < nRows; i++) {
        
        dataPt tempDataPt(1);   // just take the first brain value
        
        tempDataPt.time = tempCsv.getFloat(i, 0);
        tempDataPt.type = tempCsv.getString(i, 1);
        tempDataPt.values[0] = tempCsv.getFloat(i, 2);
        
        brainPolyline.addVertex(tempDataPt.time, tempDataPt.values[0]);
        
        brainData.push_back(tempDataPt);
    }
}

//---------------------------------------------------------------------

// draw the eye line
void textGen::drawEyeLine(ofColor lineColor) {
    ofSetLineWidth(1);
    ofSetColor(lineColor);
    eyePolyline.draw();
}

//---------------------------------------------------------------------

// draw the brain line used to calculate the probability tree
void textGen::drawBrainLine(ofColor lineColor) {
    ofSetLineWidth(1);
    ofSetColor(lineColor);
    brainPolyline.draw();
}

//---------------------------------------------------------------------

// create the probability tree
void textGen::generateProbTree(string T_file_name) {
    
    // for each word in metaText, find the closest point on eye polyline
    int nWords = metaText.size();
    for (int i = 0; i < nWords; i++) {
        
        // ---------------- FIND CLOSEST TIME --------------------
        
        string thisWord = metaText[i].wordStripped;
        ofPoint thisPoint = metaText[i].center;
        
        unsigned int closestEyeIndex; // closest index to reference time at this point
        ofPoint closestEyePoint = eyePolyline.getClosestPoint(thisPoint, &closestEyeIndex);
        
        // if distance between closestPoint and thisPoint is above a certain threshold or if it's the first point on the eye path, then increment the loop's iterator and start with a new word
        float distPoints = ofDist(thisPoint.x, thisPoint.y, closestEyePoint.x, closestEyePoint.y);
        
        if ((distPoints > distThreshold) || (closestEyeIndex == 0)) {
            continue;
        }
        
        float thisTime = eyeData[closestEyeIndex].time; // time at closest point on eye path
        
        
        // --------------- INTERPOLATE PROBABILITY ----------------
        
        // shouldn't this be done after looking for a previous word???????
        
        // interpolate probability from brain data at this time
        float thisProb;
        // First, search all brain data for the interval in which this time resides
        int ceilBrainIndex = -1; // ceiling of closest brain index (unless at end of time)
        int floorBrainIndex = -1;
        for (int j = 0; j < brainData.size(); j++) {
            
            // exact time match
            if (brainData[j].time == thisTime) {
                ceilBrainIndex = j;
                floorBrainIndex = j;
            }
            
            // found an upper limit
            else if (brainData[j].time > thisTime) {
                ceilBrainIndex = j;
                if (j != 0) {
                    floorBrainIndex = j - 1;
                }
                break;
            }
        }
        
        // Then, find the correponding probability depending on where this time resides in the data set (beg, end or middle)
        
        // no lower bound (eye time is below lowest brain time)
        if (floorBrainIndex == -1) {
            thisProb = brainData[0].values[0];
        }
        // no upper bound (eye time is above highest brain time)
        else if (ceilBrainIndex == -1) {
            thisProb = brainData[brainData.size() - 1].values[0]; // MINUS 1???
        }
        // eye time equals brain time
        else if (ceilBrainIndex == floorBrainIndex) {
            // use either floor or brain index
            thisProb = brainData[floorBrainIndex].values[0];
        }
        // interpolate reading between floor and ceil index
        else {
            float floorTime = brainData[floorBrainIndex].time;
            float ceilTime = brainData[ceilBrainIndex].time;
            float lerpAmt = (float)(thisTime - floorTime) / (float)(ceilTime - floorTime);
            
            float floorVal = brainData[floorBrainIndex].values[0];
            float ceilVal = brainData[ceilBrainIndex].values[0];
            thisProb = ofLerp(floorVal, ceilVal, lerpAmt);
        }
        
        
        // ------------------ FIND PREVIOUS WORD -------------------
        // NOTE: THIS DOES NOT APPEAR TO FIND THE PREVIOUS WORD LOOKED AT, SINCE THE SPATIAL RESOLUTION OF GAZE IS INHERENTLY NOISY. IN OTHER WORDS, THE EYE IS NATURALLY JUMPY, SO THIS WILL NOT WORK UNLESS TEXT IS LARGER AND MORE SPACED OUT.
        
        // Find the next closest prevoius word on the eye path (if it exists)
        Boolean foundDiffPrevWord = false; // T = prev word found
        int prevEyeIndex = closestEyeIndex - 1;
        unsigned int prevWordIndex;
        while(!foundDiffPrevWord) {
            
            // find previous point to the closest point on the eye path
            ofPoint prevClosestEyePoint = ofPoint(eyeData[prevEyeIndex].values[0], eyeData[prevEyeIndex].values[1]);
            
            // find the index of the word closest to this previous point
            metaTextPolyline.getClosestPoint(prevClosestEyePoint, &prevWordIndex);
            
            // if this isn't the same contextualized word, continue
            // Should we allow words to repeat? Yes. Just make sure they don't get caught in a loop indefinitely.
            if (prevWordIndex != i) {
                foundDiffPrevWord = true;
                break;
            }
            
            prevEyeIndex--; // decrement index to try word prev to the previous
            if (prevEyeIndex < 0) break; // no more previous words to search
        }
        
        // go back to beginning of for loop and try the next word in metaText if previous word could not be found
        if (!foundDiffPrevWord) continue;
        
        // Now, only word with acceptable previous words will be used in the tree
        
        string prevWord = metaText[prevWordIndex].wordStripped;
        
        
        
        //----------------- ADD PROBABILITY TO TREE -----------------
        
        // Add thisProb to metaProbTree
        
        int probRow = -1;    // wordNow = row
        int probCol = -1;    // prevWord = column
        
        // If it's the first word, add it to the tree
        if (vocab.size() == 0) {
            probCol = 0;    // default
            probRow = 1;    // default
            
            // add to vocab
            vocab.push_back(prevWord);
            vocab.push_back(thisWord);
            
            // increase metaProbTree to 2 x 2 matrix
            vector<metaProb> twoMetaProb(2);
            metaProbTree.push_back(twoMetaProb);
            metaProbTree.push_back(twoMetaProb);
            
            // set spot (1, 0) to this probability
            metaProbTree[probRow][probCol].prob = thisProb;
            metaProbTree[probRow][probCol].nProb = 1;
        }
        // otherwise, update vocab and tree size as appropriate and update probability
        else {
            
            // loop through all words in vocab to compare both prevWord and thisWord and assign their rows and columns
            // if words are not found, add them to vocab and add a row and column to the vector of vectors
            
            // search for prevoius word in vocab
            for (int j = 0; j < vocab.size(); j++) {
                string comparisonWord = vocab[j];
                
                // word is found
                if (!prevWord.compare(comparisonWord)) {
                    // column in metaProbTree = current index (j)
                    probCol = j;
                    break; // stop looking
                }
            }
            // if word wasn't found
            if (probCol < 0) {
                // add to end of vocab
                probCol = vocab.size();
                vocab.push_back(prevWord);
                growTree(); // increase rows and columns by 1
            }
            
            // search for current word in vocab
            for (int j = 0; j < vocab.size(); j++) {
                string comparisonWord = vocab[j];
                
                // word is found
                if (!thisWord.compare(comparisonWord)) {
                    // column in metaProbTree = current index (j)
                    probRow = j;
                    break; // stop looking
                }
            }
            // if word wasn't found
            if (probRow < 0) {
                // add to end of vocab
                probRow = vocab.size();
                vocab.push_back(thisWord);
                growTree(); // increase rows and columns by 1
            }
            
            // check the probability there and add the new one in
            float avgProb = metaProbTree[probRow][probCol].prob;
            int numAvgProb = metaProbTree[probRow][probCol].nProb;
            
            int newNumAvgProb = numAvgProb + 1;
            float newAvgProb = (avgProb * (float)numAvgProb + thisProb) / ((float)newNumAvgProb);
            
            metaProbTree[probRow][probCol].prob = newAvgProb;
            metaProbTree[probRow][probCol].nProb = newNumAvgProb;
        }
        // probability has now been assigned to appropriate spot!
        
    }
    // all words have now been accounted for!
    
    // ADD TIME STAMPS BELOW SO ALL FILES ARE 100% UNIQUE
    
    // NOW, copy the prob tree to a csv and save to file
    string tempFileName = "TREE_PROB " + T_file_name + ".csv";
    treeCsv.createFile(ofToDataPath(tempFileName));
    
    int nRows = metaProbTree.size();
    int nCols = metaProbTree[0].size();
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j ++) {
            treeCsv.setFloat(i, j, metaProbTree[i][j].prob);
        }
    }
    treeCsv.saveFile(ofToDataPath(tempFileName));
    
    // THEN, copy the vocab list into a csv and save the file
    tempFileName = "TREE_VOCAB " + T_file_name + ".csv";
    vocabCsv.createFile(ofToDataPath(tempFileName));
    
    int nVocab = vocab.size();
    for (int i = 0; i < nVocab; i++) {
        vocabCsv.setString(0, i, vocab[i]);
    }
    vocabCsv.saveFile(ofToDataPath(tempFileName));
    
}

//---------------------------------------------------------------------

// add new row and column to metaProbTree
void textGen::growTree() {
    metaProb newMetaProb;
    
    // get number of rows and add metaProb to end of each one
    int nRows = metaProbTree.size();
    for (int r = 0; r < nRows; r++) {
        metaProbTree[r].push_back(newMetaProb);
    }
    
    // reflects new number of columns
    int nCols = metaProbTree[0].size();
    vector<metaProb> newRow(nCols);
    metaProbTree.push_back(newRow); // add new row to bottom
}

//---------------------------------------------------------------------


// draw connections between words and points
//        ofSetColor(ofColor::red);
//        ofSetLineWidth(3);
//        ofLine(closestPoint, thisPoint);
//        ofSetColor(ofColor::black);
//        ofCircle(closestPoint, 4);
//        ofSetColor(ofColor::red);
//        ofCircle(thisPoint, 5);
//        ofSetLineWidth(1);