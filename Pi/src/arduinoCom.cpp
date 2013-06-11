#include "arduinoCom.h"

ArduinoCom::ArduinoCom(const std::string &path, int pin){
	//Takes as an argument the pin 
	//which the arduino interrupt is connected to, 
	//and the path of the usb port which that same arduino is hooked up to.


	GPIO::init();
	GPIO::setPin(pin, OUTPUT);
	this->interruptPin = pin;

	this->openPort(path);

}

int ArduinoCom::requestVariables(const std::string &source ,
							const std::string &labels, 
							std::string &vars) {
	/*
		This method takes a name of a Sensor (registered in the arduino),
		And a space-separated string of variable id's
		and returns a comma seperated string of those variable values.
		Ex: 
		requestVariables("airmar","lat lon heading",vars);
	*/

	std::string resp = "";
	//First, raise an interrupt, and wait for a '>' from the arduino.
	GPIO::digitalWrite(this->interruptPin, HIGH);
	int stat = this->readBlock(resp);
	if(stat!=0) {
		//Request failed.
		Logging::error(__func__, "Variable request failed. Arduino not responsive: "+resp);
		return -1; //Error getting info from arduino.
	}
	if(resp.find("> ")==-1) {//Arduino is not responding.
	
	}
	stat = this->sendCommand("req "+source+" "+labels, vars);
	if(stat!=0) {
		Logging::error(__func__,"Variables not returned. Response: "+vars);
		return -1;
	}
	GPIO::digitalWrite(this->interruptPin, LOW);
	return 0;
}