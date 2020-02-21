/*
   Detector Building v2
   Code by Anthony Wang
   Ladue High School Science Olympiad
*/

/*
  TODO:
  New calibration method using cubic regression
*/

#include <curveFitting.h>
#include <detectorBuilding.h>


const int n = 30; // Number of data points
const int m = 1; // Number of segments
double V[n] = { // Voltage measurements
  2.12, 3.26, 3.96
};
double T[n] = { // Temperature measurements
  22.0, 39.15, 60
};

void loop() {

}

void setup() {
  Serial.begin(9600);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}
