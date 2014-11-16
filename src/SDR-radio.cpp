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
#include "PLL.h"
#include "DEEMPHASIS.h"
#include "AUDIO.h"

using namespace std;

#define SLICE		25000

#define	I32	0
#define	U32	1
#define	SGL	2
#define	CSG	3

#define	LOW_PASS	0
#define	HIGH_PASS	1
#define	BANDPASS	2
#define	BANDSTOP	3
#define	PRE_EMPHASIS 4
#define	DE_EMPHASIS 5

#define	FS	(57000*20)

#define	TUNE			107800000
#define	PLL_FREQ		0

//#define FUPPER			58650
#define FUPPER			100000

#define INCLUDE_GRAPH
#define	INCLUDE_AUDIO

int main() {
	RTL *rtl = new RTL(1234, (char *)"192.168.1.2", SLICE, 127);
	OSC *osc = new OSC(2345, (char *)"192.168.1.102");
	FILTER *LPF1 = new FILTER(3*((int)(FS/(FS/8 - FUPPER))+1), LOW_PASS, FUPPER, 0, 1, 0);
	FILTER *LPF2 = new FILTER(512, LOW_PASS, 15000, 0, 5, 0);
	PLL *pll = new PLL(PLL_FREQ);
	DEEMPHASIS	*demph = new DEEMPHASIS(50e-6);
	BUFFER *buf = new BUFFER(SLICE, FS);

#ifdef	INCLUDE_AUDIO
	AUDIO *dev = new AUDIO((char *)"/home/nemo/audiopipe", 1);
#endif

	rtl->setFs(FS);
	rtl->setCorr(0);
	rtl->setTune(TUNE - PLL_FREQ);
	rtl->setAGC(0);
	rtl->setGain(1);
	rtl->setIndex(0);

	for(;;){
		rtl->read(buf);
		LPF1->make(buf);
		pll->make(buf);
		LPF2->make(buf);
		demph->make(buf);
#ifdef	INCLUDE_GRAPH
		osc->send(buf, CSG, 2);
#endif

#ifdef	INCLUDE_AUDIO
		dev->send(buf);
#endif

	}

	fftwf_free(buf);
	delete rtl;
	delete osc;
	return 0;
}
