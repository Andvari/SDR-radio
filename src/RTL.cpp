/*
 * RTL.cpp
 *
 *  Created on: Oct 5, 2014
 *      Author: nemo
 */

#include "RTL.h"

RTL::RTL(int port, char *addr, int size, int dc) {

	this->dc = dc;
	this->size = size;
	this->fs = 0;
	this->agc = 0;
	this->corr = 0;
	this->gain = 0;
	this->index = 0;
	this->tune = 0;

	this->tmp = (unsigned char *)malloc(this->size*2);

	this->peer.sin_family = AF_INET;
	this->peer.sin_port = htons(port);
	this->peer.sin_addr.s_addr = inet_addr(addr);
	this->sRTL = socket(AF_INET, SOCK_STREAM, 0);
	connect(this->sRTL, (struct sockaddr *) &this->peer, sizeof(this->peer));

}

RTL::~RTL() {
	free(this->tmp);
}

void RTL :: read(BUFFER *b){
	fftwf_complex *buf;

	b->setFs(this->fs);
	b->setSize(this->size);

	buf = b->getB();

	if(this->tmp == NULL){
		printf("read(): tmp buffer not ready");
		return;
	}

	recv(this->sRTL, this->tmp, this->size*2, 0);

	if(b->getSize() < this->size){
		printf("read(): No enough space in BUFFER\n");
		return;
	}
	for(int i=0; i < this->size; i++){
		buf[i][0] = this->tmp[i*2+0] - this->dc;
		buf[i][1] = this->tmp[i*2+1] - this->dc;
	}

	return;
}

void RTL :: setDC(int dc){
	this->dc = dc;
}

void RTL :: cmd(char cmd, int value){

	char v;

	sendto(this->sRTL, &cmd,   1, 0, (struct sockaddr *)&this->peer, sizeof(this->peer));

	v = (value >> 24)&0xFF;
	sendto(this->sRTL, &v, 1, 0, (struct sockaddr *)&this->peer, sizeof(this->peer));
	v = (value >> 16)&0xFF;
	sendto(this->sRTL, &v, 1, 0, (struct sockaddr *)&this->peer, sizeof(this->peer));
	v = (value >> 8)&0xFF;
	sendto(this->sRTL, &v, 1, 0, (struct sockaddr *)&this->peer, sizeof(this->peer));
	v = (value >> 0)&0xFF;
	sendto(this->sRTL, &v, 1, 0, (struct sockaddr *)&this->peer, sizeof(this->peer));
}

void RTL :: setFs(int fs){
	this->fs = fs;
	this->cmd(FS_CMD, fs);
}

void RTL :: setCorr(int corr){
	this->corr = corr;
	this->cmd(CORR, corr);
}

void RTL :: setTune(int tune){
	this->tune = tune;
	this->cmd(FREQ, tune);
}

void RTL :: setAGC(int agc){
	this->agc = agc;
	this->cmd(AGC, agc);
}

void RTL :: setGain(int gain){
	this->gain = gain;
	this->cmd(GAIN,	gain);
}

void RTL :: setIndex(int index){
	this->index = index;
	this->cmd(INDEX, index);
}

