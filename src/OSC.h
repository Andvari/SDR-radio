/*
 * OSC.h
 *
 *  Created on: Oct 5, 2014
 *      Author: nemo
 */

#ifndef OSC_H_
#define OSC_H_

#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "string.h"

#define	BODY_SIZE (360*sizeof(float))

class OSC {
public:
	OSC(int, char *);
	virtual ~OSC();

	void send(void *, int, int, int);

private:
	int sOSC;
	struct sockaddr_in peer;

	struct {
		int	full_length;
		int	num_packets;
		int num_packet;
		int packet_length;
		int type;
		int unit_size;
		int	reserv[2];
		unsigned char body[BODY_SIZE];
	}pack;

};

#endif /* OSC_H_ */
