/*
 * PHASEDETECTOR.h
 *
 *  Created on: Oct 12, 2014
 *      Author: nemo
 */

#ifndef PHASEDETECTOR_H_
#define PHASEDETECTOR_H_

#include <math.h>

#include "fftw3.h"

#define	PI	3.14159265356

class PHASE_DETECTOR {
public:
	PHASE_DETECTOR(int);
	virtual ~PHASE_DETECTOR();

	void make(fftwf_complex *);
private:
	int size;
	float t[2];

	float *dp;
	float *dps;
	float *dp_corr;
	float *cumsum;
	float dp_t;
	//float dps_t;


	float phase;
	float mean;
};

#endif /* PHASEDETECTOR_H_ */
