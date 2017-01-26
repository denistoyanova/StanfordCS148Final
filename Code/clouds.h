#ifndef ____clouds__
#define ____clouds__

#include <stdio.h>
#include "main.h"
#ifdef WIN32
#define ssize_t SSIZE_T
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cmath>
#include <random>
#include "SimpleImage.h"


class Cloud{
public:
    SimpleImage skytext;
    

    Cloud();
    
    void drawButtom();
    void drawTop();
    void drawSide();
    
    void genCloud();
    
};

#endif
