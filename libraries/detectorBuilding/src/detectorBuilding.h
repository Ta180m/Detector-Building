/*
  detectorBuilding.h - Library for Detector Building utility functions
  Created by Anthony Wang, February 21, 2020.
  Released into the public domain.
*/

#ifndef detectorBuilding_h
#define detectorBuilding_h

#include "Arduino.h"
typedef long double ld;

const int LED_R = 8, LED_G = 10, LED_B = 12, THERM = 0; // Device component pins
const ld R_k = 10000, V_in = 5, analog_max = 1023; // Device constants

ld f2c(ld f);
ld c2f(ld c);
ld k2c(ld k);
ld c2k(ld c);
ld f2k(ld f);
ld k2f(ld k);

ld a2d(int a);
int d2a(ld d);

ld v2r(ld V_out);

void sort(ld a[], int n);

//void calculate();

#endif