//============================================================================
// Name        : SDR-radio.cpp
// Author      : Nemo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fftw3.h>

#include "RTL.h"
#include "OSC.h"

using namespace std;

#define N	8

int main() {
	RTL *rtl;
	OSC *osc;

	float in[2000];

	rtl = new RTL(1234, (char *)"192.168.1.2", 1000, 2, 127);
	osc = new OSC(2345, (char *)"192.168.1.102");


	for(;;){
		rtl->read(in, 1000, 2);
		osc->send(in, 1000, 8, 1);

	}

	delete rtl;
	delete osc;
	return 0;
}

/*
 	fftw_complex *in, *out;
	fftw_plan p;

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);

	fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);
 */
