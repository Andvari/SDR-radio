/*
 * DEEMPHASIS.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: nemo
 */

#include "DEEMPHASIS.h"

DEEMPHASIS::DEEMPHASIS(int R, double C) {
	this->tau = R*C;
	this->y[0] = 0;
	this->y[1] = 0;
}

DEEMPHASIS::DEEMPHASIS(double tau) {
	this->tau = tau;
	this->y[0] = 0;
	this->y[1] = 0;
}

DEEMPHASIS::~DEEMPHASIS() {
}

void DEEMPHASIS :: make(BUFFER *buf){
	double Ts;
	double a;

	Ts = 1./buf->getFs();

	a = Ts / (this->tau + Ts);
	fftwf_complex *b;
	int size;

	b = buf->getB();
	size = buf->getSize();



	b[0][0]  = a * b[0][0] + (1-a) * this->y[0];
	b[0][1]  = a * b[0][1] + (1-a) * this->y[1];

	for(int i=1; i<size; i++){
		b[i][0]  = b[i-1][0] + a * (b[i][0] - b[i-1][0]);
		b[i][1]  = b[i-1][1] + a * (b[i][1] - b[i-1][1]);
	}

	this->y[0] = b[size-1][0];
	this->y[1] = b[size-1][1];
}
