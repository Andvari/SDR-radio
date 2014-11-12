/*
 * RTL.cpp
 *
 *  Created on: Oct 5, 2014
 *      Author: nemo
 */

#include "RTL.h"

RTL::RTL(int port, char *addr, int size, int dc) {

	this->dc = dc;
	this->block_size = size * 2;
	this->tmp = (unsigned char *)malloc(size * 2);

	this->peer.sin_family = AF_INET;
	this->peer.sin_port = htons(port);
	this->peer.sin_addr.s_addr = inet_addr(addr);
	this->sRTL = socket(AF_INET, SOCK_STREAM, 0);
	connect(this->sRTL, (struct sockaddr *) &this->peer, sizeof(this->peer));

}

RTL::~RTL() {
	free(this->tmp);
}

int RTL :: read(fftwf_complex *buf, int size){
	if(this->tmp == NULL) return -1;

	recv(this->sRTL, this->tmp, size * 2, 0);

	for(int i=0; i < size; i++){
		buf[i][0] = this->tmp[i*2+0] - this->dc;
		buf[i][1] = this->tmp[i*2+1] - this->dc;
	}

	return size;
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
