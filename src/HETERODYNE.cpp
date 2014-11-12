/*
 * HETERODYNE.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: nemo
 */

#include "HETERODYNE.h"

HETERODYNE::HETERODYNE(int fs, int fh) {
	this->Fs = fs;
	this->Fh = (float)fh;
	this->Ts = 1./this->Fs;
	this->ph = 0;
	this->t = 0;
}

HETERODYNE::~HETERODYNE() {
}

void HETERODYNE :: make(fftwf_complex *b, int size){
	fftwf_complex a;
	fftwf_complex aa;

	for(int i=0; i<size; i++){
		a[0] = cos(2.*PI*this->Fh*this->t);
		a[1] = sin(2.*PI*this->Fh*this->t);

		aa[0] = b[i][0]*a[0] - b[i][1]*a[1];
		aa[1] = b[i][0]*a[1] + b[i][1]*a[0];

		b[i][0] = aa[0];
		b[i][1] = aa[1];

		this->t += this->Ts;
	}
}
