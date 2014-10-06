/*
 * RTL.cpp
 *
 *  Created on: Oct 5, 2014
 *      Author: nemo
 */

#include "RTL.h"

RTL::RTL(int port, char *addr, int size, int size_t, int dc) {

	this->dc = dc;
	this->block_size = size * size_t;
	this->tmp = (unsigned char *)malloc(size * size_t);

	this->peer.sin_family = AF_INET;
	this->peer.sin_port = htons(port);
	this->peer.sin_addr.s_addr = inet_addr(addr);
	this->sRTL = socket(AF_INET, SOCK_STREAM, 0);
	connect(this->sRTL, (struct sockaddr *) &this->peer, sizeof(this->peer));
}

RTL::~RTL() {
	free(this->tmp);
}

int RTL :: read(float *buf, int size, int size_t){

	if(this->tmp == NULL) return -1;

	recv(this->sRTL, this->tmp, size * size_t, 0);

	for(int i=0; i < size * size_t; i++){
		buf[i] = this->tmp[i] - this->dc;
	}

	return size;
}

void RTL :: setDC(int dc){
	this->dc = dc;
}
