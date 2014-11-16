/*
 * PLL.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: nemo
 */

#include "PLL.h"

#define	PI	3.14159265356

PLL::PLL(int fg) {
	this->t = 0;
	this->p = 0;
	this->Fget = fg;
}

PLL::~PLL() {
}

void PLL :: make(BUFFER *buf){
	fftwf_complex *b;
	int size;

	size = buf->getSize();
	b = buf->getB();

	float v[2];
	float gc;
	float gs;

	float p;
	float Ts;

	Ts = 1./(double)(buf->getFs());

	for(int i=0; i<size; i++){

		gc = cos(2.0*PI*this->Fget*this->t + this->p);
		gs = sin(2.0*PI*this->Fget*this->t + this->p);

		v[0] =   b[i][0] * gc + b[i][1] * gs;
		v[1] =  -b[i][0] * gs + b[i][1] * gc;

		p = atan2(v[1], v[0]);

		this->p += p;

		b[i][0] = p;
		b[i][1] = 0;
		this->t += Ts;

 	}
}

