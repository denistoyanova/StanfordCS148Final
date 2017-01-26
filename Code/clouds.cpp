#include "clouds.h"
#include "main.h"
#ifdef WIN32
#define ssize_t SSIZE_T
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "SimpleImage.h"

#define CHANGINGSPEED 0.05
#define LARGEGRID 100
#define SMALLGRID 1
#define WIDTH 2000
#define HEIGHT 1500

#define SQUARELENGTH 500

using namespace std;

float tempgrid[SQUARELENGTH/LARGEGRID+1][SQUARELENGTH/LARGEGRID+1];
//float skyColorRed = 0.1, skyColorGreen = 0.137, skyColorBlue = 0.377;
float skyColorRed = 0.643, skyColorGreen = 0.314, skyColorBlue = 0.235;
inline float interpolate(int loc, int unitgrid, float start, float end){
    float percentage = (float)loc / (float)unitgrid;
    return percentage * start + (1 - percentage) * end;
}


void Cloud::drawButtom(){
    for(int i = 0; i < SQUARELENGTH; i++){
        for(int j = 0; j < SQUARELENGTH; j++){
            skytext.set(i + SQUARELENGTH, j + SQUARELENGTH*2, RGBColor(0.0, 1.0, 0.0));
        }
    }
}

inline float gaussrand(float stddev, float mean)
{
    static double V1, V2, S;
    static int phase = 0;
    double X;
    
    if ( phase == 0 ) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;
            
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);
        
        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);
    
    phase = 1 - phase;
    
    float temp = (float)(X * stddev + mean);
    if(temp < 0.0) return 0.0;
    else return temp;
}



void Cloud::drawTop(){
    float pixel[SQUARELENGTH][SQUARELENGTH];
    float grid[SQUARELENGTH/LARGEGRID+1][SQUARELENGTH/LARGEGRID+1];
    srand(time(NULL));
    for(int i = 0; i < SQUARELENGTH/LARGEGRID+1; i++){
        for(int j = 0; j < SQUARELENGTH/LARGEGRID+1; j++){
            grid[i][j] = gaussrand(0.25, 0.1);
            tempgrid[i][j] = grid[i][j];
        }
    }
    float up,down;
    for(int i = 0; i < SQUARELENGTH/LARGEGRID ; i++){
        for(int j = 0; j < SQUARELENGTH/LARGEGRID ; j++){
            for(int x = 0; x < LARGEGRID; x++){
                up = interpolate(x, LARGEGRID,grid[i][j], grid[i+1][j]);
                down = interpolate(x, LARGEGRID,grid[i][j+1], grid[i+1][j+1]);
                for(int y = 0; y < LARGEGRID; y++){
                    
                    pixel[i * LARGEGRID + LARGEGRID - x - 1][j * LARGEGRID + LARGEGRID - y - 1] = interpolate(y, LARGEGRID,up, down);
                }
            }
        }
    }
    
    float grids[SQUARELENGTH/SMALLGRID+1][SQUARELENGTH/SMALLGRID+1];
    for(int i = 0; i < SQUARELENGTH/SMALLGRID+1; i++){
        for(int j = 0; j < SQUARELENGTH/SMALLGRID+1; j++){
            grids[i][j] = gaussrand(0.05, 0.1);
        }
    }
    for(int i = 0; i < SQUARELENGTH/SMALLGRID ; i++){
        for(int j = 0; j < SQUARELENGTH/SMALLGRID ; j++){
            for(int x = 0; x < SMALLGRID; x++){
                up = interpolate(x, SMALLGRID,grids[i][j], grids[i+1][j]);
                down = interpolate(x, SMALLGRID,grids[i][j+1], grids[i+1][j+1]);
                for(int y = 0; y < SMALLGRID; y++){
                    
                    pixel[i * SMALLGRID + SMALLGRID - x - 1][j * SMALLGRID + SMALLGRID - y - 1] += interpolate(y, SMALLGRID,up, down);
                }
            }
        }
    }
    
    
    for(int i = 0; i < SQUARELENGTH; i++){
        for(int j = 0; j <SQUARELENGTH;j++){
            if(pixel[i][j] > 0.7) pixel[i][j] = 0.7;
            float red =pixel[i][j] + skyColorRed;
            float green =pixel[i][j] + skyColorGreen;
            float blue = pixel[i][j] + skyColorBlue;
            if(red > 1.0) red = 1.0;
            if(green > 1.0) green = 1.0;
            if(blue > 1.0) blue = 1.0;
            skytext.set(i + SQUARELENGTH, j , RGBColor(red, green, blue));
        }
    }
}

void Cloud::drawSide(){
    float pixel[SQUARELENGTH * 4][SQUARELENGTH];
    float grid[SQUARELENGTH/LARGEGRID * 4+1][SQUARELENGTH/LARGEGRID+1];
    
    srand(time(NULL));
    for(int i = 0; i < SQUARELENGTH/LARGEGRID * 4 + 1; i++){
        for(int j = 0; j < SQUARELENGTH/LARGEGRID+1; j++){
            grid[i][j] = gaussrand(0.25, 0.1);
        }
    }
    
    for(int i = 0; i < SQUARELENGTH/LARGEGRID + 1; i++){
        grid[i][0] = tempgrid[0][i];
        grid[i + SQUARELENGTH/LARGEGRID][0] = tempgrid[i][SQUARELENGTH/LARGEGRID];
        grid[i + SQUARELENGTH/LARGEGRID * 2][0] = tempgrid[SQUARELENGTH/LARGEGRID][SQUARELENGTH/LARGEGRID - i];
        grid[i + SQUARELENGTH/LARGEGRID * 3][0] = tempgrid[SQUARELENGTH - i][0];
    }
    
    float up,down;
    for(int i = 0; i < SQUARELENGTH/LARGEGRID * 4 ; i++){
        for(int j = 0; j < SQUARELENGTH/LARGEGRID ; j++){
            for(int x = 0; x < LARGEGRID; x++){
                up = interpolate(x, LARGEGRID,grid[i][j], grid[i+1][j]);
                down = interpolate(x, LARGEGRID,grid[i][j+1], grid[i+1][j+1]);
                for(int y = 0; y < LARGEGRID; y++){
                    
                    pixel[i * LARGEGRID + LARGEGRID - x - 1][j * LARGEGRID + LARGEGRID - y - 1] = interpolate(y, LARGEGRID,up, down);
                }
            }
        }
    }
    
    float grids[SQUARELENGTH/SMALLGRID * 4 +1][SQUARELENGTH/SMALLGRID+1];
    for(int i = 0; i < SQUARELENGTH/SMALLGRID * 4+1; i++){
        for(int j = 0; j < SQUARELENGTH/SMALLGRID+1; j++){
            grids[i][j] = gaussrand(0.05, 0.1);
        }
    }
    for(int i = 0; i < SQUARELENGTH/SMALLGRID * 4 ; i++){
        for(int j = 0; j < SQUARELENGTH/SMALLGRID ; j++){
            for(int x = 0; x < SMALLGRID; x++){
                up = interpolate(x, SMALLGRID,grids[i][j], grids[i+1][j]);
                down = interpolate(x, SMALLGRID,grids[i][j+1], grids[i+1][j+1]);
                for(int y = 0; y < SMALLGRID; y++){
                    
                    pixel[i * SMALLGRID + SMALLGRID - x - 1][j * SMALLGRID + SMALLGRID - y - 1] += interpolate(y, SMALLGRID,up, down);
                }
            }
        }
    }
    for(int i = 0; i < SQUARELENGTH * 4; i++){
        for(int j = 0; j < SQUARELENGTH; j++){
            if(pixel[i][j] > 0.7) pixel[i][j] = 0.7;
            float offset = 1.0 -1.0/(1.0+exp(-((float)j-200.0)/50));
            float red =pixel[i][j]* offset + skyColorRed * (1.0 + float(j)/SQUARELENGTH) ;
            float green =pixel[i][j] * offset + skyColorGreen* (1.0 + float(j)/SQUARELENGTH);
            float blue = pixel[i][j] * offset + skyColorBlue* (1.0 + float(j)/SQUARELENGTH);
            if(red > 1.0) red = 1.0;
            if(green > 1.0) green = 1.0;
            if(blue > 1.0) blue = 1.0;
            skytext.set(i, j + SQUARELENGTH, RGBColor(red, green, blue));
        }
    }
    

}

void Cloud::genCloud(){
        drawButtom();
        drawTop();
        drawSide();
        skytext.save("sky.png");
}

Cloud::Cloud(){
    skytext = SimpleImage(WIDTH, HEIGHT, RGBColor(0,0,0));
    
}




