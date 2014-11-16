/*
 * RTL.h
 *
 *  Created on: Oct 5, 2014
 *      Author: nemo
 */

#ifndef RTL_H_
#define RTL_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <malloc.h>

#include "fftw3.h"
#include "BUFFER.h"

/*
 * rtl commands
 */
#define FREQ	(char)(0x01)
#define	FS_CMD	(char)(0x02)
#define	GAIN	(char)(0X03)
#define	CORR	(char)(0x04)
#define	AGC		(char)(0x08)
#define	INDEX	(char)(0x0d)


class RTL {
public:
	RTL(int, char *, int, int);
	virtual ~RTL();

	void read(BUFFER *);
	void setDC(int);
	void cmd(char, int);

	void setFs(int);
	void setCorr(int);
	void setTune(int);
	void setAGC(int);
	void setGain(int);
	void setIndex(int);

private:
	int sRTL;
	struct sockaddr_in peer;
	int dc;
	int size;

	int fs;
	int corr;
	int tune;
	int agc;
	int gain;
	int index;

	unsigned char *tmp;
};

#endif /* RTL_H_ */
