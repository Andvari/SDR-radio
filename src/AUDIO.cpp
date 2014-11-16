/*
 * AUDIO.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: nemo
 */

#include "AUDIO.h"
#include <unistd.h>

#define	SOURCEBAND_DEFAULT	228000

AUDIO::AUDIO(char *name, int debug) {
	this->pipe = open(name, O_WRONLY);
	this->debug = debug;
	this->left_agc = 0;
	this->right_agc = 0;
	this->audioband = AUDIO_BAND;
	this->sourceband = SOURCEBAND_DEFAULT;

	this->upsampler   = new FILTER(1024, LOW_PASS, 4*AUDIO_BAND/2, 0, -4, 0);
	this->downsampler = new FILTER(1024, LOW_PASS, AUDIO_BAND/2, 0, 19, 0);

}

AUDIO::~AUDIO() {
}

void AUDIO :: send(BUFFER *buf){

	if((buf->getFs()==228000)&&(AUDIO_BAND==48000)){
		this->upsampler->make(buf);
		this->downsampler->make(buf);

		for(int i=0; i<buf->getSize();i++){
			if(buf->getB()[i][0] > this->left_agc)  this->left_agc  = buf->getB()[i][0];
			if(buf->getB()[i][1] > this->right_agc) this->right_agc = buf->getB()[i][1];
		}

		for(int i=0; i<buf->getSize();i++){
			buf->getB()[i][0] /= this->left_agc;
			buf->getB()[i][1] /= this->right_agc;
		}

		write(this->pipe, (float *)buf->getB(), buf->getSize() *sizeof(float) * 2);
	}
	else{
		printf("send(): resampling parameters not defined");
	}
}
