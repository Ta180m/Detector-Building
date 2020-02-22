/*
  detectorBuilding.h - Library for Detector Building utility functions
  Created by Anthony Wang, February 21, 2020.
  Released into the public domain.
*/

#ifndef detectorBuilding_h
#define detectorBuilding_h

#include "Arduino.h"

const int LED_R = 8, LED_G = 10, LED_B = 12, THERM = 0; // Device component pins
const double R_k = 10000, V_in = 5, analog_max = 1023; // Device constants

double f2c(double f);
double c2f(double c);
double k2c(double k);
double c2k(double c);
double f2k(double f);
double k2f(double k);

double a2d(int a);
int d2a(double d);

double v2r(double V_out);

void sort(double a[], int n);

//void calculate();

#endif