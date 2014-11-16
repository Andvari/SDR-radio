/*
 * FILTER.h
 *
 *  Created on: Oct 7, 2014
 *      Author: nemo
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "fftw3.h"
#include "BUFFER.h"

#define	LOW_PASS	0
#define	HIGH_PASS	1
#define	BANDPASS	2
#define	BANDSTOP	3

class FILTER {
public:
	FILTER(int, int, int, int, int, int);
	virtual ~FILTER();

	void getFR(BUFFER *);
	void make(BUFFER *);

	float *getBody();
	void setBody(float *, int);

	void updateFR(BUFFER *);
private:
	int	IR_lenght;
	int FR_lenght;
	fftwf_complex *FR;
	int	Fs;
	int Flow;
	int Fhigh;
	int dec;
	int type;
	fftwf_complex *mem;
	fftwf_complex *p;
	int debug;
};

#endif /* FILTER_H_ */
