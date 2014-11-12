/*
 * DEEMPHASIS.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: nemo
 */

#include "DEEMPHASIS.h"

DEEMPHASIS::DEEMPHASIS(int fs, int R, double C) {
	this->Ts = 1./fs;
	this->tau = R*C;
}

DEEMPHASIS::DEEMPHASIS(int fs, double tau) {
	this->Ts = 1./fs;
	this->tau = tau;
}

DEEMPHASIS::~DEEMPHASIS() {
}

void DEEMPHASIS :: make(fftwf_complex *b, int size){
		double a = this->Ts / (this->tau + this->Ts);

	b[0][0]  = a * b[0][0] + (1-a)*this->y[0];
	b[0][1]  = a * b[0][1] + (1-a)*this->y[1];

	for(int i=1; i<size; i++){
		b[i][0]  = b[i-1][0] + a * (b[i][0] - b[i-1][0]);
		b[i][1]  = b[i-1][1] + a * (b[i][1] - b[i-1][1]);
	}

	this->y[0] = b[size-1][0];
	this->y[1] = b[size-1][1];
}
