/*
  detectorBuilding.h - Library for Detector Building utility functions
  Created by Anthony Wang, February 21, 2020.
  Released into the public domain.
*/

#ifndef detectorBuilding_h
#define detectorBuilding_h

#include "Arduino.h"

inline double f2c(double f);
inline double c2f(double c);
inline double k2c(double k);
inline double c2k(double c);
inline double f2k(double f);
inline double k2f(double k);

inline double a2d(int a);
inline int d2a(double d);

void sort(double a[], int n);

void calculate();

#endif