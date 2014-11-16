/*
 * FILTER.cpp
 *
 *  Created on: Oct 7, 2014
 *      Author: nemo
 */

#include "FILTER.h"
#include <math.h>
#include <string.h>
#include "fftw3.h"
#include "stdio.h"

#define	PI	3.14159265356

FILTER::FILTER(int IR_lenght, int type, int low, int high, int dec, int debug) {
	this->IR_lenght = IR_lenght;
	this->FR_lenght = -1;
	this->Fs = -1;
	this->Flow = low;
	this->Fhigh = high;
	this->dec = dec;
	this->type = type;
	this->debug = debug;
}


FILTER::~FILTER() {
	fftwf_free(this->FR);
	fftwf_free(this->mem);
	fftwf_free(this->p);
}

void FILTER :: getFR(BUFFER *b){

	if(b->getSize() < this->FR_lenght){
		printf("getFr(): No enough space in BUFFER\n");
		return;
	}

	for(int i=0; i<this->FR_lenght; i++){
		b->getB()[i][0] = sqrt(this->FR[i][0]*this->FR[i][0] + this->FR[i][1]*this->FR[i][1]);
		b->getB()[i][1] = atan2(this->FR[i][1], this->FR[i][0]);
	}
}

void FILTER :: make(BUFFER *buf){
	fftwf_complex *b;
	fftwf_plan p;

	int newfr;

	if(this->dec > 0){
		newfr= (int)pow(2, 1+(int)log2(this->IR_lenght+buf->getSize()));
	}
	else{
		newfr= (int)pow(2, 1+(int)log2(-this->dec*(this->IR_lenght+buf->getSize())));
	}

	int size;
	double a[2];
	double norm;

	if(buf->getFs()   != this->Fs) this->updateFR(buf);
	if(newfr != this->FR_lenght) this->updateFR(buf);

	b = buf->getB();
	size = buf->getSize();

	memcpy(this->p, this->mem, this->IR_lenght*sizeof(fftwf_complex));
	memcpy(&this->p[this->IR_lenght][0], b, size*sizeof(fftwf_complex));
	memcpy(this->mem, &b[size-this->IR_lenght][0], this->IR_lenght*sizeof(fftwf_complex));
	memset(&this->p[size + this->IR_lenght][0], 0, (this->FR_lenght-size-this->IR_lenght)*sizeof(fftwf_complex));

	norm = 1. / sqrt(this->FR_lenght*this->IR_lenght);

	if(this->dec < 0){
		for(int i=this->IR_lenght + buf->getSize()-1; i>=0; i--){
			this->p[i*-this->dec + 0][0] = this->p[i][0];
			this->p[i*-this->dec + 0][1] = this->p[i][1];
			this->p[i*-this->dec + 1][0] = 0 ;
			this->p[i*-this->dec + 1][1] = 0 ;
			this->p[i*-this->dec + 2][0] = 0 ;
			this->p[i*-this->dec + 2][1] = 0 ;
			this->p[i*-this->dec + 3][0] = 0 ;
			this->p[i*-this->dec + 3][1] = 0 ;
		}

		buf->setFs(buf->getFs()*-this->dec);
		buf->setSize(buf->getSize()*-this->dec);
	}

	p = fftwf_plan_dft_1d(this->FR_lenght, this->p, this->p, FFTW_FORWARD, FFTW_ESTIMATE);
	fftwf_execute(p);

	for(int i=0; i<this->FR_lenght; i++){
		a[0] = this->p[i][0]*this->FR[i][0] - this->p[i][1]*this->FR[i][1];
		a[1] = this->p[i][0]*this->FR[i][1] + this->p[i][1]*this->FR[i][0];
		this->p[i][0] = a[0] * norm;
		this->p[i][1] = a[1] * norm;
	}

	p = fftwf_plan_dft_1d(this->FR_lenght, this->p, this->p, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftwf_execute(p);

	if(this->dec > 0){
		buf->setFs(buf->getFs()/this->dec);
		buf->setSize(buf->getSize()/this->dec);

		for(int i=0, j=this->IR_lenght; i<buf->getSize(); i++, j+=this->dec){
			b[i][0] = this->p[j][0] * norm ;
			b[i][1] = this->p[j][1] * norm ;
		}
	}
	else{
		for(int i=0, j=this->IR_lenght; i<buf->getSize(); i++, j++){
			b[i][0] = this->p[j][0] * norm ;
			b[i][1] = this->p[j][1] * norm ;
		}
	}

	fftwf_destroy_plan(p);

	if(this->debug == 1){
		FILE *f;

		f = fopen("debug.txt", "w");
		for(int i=0; i<buf->getSize(); i++){
			fprintf(f, "%d %f %f \n", i, buf->getB()[i][0], buf->getB()[i][1]);
		}
		fclose(f);
	}
}

void FILTER :: updateFR(BUFFER *b){
	fftwf_plan p;

	this->Fs = b->getFs();
	if(this->dec > 0){
		this->FR_lenght = (int)pow(2, 1+(int)log2(this->IR_lenght+b->getSize()));
	}
	else{
		this->FR_lenght = (int)pow(2, 1+(int)log2(-this->dec*(this->IR_lenght+b->getSize())));
	}

	fftwf_free(this->FR);
	fftwf_free(this->mem);
	fftwf_free(this->p);

	this->FR = (fftwf_complex *)  fftwf_malloc(sizeof(fftwf_complex) * this->FR_lenght);
	this->mem = (fftwf_complex *) fftwf_malloc(sizeof(fftwf_complex) * this->IR_lenght);
	this->p   = (fftwf_complex *) fftwf_malloc(sizeof(fftwf_complex) * this->FR_lenght);

	memset(this->FR,	0,	this->FR_lenght*sizeof(fftwf_complex));
	memset(this->mem,	0,	this->IR_lenght*sizeof(fftwf_complex));
	memset(this->p,		0,	this->FR_lenght*sizeof(fftwf_complex));

	if(this->debug == 1) printf("%d\n", this->FR_lenght);

	switch(this->type){
			case LOW_PASS:
				for(int i=0; i<this->IR_lenght; i++){
					if(((i*this->Fs / this->IR_lenght) > this->Flow)&&((i*this->Fs / this->IR_lenght )< (this->Fs - this->Flow))){
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
					if(((i*this->Fs / this->IR_lenght) > this->Fhigh/2)&&((i*this->Fs/this->IR_lenght )< (this->Fs-this->Fhigh/2))){
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

	if(type == BANDPASS){
		double ac, as;
		for(int i=0; i<this->FR_lenght; i++){
			ac = this->FR[i][0]*cos(2.*PI*this->Flow*i/this->Fs) - this->FR[i][1]*sin(2.*PI*sin(2.*PI*this->Flow*i/this->Fs));
			as = this->FR[i][0]*sin(2.*PI*this->Flow*i/this->Fs) + this->FR[i][1]*cos(2.*PI*sin(2.*PI*this->Flow*i/this->Fs));
			this->FR[i][0] = ac;
			this->FR[i][1] = as;
		}
	}

	p = fftwf_plan_dft_1d(FR_lenght, this->FR, this->FR, FFTW_FORWARD, FFTW_ESTIMATE);
	fftwf_execute(p);
	fftwf_destroy_plan(p);

}
