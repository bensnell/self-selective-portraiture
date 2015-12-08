//
//  textBlock.cpp
//  SSPortraiture_3_Generation
//
//  Modified by Ben Snell on 10/19/15.
//
//  Heavily borrowed from Luke Malcom and only slightly edited to include
//  ability to store x and y position of each word
//
//  Note: not all functionality remains. Only by using the default justificaiton will the appropriate reference be filled.
//

/***********************************************************************
 
 Copyright (c) 2009, Luke Malcolm, www.lukemalcolm.com
 
 Download the original ofxAddon "ofxTextSuite" here: https://github.com/lukemalcolm/ofxTextSuite
 
 ***********************************************************************/

#include "textBlock.h"

textBlock::textBlock() {
    
    scale       =   1.0f;
}

textBlock::~textBlock() {
    //dtor
}

void textBlock::init(string fontLocation, float fontSize){
    
    defaultFont.loadFont(fontLocation, fontSize, true, true);
    
    //Set up the blank space word
    blankSpaceWord.rawWord = " ";
    blankSpaceWord.width   = defaultFont.stringWidth ("x");
    blankSpaceWord.height  = defaultFont.stringHeight("i");
    blankSpaceWord.color.r = blankSpaceWord.color.g = blankSpaceWord.color.b = 255;
}

void textBlock::setText(string _inputText) {
    rawText     = _inputText;
    _loadWords();
    wrapTextForceLines(1);
}

void textBlock::draw(float x, float y) {
    
    drawLeft(x, y);
    
}

void textBlock::drawLeft(float x, float y) {
    
    string  strToDraw;
    int     currentWordID;
    float   drawX;
    float   drawY;
    
    float currX = 0;
    
    if (words.size() > 0) {
        
        for(int l=0;l < lines.size(); l++)
        {
            for(int w=0;w < lines[l].wordsID.size(); w++)
            {
                currentWordID = lines[l].wordsID[w];
                
                drawX = x + currX;
                drawY = y + (defaultFont.getLineHeight() * (l + 1));
                
                // Added by Ben Snell
                // set word position
                words[currentWordID].wordX = drawX;
                words[currentWordID].wordY = drawY;
                
                // set line position
                if (w == 0) {
                    lines[l].lineX = drawX;
                    lines[l].lineY = drawY;
                }
                
                ofSetColor(words[currentWordID].color.r, words[currentWordID].color.g, words[currentWordID].color.b, words[currentWordID].color.a);
                glPushMatrix();
                //glTranslatef(drawX, drawY, 0.0f);
                glScalef(scale, scale, scale);
                
                defaultFont.drawString(words[currentWordID].rawWord.c_str(), drawX, drawY);
                currX += words[currentWordID].width;
                
                glPopMatrix();
            }
            currX = 0;
        }
    }
}


// Added by Ben Snell
void textBlock::ghostDrawLeft(float x, float y) {
    
    string  strToDraw;
    int     currentWordID;
    float   drawX;
    float   drawY;
    
    float currX = 0;
    
    if (words.size() > 0) {
        
        for(int l=0;l < lines.size(); l++)
        {
            for(int w=0;w < lines[l].wordsID.size(); w++)
            {
                currentWordID = lines[l].wordsID[w];
                
                drawX = x + currX;
                drawY = y + (defaultFont.getLineHeight() * (l + 1));
                
                // Added by Ben Snell
                // set word position
                words[currentWordID].wordX = drawX;
                words[currentWordID].wordY = drawY;
                
                // set line position
                if (w == 0) {
                    lines[l].lineX = drawX;
                    lines[l].lineY = drawY;
                }
                
                ofSetColor(words[currentWordID].color.r, words[currentWordID].color.g, words[currentWordID].color.b, words[currentWordID].color.a);
                glPushMatrix();
                //glTranslatef(drawX, drawY, 0.0f);
                glScalef(scale, scale, scale);
                
                // DON'T DRAW ANYTHING! That's the point!
                
                currX += words[currentWordID].width;
                
                glPopMatrix();
            }
            currX = 0;
        }
    }
}


void textBlock::drawCenter(float x, float y){
    
    string  strToDraw;
    int     currentWordID;
    float   drawX;
    float   drawY;
    float   lineWidth;
    
    float currX = 0;
    
    if (words.size() > 0) {
        
        for(int l=0;l < lines.size(); l++)
        {
            
            //Get the length of the line.
            lineWidth = 0;
            for(int w=0;w < lines[l].wordsID.size(); w++)
            {
                currentWordID = lines[l].wordsID[w];
                lineWidth += words[currentWordID].width;
            }
            
            for(int w=0;w < lines[l].wordsID.size(); w++)
            {
                currentWordID = lines[l].wordsID[w];
                
                drawX = -(lineWidth / 2) + currX;
                drawY = defaultFont.getLineHeight() * (l + 1);
                
                ofSetColor(words[currentWordID].color.r, words[currentWordID].color.g, words[currentWordID].color.b, words[currentWordID].color.a);
                
                glPushMatrix();
                
                //Move to central point using pre-scaled co-ordinates
                glTranslatef(x, y, 0.0f);
                
                glScalef(scale, scale, scale);
                
                defaultFont.drawString(words[currentWordID].rawWord.c_str(), drawX, drawY);
                currX += words[currentWordID].width;
                
                glPopMatrix();
                
            }
            currX = 0;
            
        }
    }
}

void textBlock::drawJustified(float x, float y, float boxWidth){
    
    string  strToDraw;
    int     currentWordID;
    float   drawX;
    float   drawY;
    int     spacesN;
    float   nonSpaceWordWidth;
    float   pixelsPerSpace;
    
    float currX = 0;
    
    if (words.size() > 0) {
        
        
        for(int l=0;l < lines.size(); l++)
        {
            //Find number of spaces and width of other words;
            spacesN = 0;
            nonSpaceWordWidth = 0;
            
            for(int w=0;w < lines[l].wordsID.size(); w++)
            {
                currentWordID = lines[l].wordsID[w];
                if (words[currentWordID].rawWord == " ") spacesN++;
                else nonSpaceWordWidth += words[currentWordID].width;
            }
            
            pixelsPerSpace = ((boxWidth / scale) - (x / scale) - nonSpaceWordWidth) / spacesN;
            
            for(int w=0;w < lines[l].wordsID.size(); w++)
            {
                currentWordID = lines[l].wordsID[w];
                
                drawX = currX;
                drawY = defaultFont.getLineHeight() * (l + 1);
                
                ofSetColor(words[currentWordID].color.r, words[currentWordID].color.g, words[currentWordID].color.b, words[currentWordID].color.a);
                glPushMatrix();
                //Move to top left point using pre-scaled co-ordinates
                glTranslatef(x, y, 0.0f);
                glScalef(scale, scale, scale);
                
                if (words[currentWordID].rawWord != " ") {
                    defaultFont.drawString(words[currentWordID].rawWord.c_str(), drawX, drawY);
                    currX += words[currentWordID].width;
                }
                else {
                    currX += pixelsPerSpace;
                }
                
                
                glPopMatrix();
                
            }
            currX = 0;
            
        }
    }
}

void textBlock::drawRight(float x, float y){
    
    string  strToDraw;
    int     currentWordID;
    float   drawX;
    float   drawY;
    
    float currX = 0;
    
    if (words.size() > 0) {
        
        for(int l=0;l < lines.size(); l++)
        {
            
            for(int w=lines[l].wordsID.size() - 1; w >= 0; w--)
            {
                
                currentWordID = lines[l].wordsID[w];
                
                drawX = -currX - words[currentWordID].width;
                drawY = defaultFont.getLineHeight() * (l + 1);
                
                ofSetColor(words[currentWordID].color.r, words[currentWordID].color.g, words[currentWordID].color.b, words[currentWordID].color.a);
                
                glPushMatrix();
                
                //Move to top left point using pre-scaled co-ordinates
                glTranslatef(x, y, 0.0f);
                glScalef(scale, scale, scale);
                
                defaultFont.drawString(words[currentWordID].rawWord.c_str(), drawX, drawY);
                currX += words[currentWordID].width;
                
                glPopMatrix();
                
            }
            currX = 0;
            
        }
    }
}

void textBlock::_trimLineSpaces()
{
    if (words.size() > 0) {
        //Now delete all leading or ending spaces on each line
        for(int l=0;l < lines.size(); l++)
        {
            //Delete the first word if it is a blank
            if (lines[l].wordsID.size() > 0){
                if (words[lines[l].wordsID[0]].rawWord == " ")   lines[l].wordsID.erase(lines[l].wordsID.begin());
            }
            
            //Delete the last word if it is a blank
            if (lines[l].wordsID.size() > 0){
                if (words[lines[l].wordsID[lines[l].wordsID.size() - 1]].rawWord == " ") lines[l].wordsID.erase(lines[l].wordsID.end() - 1);
            }
        }
    }
    
}

int textBlock::_getLinedWords(){
    
    int wordCount = 0;
    
    if (words.size() > 0) {
        
        for(int l=0;l < lines.size(); l++)
        {
            wordCount += lines[l].wordsID.size();
        }
        return wordCount;
    }
    else return 0;
}

void textBlock::wrapTextArea(float rWidth, float rHeight){
    
    float tmpScale = 0.0f;
    float maxIterations = _getLinedWords();
    float scales[1000];
    scale = 1.0f;  //Reset the scale for the height and width calculations.
    
    if (words.size() > 0) {
        
        //Check each possible line layout and check it will fit vertically
        for (int iteration=1; iteration <= maxIterations; iteration++){
            
            //printf("Iteration %i...\n", iteration);
            wrapTextForceLines(iteration);
            
            tmpScale = rWidth / getWidth();
            if ((tmpScale * getHeight()) < rHeight) {
                scales[iteration] = tmpScale;
            }
            else {
                scales[iteration] = -1;
            }
        }
        
        //Now see which is biggest
        int maxIndex = 1;
        bool bScaleAvailable = false;
        
        for (int i=1; i <= maxIterations; i++) {
            ofLog(OF_LOG_VERBOSE,"Scales %i = %f\n", i, scales[maxIndex]);
            if (scales[i] != -1) bScaleAvailable = true;
            
            if (scales[i] > scales[maxIndex]) {
                maxIndex = i;
            }
        }
        
        //When only one line is needed an appropriate on the Y scale can sometimes not be found.  In these occasions scale the size to the Y dimension
        if (bScaleAvailable) {
            scale = scales[maxIndex];
        }
        else {
            scale = (float)rHeight / (float)getHeight();
        }
        
        float persistScale = scale; //Need to persist the scale as the wrapTextForceLines will overwrite.
        wrapTextForceLines(maxIndex);
        scale = persistScale;
        
        ofLog(OF_LOG_VERBOSE,"Scaling with %i at scale %f...\n", maxIndex, scale);
    }
    
    
}


bool textBlock::wrapTextForceLines(int linesN){
    
    if (words.size() > 0) {
        
        if (linesN > words.size()) linesN = words.size();
        
        float lineWidth = _getWidthOfWords() * (1.1f / (float)linesN);
        
        int curLines = 0;
        bool bGotLines = false;
        
        //keep increasing the line width until we get the desired number of lines.
        while (!bGotLines) {
            
            curLines = wrapTextX(lineWidth);
            if (curLines == linesN) return true;
            if (curLines > linesN) return false;
            lineWidth-=10;
            
        }
        
    }
    
}


int textBlock::wrapTextX(float lineWidth){
    
    scale = 1.0f;
    
    if (words.size() > 0) {
        
        float   runningWidth = 0.0f;
        
        lines.clear();
        
        bool        newLine = true;
        lineBlock   tmpLine;
        tmpLine.wordsID.clear();
        int         activeLine = 0;
        
        for(int i=0;i < words.size(); i++)
        {
            
            runningWidth += words[i].width;
            
            if (runningWidth <= lineWidth) {
                newLine = false;
            }
            else {
                
                newLine = true;
                lines.push_back(tmpLine);
                tmpLine.wordsID.clear();
                runningWidth = 0.0f + words[i].width;;
                activeLine++;
            }
            
            tmpLine.wordsID.push_back(i);
        }
        
        //Push in the final line.
        lines.push_back(tmpLine);
        _trimLineSpaces(); //Trim the leading and trailing spaces.
        
    }
    
    return lines.size();
    
}

void textBlock::_loadWords(){
    
    wordBlock tmpWord;
    
    istringstream iss(rawText);
    
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(tokens));
    
    words.clear();
    
    for(int i=0;i < tokens.size(); i++)
    {
        tmpWord.rawWord = tokens.at(i);
        tmpWord.width   = defaultFont.stringWidth(tmpWord.rawWord);
        tmpWord.height  = defaultFont.stringHeight(tmpWord.rawWord);
        tmpWord.color.r = tmpWord.color.g = tmpWord.color.b = 150;
        words.push_back(tmpWord);
        //add spaces into the words vector if it is not the last word.
        if (i != tokens.size()) words.push_back(blankSpaceWord);
    }
    
    for(int i=0;i < words.size(); i++)
    {
        ofLog(OF_LOG_VERBOSE, "Loaded word: %i, %s\n", i, words[i].rawWord.c_str());
    }
    
    
}

float textBlock::_getWidthOfWords(){
    
    float widthTotal = 0.0f;
    
    if (words.size() > 0) {
        for(int i=0;i < words.size(); i++)
        {
            widthTotal += words[i].width;
            
        }
        return widthTotal;
    }
    else {
        return 0.0f;
    }
    
}

float textBlock::getWidth(){
    
    int   currentWordID;
    
    float currX = 0.0f;
    float maxWidth  = 0.0f;
    
    if (words.size() > 0) {
        
        for(int l=0;l < lines.size(); l++)
        {
            for(int w=0;w < lines[l].wordsID.size(); w++)
            {
                currentWordID = lines[l].wordsID[w];
                currX += words[currentWordID].width;
            }
            maxWidth = MAX(maxWidth, currX);
            currX = 0.0f;
        }
        return maxWidth * scale;
    }
    else return 0;
    
}

float textBlock::getHeight(){
    
    if (words.size() > 0) {
        return defaultFont.getLineHeight() * scale * lines.size();
    }
    else return 0;
    
}

void textBlock::setLineHeight(float lineHeight){
    
    defaultFont.setLineHeight(lineHeight);
    
}

void textBlock::setColor(int r, int g, int b, int a){
    
    ofColor tmpColor;
    tmpColor.r = r;
    tmpColor.g = g;
    tmpColor.b = b;
    tmpColor.a = a;
    
    if (words.size() > 0) {
        for(int i=0;i < words.size(); i++)
        {
            words[i].color = tmpColor;
            
        }
    }
    
}

void textBlock::forceScale(float _scale){
    scale = _scale;
}


/*
 textBlock Class Reference (additional functionality)
 
 int sizeOfWordArray = inputText.words.size(); // total # words and spaces
 //    cout << ofToString(sizeOfWordArray) << endl;
 for (int i = 0; i < sizeOfWordArray; i ++) {
 if (inputText.words[i].rawWord.compare(" ") != 0) {
 // just print the words (not spaces)
 string thisWord = inputText.words[i].rawWord;  // word at i
 int thisWidth = inputText.words[i].width;      // pixel width of word
 int thisHeight = inputText.words[i].height;    // pixel height of word
 //            cout << "word " << ofToString(i) << ": " << ofToString(thisWord);
 //            cout << "   width: " << ofToString(thisWidth);
 //            cout << "   height: " << ofToString(thisHeight) << endl;
 }
 }
 
 int sizeOfLineArray = inputText.lines.size(); // total # of lines
 //    cout << ofToString(sizeOfLineArray) << endl;
 for (int i = 0; i < sizeOfLineArray; i++) {
 int nWords = inputText.lines[i].wordsID.size();    // # words on line
 //        float lineWidth = myText.lines[i].width;          // line width
 //        float lineHeight = myText.lines[i].height;        // line height
 //        cout << ofToString(lineWidth) << "  " << ofToString(lineHeight) << endl;
 for (int j = 0; j < nWords; j++) {
 int wordID = inputText.lines[i].wordsID[j]; // index of word in words array
 //            cout << myText.words[j].rawWord << endl;
 }
 }
 */


/*
 Regular TrueTypeFont Functions:
 
 ofBuffer textBuffer;
 ofTrueTypeFont openSans;
 string text;
 
 //--------------------------------
 
 //    ofTrueTypeFont::setGlobalDpi(72);
 
 // load buffer
 textBuffer = ofBufferFromFile(file_name);
 
 // get the text to display
 text = textBuffer.getNextLine();
 
 // load font and style
 openSans.loadFont("open_sans_regular.ttf", 10);
 openSans.setLineHeight(18.0f);
 //    openSans.setLetterSpacing(2.037);
 
 ofSetColor(0);
 openSans.drawString(text, 50, 50);
 
 // get size of a string
 ofRectangle rect = openSans.getStringBoundingBox(text, 0, 0);   // rect
 int stringW = openSans.stringWidth(text);                       // just width
 int stringH = openSans.stringHeight(text);                      // just height
 */