#ifndef LEAKER_LOGGING_H_
#define LEAKER_LOGGING_H_

#include "Singleton.h"
#include <stdio.h>
class Logging: public Singleton<Logging>
{
public:
	Logging();
	~Logging();
private:
	FILE * fp;
};

#endif