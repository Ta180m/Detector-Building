/*
  ______  _______ _______ _______ _______ _______  _____   ______       
  |     \ |______    |    |______ |          |    |     | |_____/       
  |_____/ |______    |    |______ |_____     |    |_____| |    \_       
                                                                        
  ______  _     _ _____ ______  _____ __   _  ______      _    _ _______
  |_____] |     |   |   |     \   |   | \  | |  ____       \  /  ______|
  |_____] |_____| __|__ |_____/ __|__ |  \_| |_____|        \/   |______
                                                                       
   Code by Anthony Wang
   Ladue High School Science Olympiad
*/

/*
  TODO:
  Numerical precision
*/

#include <curveFitting.h>
#include <detectorBuilding.h>

const bool CALIB = false; // Calibration mode
const int n = 4; // Number of data points
const int m = 1; // Number of segments
const int deg = 3; // Regression degree
ld data[2 * n] = {
//  V    T
  2.70, 24.0,
  2.90, 30.0,
  3.10, 36.0,
  3.50, 50.0
};
ld coeff[m][deg + 1], V[n], T[n];


void setup() {
  Serial.begin(9600);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  for (int i = 0; i < n; i++) {
    V[i] = data[2 * i];
    T[i] = data[2 * i + 1];
  }
  
  sort(V, n);
  sort(T, n);

  ld x[n], y[n];
  for (int i = 0; i < n; i++) x[i] = log(v2r(V[i])) - 7;
  for (int i = 0; i < n; i++) y[i] = 1000 / c2k(T[i]);
  /*for (int i = 0; i < n; i++) {
    Serial.print("{");
    Serial.print((double)x[i], 12);
    Serial.print(", ");
    Serial.print((double)y[i], 12);
    Serial.print("},");
    Serial.println();
  }*/
  for (int i = 0; i < m; i++) {
    int ret = fitCurve(deg, n/m, x+i*n/m, y+i*n/m, deg + 1, coeff[i]);
    /*if (ret == 0) { // Returned value is 0 if no error
      char c = 'A';
      Serial.println("Coefficients are:");
      for (int j = 0; j <= deg; j++){
        Serial.print(c++);
        Serial.print(": ");
        Serial.print((double)coeff[i][j], 12);
        Serial.println();
      }
    }*/
  }
}


void loop() {
  int V_raw = analogRead(THERM); // Read in raw analog value
  ld V_out = a2d(V_raw);
  if (CALIB) {
    // Calibration mode
    Serial.print("Raw analog reading: ");
    Serial.print(V_raw);
    Serial.print("  Voltage (V): ");
    Serial.print((double)V_out);
    Serial.println();
    delay(500);
    return;
  }

  int s = 0; // Find correct segment
  while (s + 1 < m && V_out < (V[s*n/m-1] + V[s*n/m]) / 2) s++;
  
  ld logR = log(v2r(V_out)) - 7;
  ld sum = 0, prod = 1;
  for (int i = 0; i <= deg; i++) {
    sum += coeff[s][deg - i] * prod;
    prod *= logR;
  }
  ld K = 1000 / sum;
  ld C = k2c(K);
  ld F = c2f(C);


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
  Serial.print((double)V_raw);
  Serial.print(" Voltage (V): ");
  Serial.print((double)V_out);
  Serial.print(" Temperature (°C): ");
  Serial.print((double)C);
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
