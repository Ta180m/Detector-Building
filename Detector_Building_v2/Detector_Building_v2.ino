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
const int n = 17; // Number of data points
const int m = 1; // Number of segments
const int deg = 2; // Regression degree
double V[n] = { // Voltage measurements
  2.70, 3.39, 2.40, 2.31, 2.19, 1.94, 4.09, 4.11, 3.98, 3.92, 3.77, 3.53, 3.18, 3.07, 2.30, 2.53, 2.49
};
double T[n] = { // Temperature measurements
  24.0, 44.4, 18.4, 13.9, 11.2, 8.6, 60.8, 62.0, 58.3, 53.9, 49.3, 44.9, 41.0, 37.3, 13.4, 19.1, 18.2
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
  for (int i = 0; i < n; i++) x[i] = log(v2r(V[i])) - 7;
  for (int i = 0; i < n; i++) y[i] = 1000 / c2k(T[i]);
  for (int i = 0; i < n; i++) {
    Serial.print("(");
    Serial.print(x[i], 12);
    Serial.print(", ");
    Serial.print(y[i], 12);
    Serial.print(")");
    Serial.println();
  }
  for (int i = 0; i < m; i++) {
    int ret = fitCurve(deg, n / m, x + i * n / m, y + i * n / m, deg + 1, coeff[i]);
    if (ret == 0){ //Returned value is 0 if no error
      char c = 'A';
      Serial.println("Coefficients are:");
      for (int j = 0; j <= deg; j++){
        Serial.print(c++);
        Serial.print(": ");
        Serial.print(coeff[i][j], 12);
        Serial.println();
      }
    }
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
  
  double logR = log(v2r(V_out)) - 7;
  double sum = 0, prod = 1;
  for (int i = 0; i <= deg; i++) {
    sum += coeff[s][deg - i] * prod;
    prod *= logR;
  }
  double K = 1000 / sum;
  double C = k2c(K);
  double F = c2f(C);


  // LED stuff
  if (C <= 30) { // Cold
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
