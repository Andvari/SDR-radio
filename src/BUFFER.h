/*
 * BUFFER.h
 *
 *  Created on: Nov 14, 2014
 *      Author: nemo
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "fftw3.h"

class BUFFER {
public:
	BUFFER(int, int);
	virtual ~BUFFER();

	int getSize();
	void setSize(int);
	fftwf_complex* getB();
	void setFs(int);
	int getFs();

private:
	fftwf_complex	*b;
	int size;
	int fs;
};

#endif /* BUFFER_H_ */
