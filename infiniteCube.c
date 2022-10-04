#include "cubePrinter.h"

//Infinitely animates a rotating cube, press CTRL+C to stop
int main(void) {
    srand(time(NULL));

    int size = 0, fps;
    float xTheta, yTheta, zTheta;
    //Sets random starting point and random orientation increment values
    xTheta = rand() % 721 - 360, yTheta = rand() % 721 - 360, zTheta = rand() % 721 - 360;
    float xInc = (float)rand()/(float)RAND_MAX, yInc = (float)rand()/(float)RAND_MAX, zInc = (float)rand()/(float)RAND_MAX;

    printf("INFINITE CUBE\n");
    while (size < 1 || size > 10) {
        printf("Size (1-10): ");
        if (scanf("%d", &size) == 0) return 1;
    }
    printf("FPS: ");
    if (scanf("%d", &fps) == 0) return 1;

    while (1) {
        printCube(size, xTheta, yTheta, zTheta);
        xTheta += xInc, yTheta += yInc, zTheta += zInc;
        usleep((int)((float)1/fps * 1000000));
        printf("\e[1;1H\e[2J");
    }
}