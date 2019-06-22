/*
 * Hayden Pilsner
 * 22 Jun 2019
 * Sucky Succy
 * NodeMCU ESP 12-E Telemetry
 * 
 * This code is used to gather data from a moisture,
 * temperature and uv sensor and sends it to the Google Cloud
 * IoT service.
 */
 
#include <Wire.h>
#include "Adafruit_VEML6075.h"

Adafruit_VEML6075 uv = Adafruit_VEML6075();
int analogPin = A0; // pin number for analog in
int muxSelectPins[4] = {14, 12, 13, 15};
int tempPin = 0;
int moisturePin = 1;

void setup() {
  for (int i = 0; i < 4; i++) {  // setup digital multiplexer pins
    pinMode(muxSelectPins[i], OUTPUT);
    digitalWrite(muxSelectPins[i], HIGH);
  }
  // setup analog input pin
  pinMode(analogPin, INPUT);

  // setup serial output for debugging
  Serial.begin(9600);
  while (!Serial) { delay(10); }
  Serial.println("VEML6075 Simple Test");
  if (! uv.begin()) {
    Serial.println("Failed to communicate with VEML6075 sensor, check wiring?");
    while (1) { delay(100); }
  }
  Serial.println("Found VEML6075 sensor");
}

void loop() {
  // read UV Index
  Serial.print("UV Index reading: ");
  Serial.println(uv.readUVI());
  
  // select and read temperature analog
  pullAnalogMux(0);
  float temp = (getTemp());
  Serial.print("Temperature reading: ");
  Serial.println(temp);

  // select and read moisture analog
  pullAnalogMux(1);
  float moistIndex = analogRead(analogPin);
  Serial.print("Moisture Index reading: ");
  Serial.println(moistIndex);
  
  delay(1000);
}

float getTemp() {
  float voltage = analogRead(analogPin) * 3;
  voltage /= 1024.0;
  return (voltage - 0.5) * 100;
}

void pullAnalogMux(int pin) {
  for (int i = 0; i < 4; i++) {
    if (pin & (1 << i)) digitalWrite(muxSelectPins[i], HIGH);
    else digitalWrite(muxSelectPins[i], LOW);
  }
}
