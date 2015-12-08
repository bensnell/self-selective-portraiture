//
//  dataPt.h
//  SSPortraiture_3_Generation
//
//  Created by Ben Snell on 10/19/15.
//
//

#ifndef __SSPortraiture_3_Generation__dataPt__
#define __SSPortraiture_3_Generation__dataPt__

#include "ofMain.h"

// class for a data point (with any number of values)
class dataPt {
    
public:
    
    int nValues;
    
    dataPt(int n);
    
    double time;
    string type;
    double *values;
    // include avg?
};

#endif /* defined(__SSPortraiture_3_Generation__dataPt__) */
