#include <Arduino.h>
#include <SparkFunCCS811.h>
#include <Wire.h>

#define LED_BUILTIN 0x02
#define CCS811_ADDR1 0x5A
#define CCS811_ADDR2 0x5B

CCS811 sensor(CCS811_ADDR1);

void setupSerial();
void setupSensor();
void errorLeds();

String output;

void setup()
{
    setupSerial();
    setupSensor();
}

void loop()
{
    if (sensor.dataAvailable()) {
        const auto timestamp = millis();
        if (sensor.readAlgorithmResults() != CCS811Core::CCS811_Stat_SUCCESS) {
            Serial.println("Failed to read results");
            return;
        }
        output = "\nMeasure timestamp: " + String(timestamp);
        output += "\nCO2: " + String(sensor.getCO2());
        output += "\nTVOC: " + String(sensor.getTVOC());
        Serial.println(output);
    }
    delay(1000); // Delay between measurement
}

void setupSerial()
{
    Serial.begin(115200);
    while (!Serial) { };
}

void setupSensor()
{
    Wire.begin();

    if (sensor.begin() == false) {
        Serial.println("CCS811 error. Please check wiring. Freezing...");
        while (true) {
            errorLeds();
        };
    }
}

void errorLeds()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}