/*
 * FILTER.cpp
 *
 *  Created on: Oct 7, 2014
 *      Author: nemo
 */

#include "FILTER.h"
#include "MARK.h"
#include <math.h>
#include <string.h>
#include "fftw3.h"

#define	PI	3.14159265356

FILTER::FILTER(int IR_lenght, int FR_lenght, int fsample, int type, int low, int high, int dec) {
	fftwf_plan p;

	this->IR_lenght = IR_lenght;
	this->FR_lenght = FR_lenght;
	this->fsample = fsample;
	this->flow = low;
	this->dec = dec;
	this->FR = (fftwf_complex *)  fftwf_malloc(sizeof(fftwf_complex) * FR_lenght);
	this->mem = (fftwf_complex *) fftwf_malloc(sizeof(fftwf_complex) * IR_lenght);
	this->p   = (fftwf_complex *) fftwf_malloc(sizeof(fftwf_complex) * FR_lenght);

	switch(type){
			case LOW_PASS:
				for(int i=0; i<IR_lenght; i++){
					if(((i*fsample / IR_lenght) > low)&&((i*fsample / IR_lenght )< (fsample - low))){
						this->FR[i][0] = 0;
						this->FR[i][1] = 0;
					}
					else{
						if(i<(this->IR_lenght-1)/2){
							this->FR[i][0] = 1000. * cos(-PI*i*(this->IR_lenght-1)/this->IR_lenght);
							this->FR[i][1] = 1000. * sin(-PI*i*(this->IR_lenght-1)/this->IR_lenght);
						}
						else{
							this->FR[i][0] = 1000. * cos(-PI*(i-this->IR_lenght)*(this->IR_lenght-1)/this->IR_lenght);
							this->FR[i][1] = 1000. * sin(-PI*(i-this->IR_lenght)*(this->IR_lenght-1)/this->IR_lenght);
						}
					}
				}
				break;
			case BANDPASS:
				for(int i=0; i<IR_lenght; i++){
					if(((i*fsample / IR_lenght) < low)||((i*fsample / IR_lenght ) > high)){
						this->FR[i][0] = 0;
						this->FR[i][1] = 0;
					}
					else{
						if(i<(this->IR_lenght-1)/2){
							this->FR[i][0] = 1000. * cos(-PI*i*(this->IR_lenght-1)/this->IR_lenght);
							this->FR[i][1] = 1000. * sin(-PI*i*(this->IR_lenght-1)/this->IR_lenght);
						}
						else{
							this->FR[i][0] = 1000. * cos(-PI*(i-this->IR_lenght)*(this->IR_lenght-1)/this->IR_lenght);
							this->FR[i][1] = 1000. * sin(-PI*(i-this->IR_lenght)*(this->IR_lenght-1)/this->IR_lenght);
						}
					}
				}
				break;
	}

	p = fftwf_plan_dft_1d(IR_lenght, this->FR, this->FR, FFTW_FORWARD, FFTW_ESTIMATE);
	fftwf_execute(p);

	for(int i=0; i<this->IR_lenght; i++){
		this->FR[i][0] = this->FR[i][0] * (0.54 + 0.46*cos(2.*PI*(i-IR_lenght/2)/(IR_lenght-1)));
		this->FR[i][1] = 0;
	}

	for(int i=IR_lenght; i<this->FR_lenght; i++){
		this->FR[i][0] = 0;
		this->FR[i][1] = 0;
	}

	p = fftwf_plan_dft_1d(FR_lenght, this->FR, this->FR, FFTW_FORWARD, FFTW_ESTIMATE);
	fftwf_execute(p);
	fftwf_destroy_plan(p);
}


FILTER::~FILTER() {
	fftwf_free(this->FR);
	fftwf_free(this->mem);
	fftwf_free(this->p);
}

void FILTER :: getFR(fftwf_complex *fr){
	for(int i=0; i<this->FR_lenght; i++){
		fr[i][0] = sqrt(this->FR[i][0]*this->FR[i][0] + this->FR[i][1]*this->FR[i][1]);
		fr[i][1] = atan2(this->FR[i][1], this->FR[i][0]);
	}
}

void FILTER :: make(fftwf_complex *b, int size){
	fftwf_plan p;
	double a[2];

	double norm;


	memcpy(this->p, this->mem, this->IR_lenght*sizeof(fftwf_complex));
	memcpy(&this->p[this->IR_lenght][0], b, size*sizeof(fftwf_complex));
	memcpy(this->mem, &b[size-this->IR_lenght][0], this->IR_lenght*sizeof(fftwf_complex));
	memset(&this->p[size + this->IR_lenght][0], 0, (this->FR_lenght-size-this->IR_lenght)*sizeof(fftwf_complex));

/*
	for(int i=0; i<this->IR_lenght; i++){
		this->p[i][0] = this->mem[i][0];
		this->p[i][1] = this->mem[i][1];
		this->mem[i][0] = b[size-this->IR_lenght+i][0];
		this->mem[i][1] = b[size-this->IR_lenght+i][1];
	}

	for(int i=0; i<size; i++){
		this->p[this->IR_lenght + i][0] = b[i][0];
		this->p[this->IR_lenght + i][1] = b[i][1];
	}
*/
/*
	for(int i=size+this->IR_lenght; i<this->FR_lenght; i++){
		this->p[i][0] = 0;
		this->p[i][1] = 0;
	}
*/
	norm = 1. / sqrt(this->FR_lenght*this->IR_lenght);

	p = fftwf_plan_dft_1d(this->FR_lenght, this->p, this->p, FFTW_FORWARD, FFTW_ESTIMATE);
	fftwf_execute(p);

	for(int i=0; i<this->FR_lenght; i++){
		a[0] = this->p[i][0]*this->FR[i][0] - this->p[i][1]*this->FR[i][1];
		a[1] = this->p[i][0]*this->FR[i][1] + this->p[i][1]*this->FR[i][0];
		this->p[i][0] = a[0] * norm ;
		this->p[i][1] = a[1] * norm ;
	}

	p = fftwf_plan_dft_1d(this->FR_lenght, this->p, this->p, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftwf_execute(p);

	for(int i=0, j=this->IR_lenght; i<size; i++, j+=dec){
		b[i][0] = this->p[j][0] * norm ;
		b[i][1] = this->p[j][1] * norm ;
	}
	fftwf_destroy_plan(p);

}
