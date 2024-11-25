#include <M5StickC.h>
#include "BluetoothSerial.h"

long previousMicros = 0, previousMillis = 0, lastSampleTime = 0;
double interval = 0, RPM = 0;
int value = 0, count = 0, previousValue = 0;  
bool flag = false;

#define shutdown_pin 10 
#define threshold 100 // to identify R peak
#define timer_value 10000 // 10 seconds timer to calculate hr
#define sample_interval 167 // Interval in milliseconds for 6 Hz sampling

void setup() {
  Serial.begin(9600);
  pinMode(GPIO_NUM_36, INPUT);
}

void loop() { 
  long currentMillis = millis();
  
  // Check if it's time to take a new sample
  if (currentMillis - lastSampleTime >= sample_interval) {
    lastSampleTime = currentMillis;
    
    value = analogRead(GPIO_NUM_36);
    value = map(value, 250, 400, 0, 100); // Flatten the ECG values a bit

    if ((value - previousValue > threshold) && (!flag)) {
      count++;
      flag = true;
      interval = currentMillis - previousMicros; // RR interval
      previousMicros = currentMillis; 
    } else if (value < previousValue) {
      flag = false;
    }

    if ((currentMillis - previousMillis) > timer_value) {
      RPM = count * 6; // Calculate RPM (respirations per minute)
      previousMillis = currentMillis;
      count = 0; 
    }
    
    previousValue = value;

    // Output the values
    Serial.print("RPM: ");
    Serial.print(RPM);
    Serial.print(", Interval: ");
    Serial.print(interval);
    Serial.print(", Value: ");
    Serial.println(value);
  }
}
