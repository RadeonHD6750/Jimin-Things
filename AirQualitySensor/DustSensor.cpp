#include "DustSensor.h"

float DustSensor::mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float DustSensor::getVoltage()
{
    digitalWrite(ledPower, LOW); // power on the LED

    delayMicroseconds(samplingTime);
    float voMeasured = analogRead(measurePin); // read the dust value
    delayMicroseconds(deltaTime);

    digitalWrite(ledPower, HIGH); // turn the LED off
    delayMicroseconds(sleepTime);

    // 0 - 5V mapped to 0 - 1023 integer values
    // recover voltage
    float calcVoltage = this->mapf(voMeasured, 0, 1023, 0, 5);

    return calcVoltage;
}

float DustSensor::getDust(float voltage)
{
    return (calcVoltage - this->DUST_CLEAN_VOLTAGE) / 0.005f;
}

float DustSensor::getMeasuring()
{
    float v = this->getVoltage();

    return this->getDust(v);
}
