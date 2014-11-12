/*
 * PLL.h
 *
 *  Created on: Nov 6, 2014
 *      Author: nemo
 */

#ifndef PLL_H_
#define PLL_H_

#include <math.h>
#include "fftw3.h"
#define	PI	3.14159265356

class PLL {
public:
	PLL(int, int, int, float, float, float);
	virtual ~PLL();

	void make(fftwf_complex *, int);
private:
	float  kd;
	float  ko;
	float  wp;
	double zeta;

	double  Fs;
	double  Ts;

	float  ki;
	float  kp;

	double t;
	double Fget;
	double p;
	double v[2];
	double e[2];
};

#endif /* PLL_H_ */
