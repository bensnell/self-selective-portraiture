//
//  dataPt.cpp
//  SSPortraiture_2_Analysis
//
//  Created by Ben Snell on 10/20/15.
//
//

#include "dataPt.h"

// constructor for one or more values
dataPt::dataPt (int n) {
    
    // number of values if n is passed in
    nValues = n;
    
    // declare new array
    // (new arrays contain random bits of memory in the double type to clear a new array, must assign all values within to zero)
    values = new double[nValues];
    
    // assign all values to zero
    for (int i=0; i<nValues; i++){
        values[i] = 0.0;
    }
}