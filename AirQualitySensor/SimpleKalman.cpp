/*
2023-11-07


*/

#include "SimpleKalman.h"

SimpleKalman::SimpleKalman()
{
}

SimpleKalman::SimpleKalman(float X)
{
	this->X = X;
}

SimpleKalman::SimpleKalman(float Q, float R, float P, float X)
{
	this->Q = Q;
	this->R = R;
	this->P = P;
	this->X = X;
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

	float x_processed = this->A * measured_value;
	float p_processed = (this->A * this->P / this->A) + this->Q;

	
	float K = (p_processed / this->H) / (this->H * p_processed / this->H + this->R);


	float estimate = x_processed + K * (measured_value - this->H * x_processed);

	
	this->P = p_processed - (K * this->H * p_processed);

	return estimate;
}
