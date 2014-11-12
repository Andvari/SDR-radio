//============================================================================
// Name        : SDR-radio.cpp
// Author      : Nemo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fftw3.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include "RTL.h"
#include "OSC.h"
#include "FILTER.h"
#include "MARK.h"
//#include "PHASEDETECTOR.h"
#include "HETERODYNE.h"
#include "PLL.h"
#include "DEEMPHASIS.h"


using namespace std;

#define N	8

#define FS		1008000

#define	FFT_SIZE	8192
#define SLICE		7000

#define	I32	0
#define	U32	1
#define	SGL	2
#define	CSG	3

#define FREQ	(char)(0x01)
#define	FS_CMD	(char)(0x02)
#define	GAIN	(char)(0X03)
#define	CORR	(char)(0x04)
#define	AGC		(char)(0x08)
#define	INDEX	(char)(0x0d)

#define	LOW_PASS	0
#define	HIGH_PASS	1
#define	BANDPASS	2
#define	BANDSTOP	3
#define	PRE_EMPHASIS 4
#define	DE_EMPHASIS 5

//#define	TUNE			107600000
#define	TUNE			100900000
#define	HETERODYNE_FREQ	-(200000)
#define	PLL_FREQ		0

#define INCLUDE_GRAPH
#define	INCLUDE_AUDIO

int main() {
	int pipe;
	RTL *rtl;
	OSC *osc;
	HETERODYNE *HET;
	FILTER *LPF200;
	FILTER *LPF15;
	PLL *pll;
	DEEMPHASIS	*demph;
	fftwf_complex *buf;


	int t;

	float ab[SLICE];

	HET			=	new HETERODYNE(FS, HETERODYNE_FREQ);
	LPF200		=	new FILTER(128,	FFT_SIZE, FS, LOW_PASS, 200000, 0, 1);
	LPF15		=	new FILTER(512, FFT_SIZE, FS, LOW_PASS, 15000, 0, 1);
	demph		=	new DEEMPHASIS((int)FS, (double)50e-6);

	buf   = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * SLICE);

	rtl = new RTL(1234, (char *)"192.168.1.2", SLICE, 127);
	rtl->cmd(FS_CMD,	FS);
	rtl->cmd(CORR,		0);
	rtl->cmd(FREQ,		TUNE);
	rtl->cmd(AGC, 		0);
	rtl->cmd(GAIN,		1);
	rtl->cmd(INDEX, 	0);

	osc = new OSC(2345, (char *)"192.168.1.102");
	pll = new PLL(FS, PLL_FREQ, 50, 1., 1., .5);

	//PH_D = new PHASE_DETECTOR(SLICE);

	//LPF200->getFR(buf);
	//osc->setFrame((float *)buf, sizeof(fftwf_complex) * SLICE, CSG, 2);
	//osc->send();
	//for(;;);


#ifdef	INCLUDE_AUDIO
	pipe = open("/home/nemo/audiopipe", O_WRONLY);
	printf("pipe: %d\n", pipe);
#endif

	t = 3;
	double vps = 0;
	for(;;){
		rtl->read(buf, SLICE);
		//HET->make(buf, SLICE);
		LPF200->make(buf, SLICE);
		pll->make(buf,SLICE);
		LPF15->make(buf, SLICE);
		demph->make(buf, SLICE);
#ifdef	INCLUDE_GRAPH
		if(t==0){
			osc->setFrame((float *)buf, (sizeof(fftwf_complex) * SLICE), CSG, 2);
			osc->send();
			t=3;
		}
		else{
			t--;
		}
#endif

		for(int i=0, j=0; i<SLICE/21; i++, j+=21){
			ab[i] = buf[j][0] /1000.;
		}

#ifdef	INCLUDE_AUDIO
		write(pipe, ab, (SLICE / 21) *sizeof(float) );
#endif

	}

	fftwf_free(buf);
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

/*
	y[i] := α * x[i] + (1-α) * y[i-1]
*/
