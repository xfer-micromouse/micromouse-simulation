#include <Arduino.h>

const int SENSOR_PIN = A2;
const int MOTOR_PIN_A = 10;
const int MOTOR_PIN_B = 9;

void setup() {
    Serial.begin(9600);
    // No pinMode needed for A2
    pinMode(MOTOR_PIN_A, OUTPUT);
    pinMode(MOTOR_PIN_B, OUTPUT);
}

void loop() {
    int sensorVal = analogRead(SENSOR_PIN);

    // Convert 0-1023 to 0-255
    int pwmOut = sensorVal / 4;

    // Optional: Keep the motor from stalling at low power
    if (pwmOut < 30 && pwmOut > 0) pwmOut = 0;

    Serial.print("Sensor: ");
    Serial.print(sensorVal);
    Serial.print(" | PWM: ");
    Serial.println(pwmOut);

    analogWrite(MOTOR_PIN_A, pwmOut);
    analogWrite(MOTOR_PIN_B, pwmOut);

    delay(50); // Slightly longer delay for Serial stability
}