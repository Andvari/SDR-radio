/*
 * BUFFER.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: nemo
 */

#include "BUFFER.h"

BUFFER::BUFFER(int size, int fs) {
	this->b   = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * size);
	this->size = size;
	this->fs = fs;
}

BUFFER::~BUFFER() {
}

int BUFFER :: getSize(){
	return this->size;
}

void BUFFER :: setSize(int newsize){
	this->size = newsize;
}

fftwf_complex* BUFFER :: getB(){
	return this->b;
}

void BUFFER :: setFs(int newfs){
	this->fs = newfs;
}

int BUFFER :: getFs(){
	return this->fs;
}
