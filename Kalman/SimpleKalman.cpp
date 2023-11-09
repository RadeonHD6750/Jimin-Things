/*
2023-11-07

초간단 칼만필터

*/

#include "SimpleKalman.h"

SimpleKalman::SimpleKalman()
{
}

SimpleKalman::SimpleKalman(float A, float H, float Q, float R, float P, float X)
{
	this->A = A;
	this->H = A;
	this->Q = Q;
	this->R = R;
	this->P = P;
	this->X = X;
}

void SimpleKalman::setInitValue(float X)
{
	this->X = X;
}

float SimpleKalman::getInitValue()
{
	return this->X;
}

float SimpleKalman::FilterUpdate(float measured_value)
{
	//추정값과 오차 공분산 예측
	float x_processed = this->A * measured_value;
	float p_processed = (this->A * this->P / this->A) + this->Q;

	//칼만 이득 계산
	float K = (p_processed / this->H) / (this->H * p_processed / this->H + this->R);

	//추정값 계산
	float estimate = x_processed + K * (measured_value - this->H * x_processed);

	//오차 공분산 계산
	this->P = p_processed - (K * this->H * p_processed);

	return estimate;
}
