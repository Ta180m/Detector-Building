/*
  detectorBuilding.cpp - Library for Detector Building utility functions
  Created by Anthony Wang, February 21, 2020.
  Released into the public domain.
*/


#include "Arduino.h"
#include "detectorBuilding.h"


// Temperature conversions
double f2c(double f) { return (f - 32) * 5 / 9; } // Fahrenheit to Celsius
double c2f(double c) { return c * 9 / 5 + 32; } // Celsius to Fahrenheit
double k2c(double k) { return k - 273.15; } // Kelvin to Celsius
double c2k(double c) { return c + 273.15; } // Celsius to Kelvin
double f2k(double f) { return c2k(f2c(f)); } // Fahrenheit to Kelvin
double k2f(double k) { return c2f(k2c(k)); } // Kelvin to Fahrenheit


// Analog to digital conversion
double a2d(int a) { return V_in * a / analog_max; }
int d2a(double d) { return d * analog_max / V_in; }


// Voltage to resistance conversion
double v2r(double V_out) { return R_k * (V_in / V_out - 1); }


// Utility functions
// No C++ standard library :(
void sort(double a[], int n) {
  // Bubble sort
  // Slow but n < 30 so OK
  // Too lazy to implement a fast sort
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n - 1; j++) {
      if (a[j] > a[j + 1]) {
        double tmp = a[j];
        a[j] = a[j + 1];
        a[j + 1] = tmp;
      }
    }
  }
}


// Calculations: Steinhart-hart stuff
// Probably unused but can include anyways
/*void calculate() {
  sort(V, n);
  sort(T, n);
  double R[n], L[n], Y[n], G[n];
  for (int i = 0; i < n; i++) R[i] = R_k * (V_in / V[i] - 1);
  for (int i = 0; i < n; i++) L[i] = log(R[i]);
  for (int i = 0; i < n; i++) Y[i] = 1 / c2k(T[i]);
  for (int i = 0; i < n; i += 3) {
    G[i + 1] = (Y[i + 1] - Y[i]) / (L[i + 1] - L[i]);
    G[i + 2] = (Y[i + 2] - Y[i]) / (L[i + 2] - L[i]);
  }
  for (int i = 0; i < n; i += 3) { // Don't ask how this works
    C[i / 3] = (G[i + 2] - G[i + 1]) / (L[i + 2] - L[i + 1]) / (L[i] + L[i + 1] + L[i + 2]);
    B[i / 3] = G[i + 1] - C[i / 3] * (L[i] * L[i] + L[i] * L[i + 1] + L[i + 1] * L[i + 1]);
    A[i / 3] = Y[i] - L[i] * (B[i / 3] + L[i] * L[i] * C[i / 3]);
  }
  for (int i = 0; i < n; i += 3) V_mid[i / 3] = (i ? (V[i - 1] + V[i]) / 2 : V[i]);
}*/