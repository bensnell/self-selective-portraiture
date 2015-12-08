//
//  dataPt.h
//  SSPortraiture_2_Analysis
//
//  Created by Ben Snell on 10/20/15.
//
//

#ifndef __SSPortraiture_2_Analysis__dataPt__
#define __SSPortraiture_2_Analysis__dataPt__

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
#endif /* defined(__SSPortraiture_2_Analysis__dataPt__) */
