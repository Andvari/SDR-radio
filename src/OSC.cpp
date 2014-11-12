/*
 * OSC.cpp
 *
 *  Created on: Oct 5, 2014
 *      Author: nemo
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>

#include "OSC.h"


OSC::OSC(int port, char *addr) {

	   memset(&this->server_addr, 0, sizeof(this->server_addr));
	   this->server_addr.sin_family = AF_INET;
	   this->server_addr.sin_addr.s_addr=inet_addr(addr);
	   this->server_addr.sin_port=htons(port);
	   this->sOSC = socket(AF_INET, SOCK_STREAM, 0);

	   this->send_counter = 0;
	   this->frame_initialized = 0;
	   this->frame.header.framenumber = 0;
	   this->frame.header.framesize = 0;
	   this->frame.header.frametype = 0;

}

OSC::~OSC() {
}

void OSC :: send(){
	char res[3];

	int *a;
	int err;

	a = (int *)&this->frame;

	if(this->isFrameUninitialized()) return;

	this->frame.header.framenumber = this->send_counter++;

	this->sOSC = socket(AF_INET, SOCK_STREAM, 0);
	connect(this->sOSC, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr));
	sendto(this->sOSC, &this->frame, sizeof(this->frame.header) + this->frame.header.framesize, 0, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr));
	recv(this->sOSC, res, 2, MSG_WAITALL);
	close(this->sOSC);

	this->setFrameUninitialized();

	this->send_counter++;
}

void OSC :: setFrame(float *data, int size, int type, int numgraphs){
	int i;

	this->frame.header.frametype = type*256*256*256 + numgraphs*256*256;
	this->frame.header.framesize = size;

	for(i=0; i<size/4; i++){
		this->frame.body.f[i] = data[i];
	}

	this->setFrameInitialized();
}

void OSC :: setFrame(int *data, int size, int type, int numgraphs){

	this->frame.header.frametype = type*256*256*256 + numgraphs*256*256;
	this->frame.header.framesize = size;

	printf("2 %d\n", size);
	memcpy(&this->frame.body, data, size);

	this->setFrameInitialized();
}

void OSC :: setFrame(unsigned int *data, int size, int type, int numgraphs){

	this->frame.header.frametype = type*256*256*256 + numgraphs*256*256;
	this->frame.header.framesize = size;

	printf("3 %d\n", size);
	memcpy(&this->frame.body, data, size);

	this->setFrameInitialized();
}


void OSC :: setFrameInitialized(){
	this->frame_initialized = 1;
}

void OSC :: setFrameUninitialized(){
	this->frame_initialized = 0;
}

int OSC :: isFrameUninitialized(){
	return (1 - this->frame_initialized);
}
