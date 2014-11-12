/*
 * PLL.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: nemo
 */

#include "PLL.h"


PLL::PLL(int fs, int fg, int wp, float kd, float ko, float zeta) {
	this->kd   = kd;
	this->ko   = ko;

	this->wp   = 2.0*PI*wp;
	this->zeta = zeta;

	this->Fs = fs;
	this->Ts = 1.0/fs;

	float g1 = 2.0*(1.0 - exp(-wp*zeta*this->Ts)*cos(this->wp*sqrt(1.0-this->zeta*this->zeta)*this->Ts));
	float g2 = exp(-2.0*this->wp*this->zeta*this->Ts)-1.0+g1;

	this->ki = g2/(this->ko*this->kd);
	this->kp = g1/(this->ko*this->kd);

	this->t = 0;
	this->Fget = fg;
	this->p = 0;
	this->v[0] = 0;
	this->v[1] = 0;
	this->e[0] = 0;
	this->e[1] = 0;
}

PLL::~PLL() {
}

void PLL :: make(fftwf_complex *b, int size){
/*
	float v;
	float e;

	for(int i=0; i<size; i++){
		v = this->kd * b[i][1] * cos(2.0*PI*this->Fget*this->t + this->p);
		e = this->kp*v + (this->ki-this->kp)*this->v + this->e;
		this->p = this->ko * this->e + this->p;
		this->e = e;
		b[i][0] = this->e;
		b[i][1] = 0;
		this->t += this->Ts;
	}
*/

	float v[2];
	float gc;
	float gs;

	float p;

	for(int i=0; i<size; i++){

		gc = cos(2.0*PI*this->Fget*this->t + this->p);
		gs = sin(2.0*PI*this->Fget*this->t + this->p);

		v[0] =   b[i][0] * gc + b[i][1] * gs;
		v[1] =  -b[i][0] * gs + b[i][1] * gc;

		p = atan2(this->v[1] + v[1], this->v[0]+v[0]);

		this->v[0] = v[0];
		this->v[1] = v[1];

		this->p += p;

		b[i][0] = p;
		b[i][1] = 0;
		this->t += this->Ts;

 	}
}

