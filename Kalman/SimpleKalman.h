#ifndef KALMAN_H
#define KALMAN_H

#include <Arduino.h>

/*
2023-11-07

초간단 칼만필터

*/

class SimpleKalman {
private:
	float A = 1; //시스템 행렬 과거값 반영비율
	float H = 1;  //출력 행렬
	float Q = 2;  //과거값 반영비율
	float R = 100;  //측정 오차
	float P = 3; //초기 공분산
	float X = 0; //최초값

public:
	SimpleKalman();
	SimpleKalman(float A, float H, float Q, float R, float P, float X);

	void setInitValue(float X);
	float getInitValue();

	float FilterUpdate(float measured_value);
};


#endif