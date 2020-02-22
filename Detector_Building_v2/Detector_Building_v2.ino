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

const bool CALIB = false; // Calibration mode
const int n = 4; // Number of data points
const int m = 1; // Number of segments
const int deg = 2; // Regression degree
double V[n] = { // Voltage measurements
  2.12, 3.26, 3.96, 4.1
};
double T[n] = { // Temperature measurements
  22.0, 39.15, 60, 72
};
double coeff[m][deg + 1];


void setup() {
  Serial.begin(9600);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  sort(V, n);
  sort(T, n);

  double x[n], y[n];
  for (int i = 0; i < n; i++) x[i] = log(v2r(V[i])) - 5;
  for (int i = 0; i < n; i++) y[i] = 1 / c2k(T[i]);
  /*for (int i = 0; i < n; i++) {
    Serial.print("(");
    Serial.print(x[i], 12);
    Serial.print(", ");
    Serial.print(y[i], 12);
    Serial.print(")");
    Serial.println();
  }*/
  for (int i = 0; i < m; i++) {
    int ret = fitCurve(deg, n / m, x + i * n / m, y + i * n / m, deg + 1, coeff[i]);
    /*if (ret == 0){ //Returned value is 0 if no error
      char c = 'A';
      Serial.println("Coefficients are:");
      for (int j = 0; j <= deg; j++){
        Serial.print(c++);
        Serial.print(": ");
        Serial.print(coeff[i][j], 12);
        Serial.println();
      }
    }*/
  }
}


void loop() {
  int V_raw = analogRead(THERM); // Read in raw analog value
  double V_out = a2d(V_raw);
  if (CALIB) {
    // Calibration mode
    Serial.print("Raw analog reading: ");
    Serial.print(V_raw);
    Serial.print("  Voltage (V): ");
    Serial.print(V_out);
    Serial.println();
    delay(500);
    return;
  }

  int s = 0;
  while (s + 1 < m && V_out < (V[s * n / m - 1] + V[s * n / m]) / 2) s++; // Find correct segment
  
  double logR = log(v2r(V_out)) - 5;
  double sum = 0, prod = 1;
  for (int i = 0; i <= deg; i++) {
    sum += coeff[s][deg - i] * prod;
    prod *= logR;
  }
  double K = 1 / sum;
  double C = k2c(K);
  double F = c2f(C);


  // LED stuff
  if (C <= 25) { // Cold
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, HIGH);
  }
  else if (C <= 50) { // Medium
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, LOW);
  }
  else if (C <= 75) { // Hot
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
  }
  else { // Something seriously wrong
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
  }
  
  
  // Output voltage, temperature
  Serial.print("Raw analog reading: ");
  Serial.print(V_raw);
  Serial.print(" Voltage (V): ");
  Serial.print(V_out);
  Serial.print(" Temperature (°C): ");
  Serial.print(C);
  // For reference
  /*Serial.print(" Temperature (°F): ");
  Serial.print(F);
  Serial.print(" s: ");
  Serial.print(s);
  Serial.print(" logR: ");
  Serial.print(logR);*/
  Serial.println();
  delay(500);
  return;
}
