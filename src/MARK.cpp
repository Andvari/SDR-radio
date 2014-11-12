/*
 * MARK.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: nemo
 */

#include "MARK.h"

MARK::MARK(int mark) {
	this->mark = 0;
}

MARK::~MARK() {
}

void MARK :: init(){
	this->mark = 0;
}
void MARK :: show(char *name){
	printf("%s %d\n", name, this->mark++);
}
