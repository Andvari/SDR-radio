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
#include "BUFFER.h"


class PLL {
public:
	PLL(int);
	virtual ~PLL();

	void make(BUFFER *);
private:
	double t;
	double Fget;
	double p;
};

#endif /* PLL_H_ */
