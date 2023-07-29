#ifndef _IFS_H_
#define _IFS_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<vector>
#include "matrix.h"
#include "image.h"

class IFS{
private:
    int num_transformations;//a field to store n, the number of transformations
    Matrix *transformations;//an array of matrices representing the n transformations
    float *probabilities;//an array of the corresponding probabilities for choosing a transformation

public:
    //a constructor that creates an IFS
    IFS(){
        num_transformations = 0;
    }
    //a destructor that frees the memory of the various arrays (using delete)
    ~IFS(){
        delete[] transformations;
        delete[] probabilities;
    }

    //an input method that reads the IFS description
    void read_description(const char* filename);
    //a render method that takes as input an image instance, a number of points and a number of iterations
    void render(Image& img, int num_points, int num_iters);

};

#endif