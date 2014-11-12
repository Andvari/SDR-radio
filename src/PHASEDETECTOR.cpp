/*
 * PHASEDETECTOR.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: nemo
 */

#include "PHASEDETECTOR.h"


PHASE_DETECTOR::PHASE_DETECTOR(int size) {
	this->t[0] = 0;
	this->t[1] = 0;
	this->dp_t = 0;
	//this->dps_t = 0;
	this->size = size;

	dp      = new float[this->size];
	dps     = new float[this->size];
	dp_corr = new float[this->size];
	cumsum  = new float[this->size];

	this->phase = 0;
	this->mean = 0;
}

PHASE_DETECTOR::~PHASE_DETECTOR() {
	delete dp;
	delete dps;
	delete dp_corr;
	delete cumsum;
}

void PHASE_DETECTOR :: make(fftwf_complex *b){
}
