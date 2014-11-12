/*
 * DEEMPHASIS.h
 *
 *  Created on: Nov 10, 2014
 *      Author: nemo
 */

#ifndef DEEMPHASIS_H_
#define DEEMPHASIS_H_

#include "fftw3.h"

class DEEMPHASIS {
public:
	DEEMPHASIS(int, int, double);
	DEEMPHASIS(int, double);
	virtual ~DEEMPHASIS();

	void make(fftwf_complex *, int);

private:
	double	Ts;
	double	tau;

	fftwf_complex y;
};

#endif /* DEEMPHASIS_H_ */
