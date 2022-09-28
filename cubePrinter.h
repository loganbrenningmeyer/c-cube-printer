#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h> 


//Struct to hold cube coordinate information for printing
typedef struct coordinates {
    int **coordsArray;
    int length;
    int minX;
    int maxY;
    int cubeLength;
} Coordinates;

//Main functions
void animateCube();
void pauseFrame(int ms);
void printCube(int length, float xTheta, float yTheta, float zTheta);
float ** buildCube(int length);
float ** rotateCube(float ** cube, int length, float xTheta, float yTheta, float zTheta);
int ** getVerticesCoordinates(float ** cube, int length);
Coordinates * generateCoordinates(int **screen);
void getLineCoordinates(int x1, int y1, int x2, int y2, Coordinates *coords);
void printToTerminal(Coordinates *coords);
int getSpace(int length);

//free() malloc'd arrays
void freeCube(float **cube);
void freeScreen(int **screen);
void freeCoordinates(Coordinates *coords);

//Quicksort functions
void quicksort(int **coordsArray, int low, int high);
int partition(int **coordsArray, int low, int high);
void swap(int **coordsArray, int a, int b);