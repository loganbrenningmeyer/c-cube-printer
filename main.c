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
            if (scanf("%d", &userChoice) == 0) {
                return 1;
            } else if (userChoice == EOF) {
                return 0;
            }
        }

        //Animate cube
        if (userChoice == 1) {

            int fps = 0, seconds = 0, length = 0;

            //Take animation duration as input
            printf("\nDuration (s): ");
            if (scanf("%d", &seconds) == 0) return 1;

            //Take fps as input
            printf("FPS: ");
            if (scanf("%d", &fps) == 0) return 1;

            //Take length as input
            while (length < 1 || length > 10) {
                printf("Cube Length (1-10): ");
                if (scanf("%d", &length) == 0) return 1;
            }

            animateCube(length, seconds, fps);

        //Print cube
        } else {
            printf("\n");
            //Get user-inputted info for printCube()
            int length = 0, xTheta, yTheta, zTheta;
            while (length < 1 || length > 10) {
                printf("Cube Length (1-10): ");
                if (scanf("%d", &length) == 0) return 1;
            }

            printf("X-Orientation (degrees): ");
            if (scanf("%d", &xTheta) == 0) return 1;
            printf("Y-Orientation (degrees): ");
            if (scanf("%d", &yTheta) == 0) return 1;
            printf("Z-Orientation (degrees): ");
            if (scanf("%d", &zTheta) == 0) return 1;

            printCube(length, xTheta, yTheta, zTheta);
        }

        printf("\nPrint another cube? (y/n): ");
        if (scanf("%s", &printAgain) == 0 || printAgain == EOF) return 0;
    }
}