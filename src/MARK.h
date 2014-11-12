/*
 * MARK.h
 *
 *  Created on: Oct 11, 2014
 *      Author: nemo
 */

#ifndef MARK_H_
#define MARK_H_

#include <stdio.h>

class MARK {
public:
	MARK(int);
	virtual ~MARK();

	void init();
	void show(char *);
private:
	int mark;
};

#endif /* MARK_H_ */
