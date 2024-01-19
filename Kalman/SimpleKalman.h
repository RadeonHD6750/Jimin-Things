#ifndef KALMAN_H
#define KALMAN_H

#include <Arduino.h>

/*
2023-11-07

�ʰ��� Į������

*/

class SimpleKalman {
private:
	float A = 1; //�ý��� ��� ���Ű� �ݿ�����
	float H = 1;  //��� ���
	float Q = 2;  //���Ű� �ݿ�����
	float R = 100;  //���� ����
	float P = 3; //�ʱ� ���л�
	float X = 0; //���ʰ�

public:
	SimpleKalman();
	SimpleKalman(float A, float H, float Q, float R, float P, float X);

	void setInitValue(float X);
	float getInitValue();

	float FilterUpdate(float measured_value);
};


#endif