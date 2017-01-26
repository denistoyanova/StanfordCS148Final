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
#include "SimpleImage.h"

#define PI 3.14159265
//landscape size needs to be a power of 2 + 1
#define LANDSCAPE_SIZE 65
#define CORNER_1 0
#define CORNER_2 0
#define CORNER_3 0
#define CORNER_4 10

std::vector< std::vector<float> > landscape(LANDSCAPE_SIZE, std::vector<float>(LANDSCAPE_SIZE));

struct Vector3{
    float x,y,z;
    Vector3(float _x, float _y, float _z){
        x = _x; y = _y; z = _z;
    }
    Vector3(){
        x = 0; y = 0; z = 0;
    }
    Vector3 operator-(Vector3 v){
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    Vector3 operator+(Vector3 v){
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
};

Vector3 cross(Vector3 v1, Vector3 v2){
    return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

Vector3 normalize(Vector3 v){
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return Vector3(v.x / length, v.y / length, v.z / length);
}

Vector3 normal[LANDSCAPE_SIZE][LANDSCAPE_SIZE];

void getLandNormal(){
    for(int i = 1; i < LANDSCAPE_SIZE - 1; i++){
        for(int j = 1; j < LANDSCAPE_SIZE - 1; j++){
            Vector3 deltaX = Vector3(float(i + 1), float(j), landscape[i+1][j]) - Vector3(float(i -1), float(j), landscape[i-1][j]);
            Vector3 deltaY = Vector3(float(i), float(j+1), landscape[i][j+1]) - Vector3(float(i), float(j-1), landscape[i][j-1]);
            normal[i][j] = normalize(cross(deltaX, deltaY));
            
        }
    }
    for(int j = 1; j < LANDSCAPE_SIZE - 1; j++){
        normal[0][j] = normal[1][j];
        normal[LANDSCAPE_SIZE - 1][j] = normal[LANDSCAPE_SIZE - 2][j];
    }
    for(int i = 0; i < LANDSCAPE_SIZE; i++){
        normal[i][0] = normal[i][1];
        normal[i][LANDSCAPE_SIZE-1] = normal[i][LANDSCAPE_SIZE-2];
    }
    return;
}
