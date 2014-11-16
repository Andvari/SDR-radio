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
	   this->frame.header.framenumber = 0;
	   this->frame.header.framesize = 0;
	   this->frame.header.frametype = 0;

	   this->t=0;
}

OSC::~OSC() {
}

void OSC :: send(BUFFER *b, int type, int numgraphs){
	char res[3];
	int i;
	int size;
	float *data;

	if(this->t > 0){
		t--;
		return;
	}
	t = 3;

	data = (float *)b->getB();
	size = b->getSize();

	this->frame.header.framenumber = this->send_counter++;
	this->frame.header.frametype = type*256*256*256 + numgraphs*256*256;
	this->frame.header.framesize = size*8;

	for(i=0; i<size*2; i++){
		this->frame.body.f[i] = data[i];
	}

	this->sOSC = socket(AF_INET, SOCK_STREAM, 0);
	connect(this->sOSC, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr));
	sendto(this->sOSC, &this->frame, sizeof(this->frame.header) + this->frame.header.framesize, 0, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr));
	recv(this->sOSC, res, 2, MSG_WAITALL);
	close(this->sOSC);

	this->send_counter++;
}

