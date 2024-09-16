/*
* learnc.c / Practice Assignment
*
* YinSun / CS5600 / Northeastern University
* Fall 2024 / Sep 15, 2024
*
*/

#include <stdio.h>

// Function prototypes
float mpg2kml(float mpg);
float mpg2lphm(float mpg);
float lph2mpg(float lph);

int main() {
    // Test cases
    float test_mpg = 25.0;
    float test_lph = 9.41;
//%.2f display the floating-point number rounded to two decimal places.
    printf("25 MPG is %.2f kilometers per liter\n", mpg2kml(test_mpg));
    printf("25 MPG is %.2f liters per 100 kilometers\n", mpg2lphm(test_mpg));//
    printf("9.41 liters per 100 kilometers is %.2f MPG\n", lph2mpg(test_lph));

    // Bad argument testing
    printf("Testing with a bad argument (negative MPG): %.2f\n", mpg2kml(-25.0));

    return 0;
}
