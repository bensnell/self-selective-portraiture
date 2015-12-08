//
//  textBlock.h
//  SSPortraiture_1_Recording_XL
//
//  Created by Ben Snell on 10/19/15.
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

#ifndef __SSPortraiture_1_Recording_XL__textBlock__
#define __SSPortraiture_1_Recording_XL__textBlock__

#include <ofMain.h>

class wordBlock {
    
public:
    string          rawWord;
    float           width;
    float           height;
    ofColor         color;
    
    // word and space position (Added by Ben Snell)
    float wordX;
    float wordY;
};


class lineBlock {
    
public:
    vector<int>   wordsID;
    
    float   width;
    float   height;
    
    // line position (Added by Ben Snell)
    float lineX;
    float lineY;
};

//Just a helpful set of enumerated constants.
enum TextBlockAlignment { OF_TEXT_ALIGN_LEFT, OF_TEXT_ALIGN_RIGHT, OF_TEXT_ALIGN_JUSTIFIED, OF_TEXT_ALIGN_CENTER };

class textBlock {
    
public:
    textBlock();
    virtual ~textBlock();
    
    string          rawText;
    ofTrueTypeFont  defaultFont;
    wordBlock       blankSpaceWord;
    float           scale;
    
    // paragraph position (Added by Ben Snell)
    float paragraphX;
    float paragraphY;
    
    vector<wordBlock>   words;
    vector<lineBlock>   lines;
    
    void    init(string fontLocation, float fontSize);
    
    void    setText(string _inputText);
    
    // function to fill the x and y coordinates of words without actually drawing to screen the text
    void ghostDrawLeft(float x, float y); // Added by Ben Snell
    
    int     wrapTextX(float lineWidth); //Returns the number of lines it formed.
    void    wrapTextArea(float rWidth, float rHeight);
    bool    wrapTextForceLines(int linesN);
    
    void    setLineHeight(float lineHeight);
    void    setColor(int r, int g, int b, int a);
    
    void    draw(float x, float y); //Draws left align.
    void    drawLeft(float x, float y);
    void    drawRight(float x, float y);
    void    drawCenter(float x, float y);
    void    drawJustified(float x, float y, float boxWidth);
    
    
    void    forceScale(float _scale);
    
    float   getWidth();
    float   getHeight();
    
    //    protected:
    
    void    _loadWords();
    
    void    _trimLineSpaces();
    float   _getWidthOfWords();
    int     _getLinedWords();
    
    //    private:
};

#endif /* defined(__SSPortraiture_1_Recording_XL__textBlock__) */