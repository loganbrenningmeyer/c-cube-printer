#include "cubePrinter.h"

//Declare PI
#define PI 3.141593
//Declare square root of 3
#define root_3 1.732051

//Animates cube based on given starting orientation, 
//ending orientation, cube length, duration, and fps
void animateCube(int length, int seconds, int fps) {

    srand(time(NULL));

    //Calculate # of frames
    int frames = seconds * fps;

    //Variables for starting/ending orientation
    float xTheta, yTheta, zTheta, xFinal, yFinal, zFinal;

    //Set random starting and ending orientations in the range [-360, 360]
    xTheta = rand() % 721 - 360, yTheta = rand() % 721 - 360, zTheta = rand() % 721 - 360;
    xFinal = rand() % 721 - 360, yFinal = rand() % 721 - 360, zFinal = rand() % 721 - 360;

    //Calculate amount to increment for each axis per frame
    float xInc = (float)(xFinal - xTheta)/frames, 
          yInc = (float)(yFinal - yTheta)/frames, 
          zInc = (float)(zFinal - zTheta)/frames;

    //Loops for each frame in the animation
    for (int i = 0; i < frames; i++) {
        //Per frame, print the cube then adjust orientation
        printCube(length, xTheta, yTheta, zTheta);
        xTheta += xInc, yTheta += yInc, zTheta += zInc;
        //Hold the frame for 1/fps seconds
        //usleep((float)1/fps * 1000000);
        pauseFrame((int)((float)1/fps * 1000));
        //Clear the console for the next frame
        printf("\e[1;1H\e[2J");
    }
    //Print final cube
    printCube(length, xTheta, yTheta, zTheta);
}

void pauseFrame(int ms) {
    usleep(ms * 1000);
}

//Prints cube to terminal
void printToTerminal(Coordinates *coords) {
    int **coordsArray = coords->coordsArray;
    int cubeLength = coords->cubeLength;

    //Print new lines to shift cube down to origin
    //Full vertical space around cube is 2*space + cubeLength (*10 to correct 
    //for scaling in getVerticesCoordinates() when converting from float to int)
    //Print space equal to the difference of maxY and the full space
    for (int i = 0; i < (2 * getSpace(cubeLength) + cubeLength)*10 - coords->maxY; i++) {
        printf("\n");
    }

    //First point
    printf("%*c", 2*(coordsArray[0][0]-1), ' ');
    printf("██");

    for (int i = 1; i < coords->length; i++) {

        int x = coordsArray[i][0], xPrev = coordsArray[i-1][0];
        int y = coordsArray[i][1], yPrev = coordsArray[i-1][1];

        //If current point is the same as the last point, move on
        if (x == xPrev && y == yPrev) {
            //If the point is the last on its row, add a new line
            if (i == coords->length - 1 || coordsArray[i+1][1] != y) {
                printf("\n");
            }
            continue;
        }
        
        //If the point is the first on its row, space it over x-1 spaces
        if (y != yPrev) {
            if (x - 1 > 0) { 
                printf("%*c", 2*(x-1), ' ');
            }
        //Otherwise, space it over by the distance between it and the previous point in the row
        } else {
            if (x - xPrev > 1) {
                printf("%*c", 2*(x - xPrev - 1), ' ');
            }
        }

        //Print the point ██
        printf("██");

        //If the point is the last on its row, add a new line
        if (i == coords->length - 1 || coordsArray[i+1][1] != y) {
            printf("\n");
        }
    }
}

void printCube(int length, float xTheta, float yTheta, float zTheta) {
    //Initialize cube
    float **cube = buildCube(length);
    //Apply rotations
    rotateCube(cube, length, xTheta, yTheta, zTheta);
    //Find the 2D projected coordinates of the cube vertices
    int **screen = getVerticesCoordinates(cube, length);
    //Initialize coordinate struct pointer and generate all 2D cube coordinates
    Coordinates *coords = generateCoordinates(screen);
    coords->cubeLength = length;
    //Sort coordinate array for printing in the terminal
    quicksort(coords->coordsArray, 0, coords->length - 1);
    //Print cube to terminal
    printToTerminal(coords);
    //Free cube coordinates array and screen coordinates array
    freeCube(cube);
    freeScreen(screen);
    freeCoordinates(coords);
}

/*
Returns coordinates of a default cube in the first octant spaced (getSpace(length))
units outwards from the origin in every direction. Must allocate this space on the sides 
so the cube has enough room to rotate without going outside of the first octant
*/
float ** buildCube(int length) {
    //Declare initial cube coordinates array (8 vertices, 3 coordinates (x,y,z) each)
    float ** cube = malloc(sizeof(float *) * 8);
    //Initialize space variable
    int space = getSpace(length);

    for (int i = 0; i < 8; i++) {
        cube[i] = malloc(sizeof(float) * 3);
        for (int j = 0; j < 3; j++) {
            cube[i][j] = 0;
        }
    }  

    //Default coordinates of vertices for a cube in the first octant with cube[0] vertex at (0,0,0)
    cube[0][0] = 0,      cube[0][1] = 0,      cube[0][2] = 0;
    cube[1][0] = length, cube[1][1] = 0,      cube[1][2] = 0;
    cube[2][0] = length, cube[2][1] = length, cube[2][2] = 0;
    cube[3][0] = 0,      cube[3][1] = length, cube[3][2] = 0;
    cube[4][0] = 0,      cube[4][1] = 0,      cube[4][2] = length;
    cube[5][0] = length, cube[5][1] = 0,      cube[5][2] = length;
    cube[6][0] = length, cube[6][1] = length, cube[6][2] = length;
    cube[7][0] = 0,      cube[7][1] = length, cube[7][2] = length;

    //Shift cube over by space in all directions
    for (int i = 0; i < 8; i++) {
        cube[i][0] += space; cube[i][1] += space; cube[i][2] += space;
    }

    return cube;
}

/*
x-axis: x-values = y, y-values = z
y-axis: x-values = z, y-values = x
z-axis: x-values = x, y-values = y

origin: space + length/2
    - Amount of space between center of cube face and axis
    - Consider the center of the face of the cube as the origin for rotation,
    the variable "origin" represents the value that the real origin must be 
    shifted to make that true

Radius for each point: r = distance between point and axis of rotation about origin
    - Distance: sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
        - Whatever axis (x,y,z) that is being rotated is cancelled out because 
        the shortest distance between the point and the axis is where both axis values are the same
        - For x-rotation:
            - Distance: sqrt((y2 - y1)^2 + (z2 - z1)^2)
    - Axis of rotation about x-axis: <x, yOrig, zOrig>
        - Line of x-values with y = yOrig and z = zOrig

Alpha for each point: 
    - Find adjacent and opposite sides to alpha by taking
    x - origin and y - origin
    - alpha = atan(y/x)
        - If the point lies in Quadrant I in 2D, leave it
        - If the point is in QII or QIII, add PI
        - If the point is in QIV, add 2*PI
        - https://www.mathsisfun.com/polar-cartesian-coordinates.html 

(x', y'):
    - Add angle shift to alpha and correct by the origin shift
    - x' = rcos(alpha + theta) + origin
    - y' = rsin(alpha + theta) + origin
*/
float ** rotateCube(float **cube, int length, float xTheta, float yTheta, float zTheta) {
    //Convert theta from degrees to radians
    xTheta *= PI/180, yTheta *= PI/180, zTheta *= PI/180;

    //Center of the faces for rotation
    float origin = getSpace(length) + (float)length/2;

    //Apply x, y, and z rotation one point at a time
    for (int i = 0; i < 8; i++) {
        
        float x0 = cube[i][0], y0 = cube[i][1], z0 = cube[i][2];
        //Variables to be used in formula
        float x, y, z;
        float radius, alpha;
        //Calculate alpha (current radians counterclockwise from x-axis)

        //X Rotation
        if (xTheta != 0) {
            //For x-rotation, x = y, y = z
            x = y0, y = z0;

            //Calculate alpha (current radians counterclockwise from x-axis in 2D).
            //Find adjacent and opposite sides to alpha, then use atan() to find angle
            float alphaX = x - origin, alphaY = y - origin;

            //Calculate radius
            //Radius is the distance from the point to the line running through (x, origin, origin)
            radius = sqrt(pow(alphaX, 2) + pow(alphaY, 2));

            alpha = atan(alphaY/alphaX);

            //QII and QIII: Add PI
            if ((alphaX < 0 && alphaY >= 0) || (alphaX < 0 && alphaY < 0)) {
                alpha += PI;
            } 
            //QIV: Add 2*PI
            else if (alphaX >= 0 && alphaY < 0) {
                alpha += 2*PI;
            }

            cube[i][1] = radius * cos(alpha + xTheta) + origin;
            cube[i][2] = radius * sin(alpha + xTheta) + origin;

            //Reset x0, y0, z0 variables 
            x0 = cube[i][0], y0 = cube[i][1], z0 = cube[i][2];
        }

        //Y Rotation
        if (yTheta != 0) {
            //For y-rotation, x = z, y = x
            x = z0, y = x0;

            //Calculate radius
            //Radius is the distance from the point to the line running through (x, origin, origin)
            radius = sqrt(pow(x - origin, 2) + pow(y - origin, 2));

            //Calculate alpha (current radians counterclockwise from x-axis in 2D).
            //Find adjacent and opposite sides to alpha, then use atan() to find angle
            float alphaX = x - origin, alphaY = y - origin;
            alpha = atan(alphaY/alphaX);

            //If alpha in QI leave it
            //QII, QIII: Add PI
            //QIV: Add 2*PI

            //QII and QIII
            if ((alphaX < 0 && alphaY >= 0) || (alphaX < 0 && alphaY < 0)) {
                alpha += PI;
            } 
            //QIV
            else if (alphaX >= 0 && alphaY < 0) {
                alpha += 2*PI;
            }

            cube[i][2] = radius * cos(alpha + yTheta) + origin;
            cube[i][0] = radius * sin(alpha + yTheta) + origin;

            //Reset x0, y0, z0 variables 
            x0 = cube[i][0], y0 = cube[i][1], z0 = cube[i][2];
        }

        //Z Rotation
        if (zTheta != 0) {
            //For z-rotation, x = x, y = y
            x = x0, y = y0;

            //Calculate radius
            //Radius is the distance from the point to the line running through (x, origin, origin)
            radius = sqrt(pow(x - origin, 2) + pow(y - origin, 2));

            //Calculate alpha (current radians counterclockwise from x-axis in 2D).
            //Find adjacent and opposite sides to alpha, then use atan() to find angle
            float alphaX = x - origin, alphaY = y - origin;
            alpha = atan(alphaY/alphaX);

            //If alpha in QI leave it
            //QII, QIII: Add PI
            //QIV: Add 2*PI

            //QII and QIII
            if ((alphaX < 0 && alphaY >= 0) || (alphaX < 0 && alphaY < 0)) {
                alpha += PI;
            } 
            //QIV
            else if (alphaX >= 0 && alphaY < 0) {
                alpha += 2*PI;
            }

            cube[i][0] = radius * cos(alpha + zTheta) + origin;
            cube[i][1] = radius * sin(alpha + zTheta) + origin;
        }
    }

    return cube;
}

/*
Returns coordinates for the cube's vertices
Always generate cube in the same place (first octant), and always have the camera
set at a fixed point in 3D space.
Using this point, calculate the slope between it and each of the cube's vertices.
Calculate where these lines intersect some plane parallel to the y-axis (see where
they hit some wall between the camera and the cube)

Derivation of equations:

    Equation of line = (x0, y0, z0) + t<a, b, c>
        - Moving from some origin point (x0, y0, z0), scales the line
        in the direction of vector <a, b, c> by some constant t

    Camera at (xC, yC, zC)
        - Camera must be at the center of the cube and scale
        it's distance away so the cube does not warp
        - xC = length + length*1.5 + space
            - Sets camera (length*1.5) units away from the front of the cube
        - yC = zC = length/2 + space
            - Sets y and z to the center of the cube
    Vertex at (x0, y0, z0)
    Screen at plane x = (xC - length*1.5)
    Screen coordinates at (xS, yS)

    a = xC - x0, b = yC - y0, c = zC - z0

    Equation of line between vertex and camera:

        Line(vertex -> camera) = (x0, y0, z0) + t<a, b, c>

        Now, find where this line intersects the plane x = (xC - length*1.5)
            - Plane at the front of the cube 
            - (xC - length * 1.5 = length + space + length*1.5-length*1.5) = length + space

        xS = (xC - length*1.5)
        xS = x0 + ta
        x0 + ta = xC - length*1.5
        Therefore,
        t = (xC - length*1.5 - x0)/a

    The coordinates displayed on screen use the 3D y-coordinates as X and the 3D z-coordinates as Y (looking at cube
    in the direction of -x) derived from the line equation:
        - screen(xS, yS) = (y0 + tb, z0 + tc)
*/
int ** getVerticesCoordinates(float ** cube, int length) {
    //Screen coordinates array
    int **screen = malloc(sizeof(int *) * 8);
    for (int i = 0; i < 8; i++) {
        screen[i] = malloc(sizeof(int) * 2);
        for (int j = 0; j < 2; j++) {
            screen[i][j] = 0;
        }
    }
    //Camera coordinates, x set to length*1.5 units out from the space around the cube
    //y and z set to the center of the cube
    float xC = length*1.5 + length + getSpace(length),
          yC = (float)length/2 +getSpace(length), 
          zC = yC;
    //Variables to hold each instance of cube coordinates
    float x0, y0, z0;
    //Vector variables representing change in x, y, and z
    float a, b, c;
    //Constant multiplied by vector to scale line 
    //(find where line meets the plane in front of the cube)
    float t;
    //Y-value scaling constant:
    //██ Because the representation of pixels is slightly taller than it is
    //wide, use yScale to shrink the output vertically to even proportions.
    //0.8 works best
    float yScale = 0.8;

    //For each vertex...
    for (int i = 0; i < 8; i++) {
        //Set x0,y0,z0 to represent cube vertices points
        x0 = cube[i][0], y0 = cube[i][1], z0 = cube[i][2];
        //Set a, b, c vector values to change in x, y, and z, respectively
        a = xC - x0, b = yC - y0, c = zC - z0;
        //Solve for the t value that sets xS to the front of the cube (moves back length*1.5)
        //so that if the cube is not rotated it prints to scale (front face of length 10 cube is 10x10)
        t = (float)(xC - length*1.5 - x0)/(float)a;
        //Set screen(xS,yS) values
        //To avoid losing data when converting coordinates from 
        //float to int for Bresenham's algorithm, multiply each coordinate by 10
        // then round to the nearest integer
        screen[i][0] = (int)round((y0 + t*b)*10);
        screen[i][1] = (int)round((z0 + t*c)*10 * yScale);
   }
   return screen;
}

/*
Want to return a sorted 2D array of coordinates for use in printing in the terminal
Example (printCube(10, 0, 0, 0)):
    - array = {{5, 15}, {6, 15}, {7, 15}...{15, 15}, {5, 14}, {15, 14}, {5, 13}, {7, 13}...}
Sorts descending by y-value, if y-value is equal, sort ascending by x-value
    - Because terminal prints top to bottom, left to right
*/
Coordinates * generateCoordinates(int **screen) {
    //Allocate coords struct and coordsArray
    Coordinates *coords = malloc(sizeof(Coordinates));
    coords->coordsArray = malloc(sizeof(int*) * 1000);
    for (int i = 0; i < 1000; i++) {
        coords->coordsArray[i] = malloc(sizeof(int) * 2);
        for (int j = 0; j < 2; j++) {
            coords->coordsArray[i][j] = 0;
        }
    }
    coords->length = 0;
    //Find minX to shift cube to the left of the screen
    //Closest point to the left is always a vertex, so use screen array
    int minX = screen[0][0];
    int maxY = screen[0][1];
    for (int i = 1; i < 8; i++) {
        if (screen[i][0] < minX) {
            minX = screen[i][0];
        }
        if (screen[i][1] > maxY) {
            maxY = screen[i][1];
        }
    }
    coords->minX = minX;
    coords->maxY = maxY;
    //Bottom Lines:
    //B(0,1)
    getLineCoordinates(screen[0][0], screen[0][1], screen[1][0], screen[1][1], coords);
    //B(1,2)
    getLineCoordinates(screen[1][0], screen[1][1], screen[2][0], screen[2][1], coords);
    //B(2,3)
    getLineCoordinates(screen[2][0], screen[2][1], screen[3][0], screen[3][1], coords);
    //B(3,0)
    getLineCoordinates(screen[3][0], screen[3][1], screen[0][0], screen[0][1], coords);

    //Top Lines:
    //B(4,5)
    getLineCoordinates(screen[4][0], screen[4][1], screen[5][0], screen[5][1], coords);
    //B(5,6)
    getLineCoordinates(screen[5][0], screen[5][1], screen[6][0], screen[6][1], coords);
    //B(6,7)
    getLineCoordinates(screen[6][0], screen[6][1], screen[7][0], screen[7][1], coords);
    //B(7,4)
    getLineCoordinates(screen[7][0], screen[7][1], screen[4][0], screen[4][1], coords);

    //Column Lines:
    //B(0,4)
    getLineCoordinates(screen[0][0], screen[0][1], screen[4][0], screen[4][1], coords);
    //B(2,6)
    getLineCoordinates(screen[2][0], screen[2][1], screen[6][0], screen[6][1], coords);
    //B(1,5)
    getLineCoordinates(screen[1][0], screen[1][1], screen[5][0], screen[5][1], coords);
    //B(3,7)
    getLineCoordinates(screen[3][0], screen[3][1], screen[7][0], screen[7][1], coords);

    return coords;
}

/*
Bresenham's Line Generation:

   dx = x2 - x1; dy = y2 - y1;

   P1 = 2dy - dx
   if P < 0:
    Pk+1 = Pk + 2dy
   
   if P >= 0:
    Pk+1 = Pk + 2dy - 2dx
    y++

To make the code work with any slope (slopes < 1 or >= 1), just need to swap x and y
If slope < 1, dx is always >= dy, however, if slope >=1 dy is always >= dx
Therefore, you just do the normal slope < 1 algorithm, but sideways.

Variables are written as if the line's angle < 45 degrees, meaning slope < 1
Other slopes are accounted for by decrementing/incrementing variables depending on direction
If absolute(slope) > 1, swaps x and y values
*/
void getLineCoordinates(int x1, int y1, int x2, int y2, Coordinates *coords) {
    //Initialize x and y values for plotting
    int x = x1, y = y1;
    //Initializes change in x & change in y
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    //Swap variable to check if x & y values have been swapped
    int swap = 0;
    //Check if slope >= 1
    if (dx <= dy) {
        //If slope >= 1, swap all x and y values
        int tempX = x, tempX2 = x2, tempX1 = x1, tempDX = dx;

        x = y, y = tempX;
        x1 = y1, y1 = tempX1;
        x2 = y2, y2 = tempX2;
        dx = dy, dy = tempDX;

        //Update swap to 1
        swap = 1;
    }
    //Initial decision variable
    int Pk = 2*dy - dx;
    //Loops through x or y (depending on slope)
    for (int i = 0; i <= dx; i++) {
        //Set coordinates
        if (swap == 0) {
            coords->coordsArray[coords->length][0] = x; // - (coords->minX - 1);
            coords->coordsArray[coords->length][1] = y;
            coords->length++;
        } else {
            coords->coordsArray[coords->length][0] = y; // - (coords->minX - 1);
            coords->coordsArray[coords->length][1] = x;
            coords->length++;
        }

        x = x1 < x2 ? x + 1 : x - 1;

        if (Pk < 0) {
            Pk = Pk + 2*dy;
        } else {
            Pk = Pk + 2*dy - 2*dx;
            y = y1 < y2 ? y + 1 : y - 1;
        }
    }
}

/*
Quicksort functions used to sort coordinate array for printing
in the terminal. Sorts coordinates by descending y-values,
for coordinates with the same y-value, sorts by ascending x-values.
This way, when iterating through the array, it gives coordinates
from top to bottom, left to right for printing
*/
void quicksort(int **coordsArray, int low, int high) {
    if (low < high) {
        int pIndex = partition(coordsArray, low, high);

        quicksort(coordsArray, low, pIndex - 1);
        quicksort(coordsArray, pIndex + 1, high);
    }
}

int partition(int **coordsArray, int low, int high) {
    int pivot = coordsArray[high][1];

    //Index of element before the right position
    //of pivot found so far
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        //Sort descending y-values
        if (coordsArray[j][1] > pivot) {
            i++;
            swap(coordsArray, i, j);
        //For equal y-values, sort ascending x-values
        } else if (coordsArray[j][1] == pivot) {
            if (coordsArray[j][0] < coordsArray[high][0]) {
                i++;
                swap(coordsArray, i, j);
            }
        }
    }
    swap(coordsArray, i + 1, high);
    return (i + 1);
}

void swap(int **coordsArray, int a, int b) {
    int *temp = coordsArray[a];
    coordsArray[a] = coordsArray[b];
    coordsArray[b] = temp;
}

//Returns the amount of space required between the x, y, and z-axes so that the cube
//doesn't pass the bounds of the first octant when rotating
int getSpace(int length) {
    //Solid diagonal of the cube (from far corner to far corner) = side_length * sqr_root(3)
    float solidDiag = length * root_3;
    //Find spacing from axes to avoid rotating out of the first octant
    int space = ceil((solidDiag - length)/2 + 1);
    return space;
}

void freeCube(float **cube) {
    for (int i = 0; i < 8; i++) {
        free(cube[i]);
    }
    free(cube);
}

void freeScreen(int **screen) {
    for (int i = 0; i < 8; i++) {
        free(screen[i]);
    }
    free(screen);
}

void freeCoordinates(Coordinates * coords) {
    for (int i = 0; i < 1000; i++) {
        free(coords->coordsArray[i]);
    }
    free(coords->coordsArray);
    free(coords);
}