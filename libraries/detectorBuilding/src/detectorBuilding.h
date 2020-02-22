/*
  detectorBuilding.h - Library for Detector Building utility functions
  Created by Anthony Wang, February 21, 2020.
  Released into the public domain.
*/

#ifndef detectorBuilding_h
#define detectorBuilding_h

#include "Arduino.h"

const int LED_R = 4, LED_G = 3, LED_B = 2, THERM = 0; // Device component pins
const double R_k = 10000, V_in = 5, analog_max = 1023; // Device constants

inline double f2c(double f);
inline double c2f(double c);
inline double k2c(double k);
inline double c2k(double c);
inline double f2k(double f);
inline double k2f(double k);

inline double a2d(int a);
inline int d2a(double d);

inline double v2r(double V_out);

void sort(double a[], int n);

//void calculate();

#endif