#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h> 

//Struct to hold cube coordinate information for printing
typedef struct coordinates {
    int **coordsArray;
    int length;
    //int minX;
    int maxY;
    int cubeLength;
} Coordinates;

//Printing functions
void animateSolid(int solidType, int length, int seconds, int fps);
void printSolid(int solidType, int length, float xTheta, float yTheta, float zTheta);
void printToTerminal(Coordinates *coords);

//Form shapes
float ** buildCube(int length);
float ** buildSolid(float **cube, int solidType, int length, int *numVertices);

//Manipulate shapes
float ** rotateSolid(float ** cube, int length, int numVertices, float xTheta, float yTheta, float zTheta);
int ** getVerticesCoordinates(float ** cube, int length, int numVertices);
Coordinates * generateCoordinates(int **screen, int solidType, int numVertices);
void getLineCoordinates(int x1, int y1, int x2, int y2, Coordinates *coords);
int getSpace(int length);

//free() malloc'd arrays
void freeCube(float **cube);
void freeSolid(float **solid, int numVertices);
void freeScreen(int **screen, int numVertices);
void freeCoordinates(Coordinates *coords);

//Quicksort functions
void quicksort(int **coordsArray, int low, int high);
int partition(int **coordsArray, int low, int high);
void swap(int **coordsArray, int a, int b);