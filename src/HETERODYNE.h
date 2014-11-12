/*
 * HETERODYNE.h
 *
 *  Created on: Nov 8, 2014
 *      Author: nemo
 */

#ifndef HETERODYNE_H_
#define HETERODYNE_H_
#include <math.h>
#include "fftw3.h"

#define	PI	3.14159265358979323846

class HETERODYNE {
public:
	HETERODYNE(int, int);
	virtual ~HETERODYNE();

	void make(fftwf_complex *, int);
private:
	int Fh;
	int Fs;

	double ph;
	double Ts;
	double t;
};

#endif /* HETERODYNE_H_ */
