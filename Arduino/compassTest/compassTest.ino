#include <MemoryFree.h>

#include <Arduino.h>
#include <compass.h>

Compass* c;
char* comp[]={"compassHeading", "compassStatus"};
char** vars;
int stat;

void setup() {
	Serial.begin(115200);
	Serial2.begin(9600);
	c = new Compass("compass", &Serial2);
	a = new Airmar("airmar", &Serial2);
        delay(1000);
        
       
}

void loop() {
	//Serial.println(freeMemory());
	Serial2.println("$PTNT,HTM*63");
        stat = c->update();
	if(stat!=0) {
		Serial.println("Polling failed.");
	}else {
		vars = c->getVariables(2,comp);
		if(vars!=NULL){
			Serial.print("Compass Heading: ");
			Serial.print(vars[0]);
			Serial.print(", Status: ");
			Serial.println(vars[1]);
			for(int i = 0; i <2; i++) {
				free(vars[i]);
			}
			free(vars);
		}
	}
    
}
