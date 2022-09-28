#include "cubePrinter.h"

int main(void) {

    char printAgain = 'y';

    while (printAgain == 'y') {

        printf("\n                      ╔══════════════╗\n");
        printf(  "══════════════════════╣ CUBE PRINTER ╠═════════════════════\n");
        printf(  "                      ╚══════════════╝\n");   
        printf("Animate a random rotation   -OR-   Print a user-input cube\n\n");

        int userChoice = 0;

        while (userChoice != 1 && userChoice != 2) {
            printf("\nAnimate (1) or Print Cube (2): ");
            scanf("%d", &userChoice);
        }

        //Animate cube
        if (userChoice == 1) {
            int fps, seconds, length;

            //Take animation duration as input
            printf("\nDuration (s): ");
            scanf("%d", &seconds);

            //Take fps as input
            printf("FPS: ");
            scanf("%d", &fps);

            //Take length as input
            while (length > 10 || length < 1) {
                printf("Cube Length (1-10): ");
                scanf("%d", &length);
            }

            animateCube(length, seconds, fps);
            
        //Print cube
        } else {
            printf("\n");
            //Get user-inputted info for printCube()
            int length = 0, xTheta, yTheta, zTheta;
            while (length < 1 || length > 10) {
                printf("Cube Length (1-10): ");
                scanf("%d", &length);
            }
            printf("X-Orientation (degrees): ");
            scanf("%d", &xTheta);
            printf("Y-Orientation (degrees): ");
            scanf("%d", &yTheta);
            printf("Z-Orientation (degrees): ");
            scanf("%d", &zTheta);

            printCube(length, xTheta, yTheta, zTheta);
        }

        printf("\nPrint another cube? (y/n): ");
        scanf("%s", &printAgain);
    }
}