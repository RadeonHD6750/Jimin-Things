#ifndef DUST_H
#define DUST_H

#include <Arduino.h>

class DustSensor
{
private:
	//signal
	const int DHT11PIN = 2;

	//led
	const int ledPower = 3;

	// Dust Clean Voltage Init Value
	const float DUST_CLEAN_VOLTAGE = 0.60;

	const int samplingTime = 280;
	const int deltaTime = 40;
	const int sleepTime = 9680;


public:
	float mapf(float x, float in_min, float in_max, float out_min, float out_max);
	float getVoltage();
	float getDust(float voltage);
	float getMeasuring();
};

#endif
