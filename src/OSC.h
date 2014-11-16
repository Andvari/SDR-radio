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

#include "BUFFER.h"

#define	SIZE	(65536+12)

#define	I32	0
#define	U32	1
#define	SGL	2
#define	CSG	3

typedef struct{
	float re;
	float im;
}COMPLEX;

typedef struct{
	unsigned int framenumber;
	unsigned int frametype;
	unsigned int framesize;
}HEADER;

typedef struct{
	HEADER header;
	union{
		unsigned int   ui[SIZE];
		int 			i[SIZE];
		float			f[SIZE];
		//COMPLEX			c[SIZE/2];
	}body;
}FRAME;


class OSC {
public:
	OSC(int, char *);
	virtual ~OSC();

	void send(BUFFER *, int, int);

private:
	int sOSC;
	struct sockaddr_in server_addr;
	FRAME frame;
	int send_counter;
	int t;
};

#endif /* OSC_H_ */
