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

        //Animate or Print Cube
        if (userChoice == 1) {
            animateCube();
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