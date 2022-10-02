#include "platonicPrinter.h"

int main(void) {

    char printAgain = 'y';

    while (printAgain == 'y') {

        printf("\n                      ╔══════════════════╗\n");
        printf(  "══════════════════════╣ PLATONIC PRINTER ╠═════════════════════\n");
        printf(  "                      ╚══════════════════╝\n");   
        printf("Animate a random rotation   -OR-   Print a user-input platonic solid\n\n");

        int userChoice = 0, solidChoice = 0;

        while (userChoice != 1 && userChoice != 2) {
            printf("\nAnimate (1) or Print Solid (2): ");
            scanf("%d", &userChoice);
        }

        printf("\nSelect a Platonic Solid:\n");
        printf("1. Cube\n");
        printf("2. Tetrahedron\n");
        printf("3. Octahedron\n");
        printf("4. Icosahedron\n");
        printf("5. Dodecohedron\n\n");

        while (solidChoice < 1 || solidChoice > 5) {
            printf("Solid Choice: ");
            scanf("%d", &solidChoice);
        }

        //Animate cube
        if (userChoice == 1) {


            int fps, seconds, length = 0;

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

            animateSolid(solidChoice, length, seconds, fps);

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

            printSolid(solidChoice, length, xTheta, yTheta, zTheta);
        }

        printf("\nPrint another solid? (y/n): ");
        scanf("%s", &printAgain);
    }
}