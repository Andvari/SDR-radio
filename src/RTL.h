/*
 * RTL.h
 *
 *  Created on: Oct 5, 2014
 *      Author: nemo
 */

#ifndef RTL_H_
#define RTL_H_

#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"

#include "malloc.h"

class RTL {
public:
	RTL(int, char *, int, int, int);
	virtual ~RTL();
	int read(float *, int, int);

	void setDC(int);

private:
	int sRTL;
	struct sockaddr_in peer;
	int dc;
	int block_size;
	unsigned char *tmp;
};

#endif /* RTL_H_ */
