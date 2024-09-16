#include "mpg2km.h"

float mpg2kml(float mpg) {
    if (mpg <= 0) return 0.0; // Handle invalid arguments
    return mpg * 1.60934 / 3.78541; //conversion factor between miles and kilometers.
}

float mpg2lphm(float mpg) {
    if (mpg <= 0) return 0.0; // Handle invalid arguments
    return 235.214 / mpg; //miles per gallon (MPG), MPG to L/100km,
}

float lph2mpg(float lph) {
    if (lph <= 0) return 0.0; // Handle invalid arguments, L/100km to MPG, t
    return 235.214 / lph;
}
