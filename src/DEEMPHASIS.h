/*
 * DEEMPHASIS.h
 *
 *  Created on: Nov 10, 2014
 *      Author: nemo
 */

#ifndef DEEMPHASIS_H_
#define DEEMPHASIS_H_

#include "BUFFER.h"
#include "fftw3.h"

class DEEMPHASIS {
public:
	DEEMPHASIS(int, double);
	DEEMPHASIS(double);
	virtual ~DEEMPHASIS();

	void make(BUFFER *);

private:
	double	tau;

	fftwf_complex y;
};

#endif /* DEEMPHASIS_H_ */
