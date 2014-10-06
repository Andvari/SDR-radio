/*
 * OSC.cpp
 *
 *  Created on: Oct 5, 2014
 *      Author: nemo
 */

#include "OSC.h"
#include "unistd.h"

OSC::OSC(int port, char *addr) {

	this->peer.sin_family = AF_INET;
	this->peer.sin_port = htons(port);
	this->peer.sin_addr.s_addr = inet_addr(addr);
	this->sOSC = socket(AF_INET, SOCK_DGRAM, 0);
}

OSC::~OSC() {
}

void OSC :: send(void *buf, int size, int size_t, int type){
	int bytes_to_send;
	int i;

	char *src;

	src = (char *)buf;

	bytes_to_send = size * size_t;

	this->pack.full_length = bytes_to_send;
	this->pack.packet_length = BODY_SIZE;
	this->pack.type = type;
	this->pack.num_packets = 0;
	this->pack.unit_size = size_t;

	i=0;
	while(bytes_to_send >= BODY_SIZE){
		this->pack.num_packet = i;

		memset(this->pack.body, 0, sizeof(this->pack.body));
		memcpy(&this->pack.body, &src[i*BODY_SIZE], BODY_SIZE);
		sendto(sOSC, &this->pack, sizeof(this->pack), 0, (struct sockaddr *)&this->peer, sizeof this->peer);

		bytes_to_send -= BODY_SIZE;
		i++;
	}

	if(bytes_to_send != 0){
		this->pack.num_packet = i;

		memcpy(&this->pack.body, &src[i*BODY_SIZE], bytes_to_send);
		sendto(sOSC, &this->pack, sizeof(this->pack), 0, (struct sockaddr *)&this->peer, sizeof this->peer);

	}

}
