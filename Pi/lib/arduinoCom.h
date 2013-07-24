#ifndef _ARDUINOCOM_H
#define _ARDUINOCOM_H

#include "serial.h"
#include "logging.h"
#include "gpio.h"
#include <string>
#include <time.h>
#include <termios.h>

class ArduinoCom : public Serial {
public:
	ArduinoCom(const std::string &path, int pin);
	int requestVariables(const std::string &source, 
					const std::string &labels, 
					std::string &vars);
	int setHeading(int direction);
public:
	void raiseInterrupt();
	int waitForResponse();

int interruptPin;
int prevDir;
};


#endif