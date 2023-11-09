#ifndef KALMAN_H
#define KALMAN_H

#include <Arduino.h>

/*
2023-11-07



*/

class SimpleKalman {
private:
	float A = 1; 
	float H = 1; 
	float Q = 0.2;  
	float R = 10;  
	float P = 4; 
	float X = 0; 

public:
	SimpleKalman();
	SimpleKalman(float X);
	SimpleKalman(float Q, float R, float P, float X);
	SimpleKalman(float A, float H, float Q, float R, float P, float X);

	void setInitValue(float X);
	float getInitValue();

	float FilterUpdate(float measured_value);
};


#endif