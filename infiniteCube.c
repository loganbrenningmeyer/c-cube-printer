#include "cubePrinter.h"

//Infinitely animates a rotating cube, press CTRL+C to stop
int main(void) {
    srand(time(NULL));

    int size = 0, fps;
    float xTheta, yTheta, zTheta;
    //Sets random starting orientation in the range [-360,360]
    //Sets random increment values in the range [0,1]
    xTheta = rand() % 721 - 360, yTheta = rand() % 721 - 360, zTheta = rand() % 721 - 360;
    float xInc = (float)rand()/(float)RAND_MAX, yInc = (float)rand()/(float)RAND_MAX, zInc = (float)rand()/(float)RAND_MAX;

    printf("INFINITE CUBE\n");
    while (size < 1 || size > 10) {
        printf("Size (1-10): ");
        scanf("%d", &size);
    }
    printf("FPS: ");
    scanf("%d", &fps);

    //Each frame, prints the cube then adjusts its orientation by xInc, yInc, and zInc
    while (1) {
        printCube(size, xTheta, yTheta, zTheta);
        xTheta += xInc, yTheta += yInc, zTheta += zInc;
        usleep((int)((float)1/fps * 1000000));
        printf("\e[1;1H\e[2J");
    }
}
