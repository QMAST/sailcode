#include <string>
#include <iostream>
#include <unistd.h>
#include "logging.h"
#include "arduinoCom.h"
#include "gpio.h"

using namespace std;

ArduinoCom* ac;

int main(int argc, char* argv[]) {

	//Simple place to test individual features.
	ac = new ArduinoCom("/dev/ttyACM0", 2);

	if(!(ac->isValid())) {
		cout<<"Arduino failed to open"<<endl;
		return -1;
	}

	std::string str = "";
	Logging::init();
	double* test1 = new double;
	double* test2 = new double;

	Logging::addDataSource(DOUBLE, "test1", test1);
	Logging::addDataSource(DOUBLE, "test2", test2);

	char* buf;
	int stat;
	for(int i=0;i<10;i++) {
		cout<<"Request no. "<<i<<endl;
		stat = ac->requestVariables("test","test1 test2", str);
		if(stat==0) {
			buf = new char[str.length()];
			if(buf==NULL) {
				Logging::error(__func__, "Run out of memory");
				return 1;
			}
			strcpy(buf, str.c_str());
			*test1 = atof(strtok(buf, ","));
			*test2 = atof(strtok(NULL, ","));

			delete[] buf;
			cout<<str<<endl;
			stat=Logging::log();
			
		} else {
			cout<<"Request failed"<<endl;
		}
		
	}
	cout<<"End test"<<endl;

	Logging::clean();
	delete test1;
	delete test2;
	return 0;
}