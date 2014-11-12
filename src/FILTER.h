/*
 * FILTER.h
 *
 *  Created on: Oct 7, 2014
 *      Author: nemo
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "fftw3.h"

#define	LOW_PASS	0
#define	HIGH_PASS	1
#define	BANDPASS	2
#define	BANDSTOP	3

class FILTER {
public:
	FILTER(int, int, int, int, int, int, int);
	virtual ~FILTER();

	void getFR(fftwf_complex *);


	void setBody(float *, int);

	void make(fftwf_complex *, int);

	float *getBody();
private:
	int	IR_lenght;
	int FR_lenght;
	fftwf_complex *FR;
	int	fsample;
	int flow;
	int dec;
	fftwf_complex *mem;
	fftwf_complex *p;
};

#endif /* FILTER_H_ */
