/*
 * AUDIO.h
 *
 *  Created on: Nov 14, 2014
 *      Author: nemo
 */

#include <fcntl.h>
#include <math.h>

#include "FILTER.h"
#include "BUFFER.h"

#include "fftw3.h"


#ifndef AUDIO_H_
#define AUDIO_H_

#define	AUDIO_BAND	48000
#define	FFT_SIZE	8192

class AUDIO {
public:
	AUDIO(char *, int);
	virtual ~AUDIO();

	void send(BUFFER *);
private:
	int	pipe;
	FILTER *upsampler;
	FILTER *downsampler;

	int debug;
	float left_agc;
	float right_agc;

	int	sourceband;
	int audioband;
};

#endif /* AUDIO_H_ */
