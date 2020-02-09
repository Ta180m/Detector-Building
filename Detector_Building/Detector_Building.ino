/*
 * Detector Building
 * Code by Anthony Wang
 * Ladue High School Science Olympiad
 */


/*
 * TODO:
 * BUG FIXING: Expect bugs!
 */


// Constants
const bool CALIB = true; // Calibration mode
const int LED_R = 4, LED_G = 3, LED_B = 2, THERM = 0; // Device component pins
const double R_k = 10000, V_in = 5, analog_max = 1023; // Device constants


// Temperature conversions
inline double f2c(double f) { return (f - 32) * 5 / 9; } // Fahrenheit to Celsius
inline double c2f(double c) { return c * 9 / 5 + 32; } // Celsius to Fahrenheit
inline double k2c(double k) { return k - 273.15; } // Kelvin to Celsius
inline double c2k(double c) { return c + 273.15; } // Celsius to Kelvin
inline double f2k(double f) { return c2k(f2c(f)); } // Fahrenheit to Kelvin
inline double k2f(double k) { return c2f(k2c(k)); } // Kelvin to Fahrenheit


// Analog to digital conversion
inline double a2d(int a) { return V_in * a / analog_max; }
inline int d2a(double d) { return d * analog_max / V_in; }

  
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


// Calibration data
const int n = 3, m = n / 3; // Number of data points, MUST be multiple of 3
double V[n] = { 2.5, 3.26, 3.96 }; // Voltage measurements
double T[n] = { 25, 39.15, 60 }; // Temperature measurements
double V_mid[m]; // Stores each piecewise segment for binary search
double A[m], B[m], C[m]; // Coefficients for each piecewise component


// Calculations
// Steinhart-hart stuff
void calculate() {
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
}


// Arduino stuff
void blink(int pin) {
  digitalWrite(pin, HIGH);
  delay(1000);
  digitalWrite(pin, LOW);  
}
void setup() {
  Serial.begin(9600);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // blink(LED_R);
  // blink(LED_G);
  // blink(LED_B);

  calculate();

  // Debug stuff
  /*for (int i = 0; i < n; i++) {
    Serial.print(V[i]);
    Serial.print(" ");
    Serial.print(T[i]);
    Serial.print(" ");
  }
  Serial.println();
  for (int i = 0; i < m; i++) {
    Serial.print("Segment lower bound: ");
    Serial.print(i ? V_mid[i - 1] : 0);
    Serial.print(" Segment upper bound: ");
    Serial.print(V_mid[i]);
    Serial.print(" A: ");
    Serial.print(A[i], 12);
    Serial.print(" B: ");
    Serial.print(B[i], 12);
    Serial.print(" C: ");
    Serial.print(C[i], 12);
    Serial.println();
  }*/
}


// Main loop
void loop() {
  int V_raw = analogRead(THERM); // Read in raw analog value
  double V_out = a2d(V_raw); // Convert analog to digital
  if (CALIB) {
      Serial.print("Raw analog reading: ");
      Serial.print(V_raw);
      Serial.print("  Voltage (V): ");
      Serial.print(V_out);
      Serial.println();
      delay(500);
      return;
  }
  double R_t = R_k * (V_in / V_out - 1); // Thermistor resistance

  int s = 0;
  while (s < m && V_out > V_mid[s + 1]) s++; // Find correct segment
  
  double logR_t = log(R_t);
  double K = 1.0 / (A[s] + B[s] * logR_t + C[s] * logR_t * logR_t * logR_t); // Steinhart-hart
  double C = k2c(K);
  double F = c2f(C);

  // LED stuff
  if (C <= 25) {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, HIGH);
  }
  else if (C <= 50) {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, LOW);
  }
  else if (C <= 75) {
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
  }
  else {
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
  Serial.print(" Temperature (°F): "); // For reference
  Serial.print(F);
  
  // Debug stuff
  /*Serial.print(" Segment lower bound: ");
  Serial.print(s ? V_mid[s - 1] : 0);
  Serial.print(" Segment upper bound: ");
  Serial.print(V_mid[s]);*/

  Serial.println();
  delay(500);
  return;
}
