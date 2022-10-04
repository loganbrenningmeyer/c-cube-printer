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
            if (scanf("%d", &userChoice) == 0) {
                return 1;
            } else if (userChoice == EOF) {
                return 0;
            }
        }

        printf("\nSelect a Platonic Solid:\n");
        printf("1. Cube\n");
        printf("2. Tetrahedron\n");
        printf("3. Octahedron\n");
        printf("4. Icosahedron\n");
        printf("5. Dodecohedron\n\n");

        while (solidChoice < 1 || solidChoice > 5) {
            printf("Solid Choice: ");
            if (scanf("%d", &solidChoice) == 0) return 1;
        }

        //Animate cube
        if (userChoice == 1) {


            int fps, seconds, length = 0;

            //Take animation duration as input
            printf("\nDuration (s): ");
            if (scanf("%d", &seconds) == 0) return 1;

            //Take fps as input
            printf("FPS: ");
            if (scanf("%d", &fps) == 0) return 1;

            //Take length as input
            while (length > 10 || length < 1) {
                printf("Cube Length (1-10): ");
                if (scanf("%d", &length) == 0) return 1;
            }

            animateSolid(solidChoice, length, seconds, fps);

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

            printSolid(solidChoice, length, xTheta, yTheta, zTheta);
        }

        printf("\nPrint another solid? (y/n): ");
        if (scanf("%s", &printAgain) == 0) {
            return 1;
        } else if (printAgain == EOF) {
            return 0;
        }
    }
}