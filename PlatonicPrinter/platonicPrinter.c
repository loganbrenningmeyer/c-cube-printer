#include "platonicPrinter.h"

/*
When printing platonic solids, first build a cube
Then set the vertices of the solid using the vertices of the cube
Adjust screen size to # of vertices
*/

//Declare PI
#define PI 3.141593
//Declare golden ratio
#define phi 1.618034
//Declare square root of 3
#define root_3 1.732051

//Animates cube based on given starting orientation, 
//ending orientation, cube length, duration, and fps
void animateSolid(int solidType, int length, int seconds, int fps) {

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
        printSolid(solidType, length, xTheta, yTheta, zTheta);
        xTheta += xInc, yTheta += yInc, zTheta += zInc;
        //Hold the frame for 1/fps seconds
        //usleep((float)1/fps * 1000000);
        usleep((int)((float)1/fps * 1000000));
        //Clear the console for the next frame
        printf("\e[1;1H\e[2J");
    }
    //Print final solid
    printSolid(solidType, length, xTheta, yTheta, zTheta);
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

//Function called to build, rotate, and print platonic solid. Called for each frame in 
//an animation and once when printing a single solid.
void printSolid(int solidType, int length, float xTheta, float yTheta, float zTheta) {
    //Declare numVertices
    int numVertices;
    //Initialize cube to construct platonic solid
    float **cube = buildCube(length);
    //Initialize platonic solid from cube
    float **solid = buildSolid(cube, solidType, length, &numVertices);   

    //Apply rotations
    rotateSolid(solid, length, numVertices, xTheta, yTheta, zTheta);

    //Find the 2D projected coordinates of the cube vertices
    int **screen = getVerticesCoordinates(solid, length, numVertices);

    //Initialize coordinate struct pointer and generate all 2D cube coordinates
    Coordinates *coords = generateCoordinates(screen, solidType, numVertices);
    coords->cubeLength = length;

    //Sort coordinate array for printing in the terminal
    quicksort(coords->coordsArray, 0, coords->length - 1);

    //Print cube to terminal
    printToTerminal(coords);

    //Free cube coordinates array and screen coordinates array
    if (solidType != 1) freeSolid(solid, numVertices);
    freeCube(cube);
    freeScreen(screen, numVertices);
    freeCoordinates(coords);
}

/*
Returns an array of the platonic solid's (x, y, z)
coordinates. Also sets *numVertices for use in other
functions
*/
float ** buildSolid(float **cube, int solidType, int length, int *numVertices) {
    switch (solidType) {
        //Cube
        case 1:
            *numVertices = 8;

            return cube;
        //Tetrahedron
        case 2: 
            *numVertices = 4;

            float **tetrahedron = malloc(sizeof(float *) * *numVertices);
            for (int i = 0; i < *numVertices; i++) {
                tetrahedron[i] = malloc(sizeof(float) * 3);
            }
            //Set tetrahedron vertices to cube's 0, 2, 5, and 7 vertices
            tetrahedron[0][0] = cube[0][0], tetrahedron[0][1] = cube[0][1], tetrahedron[0][2] = cube[0][2];
            tetrahedron[1][0] = cube[2][0], tetrahedron[1][1] = cube[2][1], tetrahedron[1][2] = cube[2][2];
            tetrahedron[2][0] = cube[5][0], tetrahedron[2][1] = cube[5][1], tetrahedron[2][2] = cube[5][2];
            tetrahedron[3][0] = cube[7][0], tetrahedron[3][1] = cube[7][1], tetrahedron[3][2] = cube[7][2];

            return tetrahedron;
        /*
        Octahedron
        Vertices are the 6 centers of the cube's faces
        */
        case 3:
            *numVertices = 6;

            float **octahedron = malloc(sizeof(float *) * *numVertices);
            for (int i = 0; i < *numVertices; i++) {
                octahedron[i] = malloc(sizeof(float) * 3);
            }

            octahedron[0][0] = (cube[0][0]+cube[2][0])/2, octahedron[0][1] = (cube[0][1]+cube[2][1])/2, octahedron[0][2] = (cube[0][2]+cube[2][2])/2;
            octahedron[1][0] = (cube[2][0]+cube[5][0])/2, octahedron[1][1] = (cube[2][1]+cube[5][1])/2, octahedron[1][2] = (cube[2][2]+cube[5][2])/2;
            octahedron[2][0] = (cube[3][0]+cube[6][0])/2, octahedron[2][1] = (cube[3][1]+cube[6][1])/2, octahedron[2][2] = (cube[3][2]+cube[6][2])/2;
            octahedron[3][0] = (cube[0][0]+cube[7][0])/2, octahedron[3][1] = (cube[0][1]+cube[7][1])/2, octahedron[3][2] = (cube[0][2]+cube[7][2])/2;
            octahedron[4][0] = (cube[1][0]+cube[4][0])/2, octahedron[4][1] = (cube[1][1]+cube[4][1])/2, octahedron[4][2] = (cube[1][2]+cube[4][2])/2;
            octahedron[5][0] = (cube[6][0]+cube[4][0])/2, octahedron[5][1] = (cube[6][1]+cube[4][1])/2, octahedron[5][2] = (cube[6][2]+cube[4][2])/2;

            return octahedron;
        /*
        Icosahedron
        Vertices are the corners of 3 intersecting golden rectanges centered in the cube:
        length/width = phi
        length == length of cube
        width == length/phi
        One rectangle for each axis
        */
        case 4:
            *numVertices = 12;
            //Dimensions of the golden rectangles that form the vertices
            //rectangleLength declared for clarity on the rectangle dimensions
            float rectangleLength = length, rectangleWidth = rectangleLength/phi;

            float **icosahedron = malloc(sizeof(float *) * *numVertices);
            for (int i = 0; i < *numVertices; i++) {
                icosahedron[i] = malloc(sizeof(float) * 3);
            }

            //X-axis Rectangle
            icosahedron[0][0] = cube[0][0], icosahedron[0][1] = (cube[1][1]+cube[2][1])/2 - rectangleWidth/2, icosahedron[0][2] = (cube[1][2]+cube[5][2])/2;
            icosahedron[1][0] = cube[1][0], icosahedron[1][1] = (cube[1][1]+cube[2][1])/2 - rectangleWidth/2, icosahedron[1][2] = (cube[1][2]+cube[5][2])/2;
            icosahedron[2][0] = cube[1][0], icosahedron[2][1] = (cube[1][1]+cube[2][1])/2 + rectangleWidth/2, icosahedron[2][2] = (cube[1][2]+cube[5][2])/2;
            icosahedron[3][0] = cube[0][0], icosahedron[3][1] = (cube[1][1]+cube[2][1])/2 + rectangleWidth/2, icosahedron[3][2] = (cube[1][2]+cube[5][2])/2;

            //Y-axis Rectangle
            icosahedron[4][0] = (cube[0][0]+cube[1][0])/2, icosahedron[4][1] = cube[0][1], icosahedron[4][2] = (cube[0][2]+cube[4][2])/2 - rectangleWidth/2;
            icosahedron[5][0] = (cube[0][0]+cube[1][0])/2, icosahedron[5][1] = cube[2][1], icosahedron[5][2] = (cube[0][2]+cube[4][2])/2 - rectangleWidth/2;
            icosahedron[6][0] = (cube[0][0]+cube[1][0])/2, icosahedron[6][1] = cube[2][1], icosahedron[6][2] = (cube[0][2]+cube[4][2])/2 + rectangleWidth/2;
            icosahedron[7][0] = (cube[0][0]+cube[1][0])/2, icosahedron[7][1] = cube[0][1], icosahedron[7][2] = (cube[0][2]+cube[4][2])/2 + rectangleWidth/2;

            //Z-axis Rectangle
            icosahedron[8][0] = (cube[0][0]+cube[1][0])/2 - rectangleWidth/2, icosahedron[8][1] = (cube[0][1]+cube[3][1])/2, icosahedron[8][2] = cube[0][2];
            icosahedron[9][0] = (cube[0][0]+cube[1][0])/2 - rectangleWidth/2, icosahedron[9][1] = (cube[0][1]+cube[3][1])/2, icosahedron[9][2] = cube[4][2];
            icosahedron[10][0] = (cube[0][0]+cube[1][0])/2 + rectangleWidth/2, icosahedron[10][1] = (cube[0][1]+cube[3][1])/2, icosahedron[10][2] = cube[4][2];
            icosahedron[11][0] = (cube[0][0]+cube[1][0])/2 + rectangleWidth/2, icosahedron[11][1] = (cube[0][1]+cube[3][1])/2, icosahedron[11][2] = cube[0][2];
            
            return icosahedron;

        case 5:
            //Vertices = center of triangles formed by icosahedron (average coordinates of triangle vertices)
            //8 triangles in front, 8 in back, 2 on top, and 2 on bottom
            //First build an icosahedron, use it to find dodecahedron vertices, then free it

            *numVertices = 20;
            //Temporary variable to pass into buildSolid() to avoid changing the value of numVertices
            int icosaVertices;

            float **dodecahedron = malloc(sizeof(float *) * *numVertices);
            for (int i = 0; i < *numVertices; i++) {
                dodecahedron[i] = malloc(sizeof(float) * 3);
            }

            float **doIcosahedron = buildSolid(cube, 4, length, &icosaVertices);

            //Front
            //avg(1, 10, 2)
            dodecahedron[0][0] = (doIcosahedron[1][0]+doIcosahedron[10][0]+doIcosahedron[2][0])/3, 
            dodecahedron[0][1] = (doIcosahedron[1][1]+doIcosahedron[10][1]+doIcosahedron[2][1])/3,
            dodecahedron[0][2] = (doIcosahedron[1][2]+doIcosahedron[10][2]+doIcosahedron[2][2])/3;
            //avg(1, 10, 7)
            dodecahedron[1][0] = (doIcosahedron[1][0]+doIcosahedron[10][0]+doIcosahedron[7][0])/3, 
            dodecahedron[1][1] = (doIcosahedron[1][1]+doIcosahedron[10][1]+doIcosahedron[7][1])/3,
            dodecahedron[1][2] = (doIcosahedron[1][2]+doIcosahedron[10][2]+doIcosahedron[7][2])/3;
            //avg(2, 10, 6)
            dodecahedron[2][0] = (doIcosahedron[2][0]+doIcosahedron[10][0]+doIcosahedron[6][0])/3, 
            dodecahedron[2][1] = (doIcosahedron[2][1]+doIcosahedron[10][1]+doIcosahedron[6][1])/3,
            dodecahedron[2][2] = (doIcosahedron[2][2]+doIcosahedron[10][2]+doIcosahedron[6][2])/3;
            //avg(1, 4, 7)
            dodecahedron[3][0] = (doIcosahedron[1][0]+doIcosahedron[4][0]+doIcosahedron[7][0])/3, 
            dodecahedron[3][1] = (doIcosahedron[1][1]+doIcosahedron[4][1]+doIcosahedron[7][1])/3,
            dodecahedron[3][2] = (doIcosahedron[1][2]+doIcosahedron[4][2]+doIcosahedron[7][2])/3;
            //avg(2, 5, 6)
            dodecahedron[4][0] = (doIcosahedron[2][0]+doIcosahedron[5][0]+doIcosahedron[6][0])/3, 
            dodecahedron[4][1] = (doIcosahedron[2][1]+doIcosahedron[5][1]+doIcosahedron[6][1])/3,
            dodecahedron[4][2] = (doIcosahedron[2][2]+doIcosahedron[5][2]+doIcosahedron[6][2])/3;
            //avg(1, 2, 11)
            dodecahedron[5][0] = (doIcosahedron[1][0]+doIcosahedron[2][0]+doIcosahedron[11][0])/3, 
            dodecahedron[5][1] = (doIcosahedron[1][1]+doIcosahedron[2][1]+doIcosahedron[11][1])/3,
            dodecahedron[5][2] = (doIcosahedron[1][2]+doIcosahedron[2][2]+doIcosahedron[11][2])/3;
            //avg(1, 4, 11)
            dodecahedron[6][0] = (doIcosahedron[1][0]+doIcosahedron[4][0]+doIcosahedron[11][0])/3, 
            dodecahedron[6][1] = (doIcosahedron[1][1]+doIcosahedron[4][1]+doIcosahedron[11][1])/3,
            dodecahedron[6][2] = (doIcosahedron[1][2]+doIcosahedron[4][2]+doIcosahedron[11][2])/3;
            //avg(11, 2, 5)
            dodecahedron[7][0] = (doIcosahedron[11][0]+doIcosahedron[2][0]+doIcosahedron[5][0])/3, 
            dodecahedron[7][1] = (doIcosahedron[11][1]+doIcosahedron[2][1]+doIcosahedron[5][1])/3,
            dodecahedron[7][2] = (doIcosahedron[11][2]+doIcosahedron[2][2]+doIcosahedron[5][2])/3;

            //Back
            //avg(0, 9, 3)
            dodecahedron[8][0] = (doIcosahedron[0][0]+doIcosahedron[9][0]+doIcosahedron[3][0])/3, 
            dodecahedron[8][1] = (doIcosahedron[0][1]+doIcosahedron[9][1]+doIcosahedron[3][1])/3,
            dodecahedron[8][2] = (doIcosahedron[0][2]+doIcosahedron[9][2]+doIcosahedron[3][2])/3;
            //avg(0, 9, 7)
            dodecahedron[9][0] = (doIcosahedron[0][0]+doIcosahedron[9][0]+doIcosahedron[7][0])/3, 
            dodecahedron[9][1] = (doIcosahedron[0][1]+doIcosahedron[9][1]+doIcosahedron[7][1])/3,
            dodecahedron[9][2] = (doIcosahedron[0][2]+doIcosahedron[9][2]+doIcosahedron[7][2])/3;
            //avg(3, 9 ,6)
            dodecahedron[10][0] = (doIcosahedron[3][0]+doIcosahedron[9][0]+doIcosahedron[6][0])/3, 
            dodecahedron[10][1] = (doIcosahedron[3][1]+doIcosahedron[9][1]+doIcosahedron[6][1])/3,
            dodecahedron[10][2] = (doIcosahedron[3][2]+doIcosahedron[9][2]+doIcosahedron[6][2])/3;
            //avg(0, 4, 7)
            dodecahedron[11][0] = (doIcosahedron[0][0]+doIcosahedron[4][0]+doIcosahedron[7][0])/3, 
            dodecahedron[11][1] = (doIcosahedron[0][1]+doIcosahedron[4][1]+doIcosahedron[7][1])/3,
            dodecahedron[11][2] = (doIcosahedron[0][2]+doIcosahedron[4][2]+doIcosahedron[7][2])/3;
            //avg(3, 5, 6)
            dodecahedron[12][0] = (doIcosahedron[3][0]+doIcosahedron[5][0]+doIcosahedron[6][0])/3, 
            dodecahedron[12][1] = (doIcosahedron[3][1]+doIcosahedron[5][1]+doIcosahedron[6][1])/3,
            dodecahedron[12][2] = (doIcosahedron[3][2]+doIcosahedron[5][2]+doIcosahedron[6][2])/3;
            //avg(0, 3, 8)
            dodecahedron[13][0] = (doIcosahedron[0][0]+doIcosahedron[3][0]+doIcosahedron[8][0])/3, 
            dodecahedron[13][1] = (doIcosahedron[0][1]+doIcosahedron[3][1]+doIcosahedron[8][1])/3,
            dodecahedron[13][2] = (doIcosahedron[0][2]+doIcosahedron[3][2]+doIcosahedron[8][2])/3;
            //avg(0, 4, 8)
            dodecahedron[14][0] = (doIcosahedron[0][0]+doIcosahedron[4][0]+doIcosahedron[8][0])/3, 
            dodecahedron[14][1] = (doIcosahedron[0][1]+doIcosahedron[4][1]+doIcosahedron[8][1])/3,
            dodecahedron[14][2] = (doIcosahedron[0][2]+doIcosahedron[4][2]+doIcosahedron[8][2])/3;
            //avg(8, 3, 5)
            dodecahedron[15][0] = (doIcosahedron[8][0]+doIcosahedron[3][0]+doIcosahedron[5][0])/3, 
            dodecahedron[15][1] = (doIcosahedron[8][1]+doIcosahedron[3][1]+doIcosahedron[5][1])/3,
            dodecahedron[15][2] = (doIcosahedron[8][2]+doIcosahedron[3][2]+doIcosahedron[5][2])/3;

            //Top
            //avg(7, 9, 10)
            dodecahedron[16][0] = (doIcosahedron[7][0]+doIcosahedron[9][0]+doIcosahedron[10][0])/3, 
            dodecahedron[16][1] = (doIcosahedron[7][1]+doIcosahedron[9][1]+doIcosahedron[10][1])/3,
            dodecahedron[16][2] = (doIcosahedron[7][2]+doIcosahedron[9][2]+doIcosahedron[10][2])/3;
            //avg(6, 9, 10)
            dodecahedron[17][0] = (doIcosahedron[6][0]+doIcosahedron[9][0]+doIcosahedron[10][0])/3, 
            dodecahedron[17][1] = (doIcosahedron[6][1]+doIcosahedron[9][1]+doIcosahedron[10][1])/3,
            dodecahedron[17][2] = (doIcosahedron[6][2]+doIcosahedron[9][2]+doIcosahedron[10][2])/3;

            //Bottom
            //avg(4, 8, 11)
            dodecahedron[18][0] = (doIcosahedron[4][0]+doIcosahedron[8][0]+doIcosahedron[11][0])/3, 
            dodecahedron[18][1] = (doIcosahedron[4][1]+doIcosahedron[8][1]+doIcosahedron[11][1])/3,
            dodecahedron[18][2] = (doIcosahedron[4][2]+doIcosahedron[8][2]+doIcosahedron[11][2])/3;
            //avg(5, 8 ,11)
            dodecahedron[19][0] = (doIcosahedron[5][0]+doIcosahedron[8][0]+doIcosahedron[11][0])/3, 
            dodecahedron[19][1] = (doIcosahedron[5][1]+doIcosahedron[8][1]+doIcosahedron[11][1])/3,
            dodecahedron[19][2] = (doIcosahedron[5][2]+doIcosahedron[8][2]+doIcosahedron[11][2])/3;

            freeSolid(doIcosahedron, icosaVertices);

            return dodecahedron;
        default:
            return cube;
    }
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
float ** rotateSolid(float **cube, int length, int numVertices, float xTheta, float yTheta, float zTheta) {
    //Convert theta from degrees to radians
    xTheta *= PI/180, yTheta *= PI/180, zTheta *= PI/180;

    //Center of the faces for rotation
    float origin = getSpace(length) + (float)length/2;

    //Apply x, y, and z rotation one point at a time
    for (int i = 0; i < numVertices; i++) {
        
        float x0 = cube[i][0], y0 = cube[i][1], z0 = cube[i][2];
        //Variables to be used in formula
        float x, y, z;
        float radius, alpha;
        //Calculate alpha (current radians counterclockwise from x-axis)

        //For each rotation, if alphaX and alphaY are 0, meaning the vertex is in line with the camera
        //ignore rotation because the vertex doesn't move if it is on the axis of rotation

        //X Rotation
        if (xTheta != 0 && (y0 - origin != 0 || z0 - origin != 0)) {
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
        if (yTheta != 0 && (z0 - origin != 0 || x0 - origin != 0)) {
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
        if (zTheta != 0 && (x0 - origin != 0 || y0 - origin != 0)) {
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
int ** getVerticesCoordinates(float ** solid, int length, int numVertices) {
    //Screen coordinates array
    int **screen = malloc(sizeof(int *) * numVertices);
    for (int i = 0; i < numVertices; i++) {
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
    for (int i = 0; i < numVertices; i++) {
        //Set x0,y0,z0 to represent cube vertices points
        x0 = solid[i][0], y0 = solid[i][1], z0 = solid[i][2];
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
Example (printSolid(10, 0, 0, 0)):
    - array = {{5, 15}, {6, 15}, {7, 15}...{15, 15}, {5, 14}, {15, 14}, {5, 13}, {7, 13}...}
Sorts descending by y-value, if y-value is equal, sort ascending by x-value
    - Because terminal prints top to bottom, left to right
*/
Coordinates * generateCoordinates(int **screen, int solidType, int numVertices) {
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
    //Find maxY to shift cube down to stay centered
    //Highest point is always a vertex so use screen array
    int maxY = screen[0][1];
    for (int i = 1; i < numVertices; i++) {
        if (screen[i][1] > maxY) {
            maxY = screen[i][1];
        }
    }
    coords->maxY = maxY;

    //Cube
    //B(m, n) for Bresenham and the corresponding vertices being connected
    if (solidType == 1) {
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
    }
    //Tetrahedron, 6 edges
    else if (solidType == 2) {
        //B(0, 2)
        getLineCoordinates(screen[0][0], screen[0][1], screen[1][0], screen[1][1], coords);
        //B(0, 5)
        getLineCoordinates(screen[0][0], screen[0][1], screen[2][0], screen[2][1], coords);
        //B(0, 7)
        getLineCoordinates(screen[0][0], screen[0][1], screen[3][0], screen[3][1], coords);
        //B(2, 5)
        getLineCoordinates(screen[1][0], screen[1][1], screen[2][0], screen[2][1], coords);
        //B(5, 7)
        getLineCoordinates(screen[2][0], screen[2][1], screen[3][0], screen[3][1], coords);
        //B(2, 7)
        getLineCoordinates(screen[1][0], screen[1][1], screen[3][0], screen[3][1], coords);
    }
    //Octahedron, 12 edges
    //4 edges from 0 
    //4 edges from 5
    //4 edges around middle
    else if (solidType == 3) {
        //B(0, 1)
        getLineCoordinates(screen[0][0], screen[0][1], screen[1][0], screen[1][1], coords);
        //B(0, 2)
        getLineCoordinates(screen[0][0], screen[0][1], screen[2][0], screen[2][1], coords);
        //B(0, 3)
        getLineCoordinates(screen[0][0], screen[0][1], screen[3][0], screen[3][1], coords);
        //B(0, 4)
        getLineCoordinates(screen[0][0], screen[0][1], screen[4][0], screen[4][1], coords);
        //B(5, 1)
        getLineCoordinates(screen[5][0], screen[5][1], screen[1][0], screen[1][1], coords);
        //B(5, 2)
        getLineCoordinates(screen[5][0], screen[5][1], screen[2][0], screen[2][1], coords);
        //B(5, 3)
        getLineCoordinates(screen[5][0], screen[5][1], screen[3][0], screen[3][1], coords);
        //B(5, 4)
        getLineCoordinates(screen[5][0], screen[5][1], screen[4][0], screen[4][1], coords);
        //B(1, 2)
        getLineCoordinates(screen[1][0], screen[1][1], screen[2][0], screen[2][1], coords);
        //B(2, 3)
        getLineCoordinates(screen[2][0], screen[2][1], screen[3][0], screen[3][1], coords);
        //B(3, 4)
        getLineCoordinates(screen[3][0], screen[3][1], screen[4][0], screen[4][1], coords);
        //B(4, 1)
        getLineCoordinates(screen[4][0], screen[4][1], screen[1][0], screen[1][1], coords);
    }
    //Icosahedron
    //30 edges
    else if (solidType == 4) {
        //12 front, 12 back
        //6 along rectangle width

        //Front
        //B(10, 7)
        getLineCoordinates(screen[10][0], screen[10][1], screen[7][0], screen[7][1], coords);
        //B(10, 6)
        getLineCoordinates(screen[10][0], screen[10][1], screen[6][0], screen[6][1], coords);
        //B(10, 1)
        getLineCoordinates(screen[10][0], screen[10][1], screen[1][0], screen[1][1], coords);
        //B(10, 2)
        getLineCoordinates(screen[10][0], screen[10][1], screen[2][0], screen[2][1], coords);
        //B(7, 1)
        getLineCoordinates(screen[7][0], screen[7][1], screen[1][0], screen[1][1], coords);
        //B(6, 2)
        getLineCoordinates(screen[6][0], screen[6][1], screen[2][0], screen[2][1], coords);
        //B(1, 4)
        getLineCoordinates(screen[1][0], screen[1][1], screen[4][0], screen[4][1], coords);
        //B(2, 5)
        getLineCoordinates(screen[2][0], screen[2][1], screen[5][0], screen[5][1], coords);
        //B(4, 11)
        getLineCoordinates(screen[4][0], screen[4][1], screen[11][0], screen[11][1], coords);
        //B(5, 11)
        getLineCoordinates(screen[5][0], screen[5][1], screen[11][0], screen[11][1], coords);
        //B(1, 11)
        getLineCoordinates(screen[1][0], screen[1][1], screen[11][0], screen[11][1], coords);
        //B(2, 11)
        getLineCoordinates(screen[2][0], screen[2][1], screen[11][0], screen[11][1], coords);

        //Back
        //B(9, 7)
        getLineCoordinates(screen[9][0], screen[9][1], screen[7][0], screen[7][1], coords);
        //B(9, 6)
        getLineCoordinates(screen[9][0], screen[9][1], screen[6][0], screen[6][1], coords);
        //B(9, 0)
        getLineCoordinates(screen[9][0], screen[9][1], screen[0][0], screen[0][1], coords);
        //B(9, 3)
        getLineCoordinates(screen[9][0], screen[9][1], screen[3][0], screen[3][1], coords);
        //B(7, 0)
        getLineCoordinates(screen[7][0], screen[7][1], screen[0][0], screen[0][1], coords);
        //B(6, 3)
        getLineCoordinates(screen[6][0], screen[6][1], screen[3][0], screen[3][1], coords);
        //B(0, 4)
        getLineCoordinates(screen[0][0], screen[0][1], screen[4][0], screen[4][1], coords);
        //B(3, 5)
        getLineCoordinates(screen[3][0], screen[3][1], screen[5][0], screen[5][1], coords);
        //B(4, 8)
        getLineCoordinates(screen[4][0], screen[4][1], screen[8][0], screen[8][1], coords);
        //B(5, 8)
        getLineCoordinates(screen[5][0], screen[5][1], screen[8][0], screen[8][1], coords);
        //B(0, 8)
        getLineCoordinates(screen[0][0], screen[0][1], screen[8][0], screen[8][1], coords);
        //B(3, 8)
        getLineCoordinates(screen[3][0], screen[3][1], screen[8][0], screen[8][1], coords);

        //Along widths
        //B(0, 3)
        getLineCoordinates(screen[0][0], screen[0][1], screen[3][0], screen[3][1], coords);
        //B(1, 2)
        getLineCoordinates(screen[1][0], screen[1][1], screen[2][0], screen[2][1], coords);
        //B(4, 7)
        getLineCoordinates(screen[4][0], screen[4][1], screen[7][0], screen[7][1], coords);
        //B(5, 6)
        getLineCoordinates(screen[5][0], screen[5][1], screen[6][0], screen[6][1], coords);
        //B(8, 11)
        getLineCoordinates(screen[8][0], screen[8][1], screen[11][0], screen[11][1], coords);
        //B(9, 10)
        getLineCoordinates(screen[9][0], screen[9][1], screen[10][0], screen[10][1], coords);
    }
    //Dodecahedron
    //30 edges (separated by pentagon, each needs less as shared lines have already been calculated)
    else if (solidType == 5) {
        //B(0, 1)
        getLineCoordinates(screen[0][0], screen[0][1], screen[1][0], screen[1][1], coords);
        //B(1, 16)
        getLineCoordinates(screen[1][0], screen[1][1], screen[16][0], screen[16][1], coords);
        //B(16, 17)
        getLineCoordinates(screen[16][0], screen[16][1], screen[17][0], screen[17][1], coords);
        //B(17, 2)
        getLineCoordinates(screen[17][0], screen[17][1], screen[2][0], screen[2][1], coords);
        //B(2, 0)
        getLineCoordinates(screen[2][0], screen[2][1], screen[0][0], screen[0][1], coords);

        //B(16, 9)
        getLineCoordinates(screen[16][0], screen[16][1], screen[9][0], screen[9][1], coords);
        //B(9, 8)
        getLineCoordinates(screen[9][0], screen[9][1], screen[8][0], screen[8][1], coords);
        //B(8, 10)
        getLineCoordinates(screen[8][0], screen[8][1], screen[10][0], screen[10][1], coords);
        //B(10, 17)
        getLineCoordinates(screen[10][0], screen[10][1], screen[17][0], screen[17][1], coords);

        //B(1, 3)
        getLineCoordinates(screen[1][0], screen[1][1], screen[3][0], screen[3][1], coords);
        //B(9, 11)
        getLineCoordinates(screen[9][0], screen[9][1], screen[11][0], screen[11][1], coords);
        //B(11, 3)
        getLineCoordinates(screen[11][0], screen[11][1], screen[3][0], screen[3][1], coords);

        //B(2, 4)
        getLineCoordinates(screen[2][0], screen[2][1], screen[4][0], screen[4][1], coords);
        //B(10, 12)
        getLineCoordinates(screen[10][0], screen[10][1], screen[12][0], screen[12][1], coords);
        //B(12, 4)
        getLineCoordinates(screen[12][0], screen[12][1], screen[4][0], screen[4][1], coords);

        //B(0, 5)
        getLineCoordinates(screen[0][0], screen[0][1], screen[5][0], screen[5][1], coords);
        //B(5, 6)
        getLineCoordinates(screen[5][0], screen[5][1], screen[6][0], screen[6][1], coords);
        //B(6, 3)
        getLineCoordinates(screen[6][0], screen[6][1], screen[3][0], screen[3][1], coords);

        //B(5, 7)
        getLineCoordinates(screen[5][0], screen[5][1], screen[7][0], screen[7][1], coords);
        //B(7, 4)
        getLineCoordinates(screen[7][0], screen[7][1], screen[4][0], screen[4][1], coords);

        //B(8, 13)
        getLineCoordinates(screen[8][0], screen[8][1], screen[13][0], screen[13][1], coords);
        //B(13, 14)
        getLineCoordinates(screen[13][0], screen[13][1], screen[14][0], screen[14][1], coords);
        //B(14, 11)
        getLineCoordinates(screen[14][0], screen[14][1], screen[11][0], screen[11][1], coords);

        //B(13, 15)
        getLineCoordinates(screen[13][0], screen[13][1], screen[15][0], screen[15][1], coords);
        //B(15, 12)
        getLineCoordinates(screen[15][0], screen[15][1], screen[12][0], screen[12][1], coords);

        //B(6, 18)
        getLineCoordinates(screen[6][0], screen[6][1], screen[18][0], screen[18][1], coords);
        //B(18, 14)
        getLineCoordinates(screen[18][0], screen[18][1], screen[14][0], screen[14][1], coords);

        //B(7, 19)
        getLineCoordinates(screen[7][0], screen[7][1], screen[19][0], screen[19][1], coords);
        //B(19, 15)
        getLineCoordinates(screen[19][0], screen[19][1], screen[15][0], screen[15][1], coords);

        //B(18, 19)
        getLineCoordinates(screen[18][0], screen[18][1], screen[19][0], screen[19][1], coords);
    }
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
            coords->coordsArray[coords->length][0] = x;
            coords->coordsArray[coords->length][1] = y;
            coords->length++;
        } else {
            coords->coordsArray[coords->length][0] = y;
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

void freeSolid(float **solid, int numVertices) {
    for (int i = 0; i < numVertices; i++) {
        free(solid[i]);
    }
    free(solid);
}

void freeScreen(int **screen, int numVertices) {
    for (int i = 0; i < numVertices; i++) {
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