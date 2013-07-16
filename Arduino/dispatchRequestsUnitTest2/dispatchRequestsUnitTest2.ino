

#include <Arduino.h>
#include <ashcon.h>
#include <airmar.h>
#include <compass.h>
#include <sensor.h>
#include <rc.h>
#include <pololu_servo.h>
#include <motor.h>

#define MULTIPLEX_PIN1 28
#define MULTIPLEX_PIN2 29
#define SERVO_RESET_PIN 40

typedef struct SensorLink {
	struct SensorLink* next;
	Sensor* s;
} SensorLink;

volatile int mode = 0;

//All the objects necessary on the boat
Airmar* airmar;
Compass* compass;
ashcon* Console;

SensorLink* sensorList;


//Function prototypes
void addToList(Sensor* item);
int dispatchRequest(int argc, char* argv[]);
Sensor* getHottestSensor();
char* comp[] = {"req","compass","compassHeading", "pitch"};

void setup() {
	//Initialize console
    pinMode(13, OUTPUT);

    Serial.begin(115200);
    Console = new ashcon(&Serial);
    Console->user_function_register("req", &dispatchRequest);
    
    sensorList = NULL;
    
    //Initialize multiplexor
    pinMode(MULTIPLEX_PIN1, OUTPUT);
    pinMode(MULTIPLEX_PIN2, OUTPUT);

    //Initialize sensors
    Serial2.begin(9600);
    //airmar = new Airmar("airmar",&Serial2);
    compass = new Compass("compass",&Serial2);
    //addToList(airmar);
    addToList(compass);

    //Setup interrupts
}


void loop(){ 
    dispatchRequest(4, comp);
    
    delay(500);
}

int dispatchRequest(int argc, char* argv[]) {
        digitalWrite(13, HIGH);
	//Need to search through a list of sensors, 
	//and find one that matches argv[1] - 
	//this should be the sensor name. 
	//All following args are variables that are requested.
	SensorLink* link = sensorList;
        Serial.print("Loooking for sensor ");
        Serial.println(argv[1]);
	while(link!=NULL) {
		if(strcmp(link->s->id, argv[1])==0){
                        Serial.println("Found Sensor!");
			break;
		}
		link = link->next;
	}
        
        if(link==NULL){
             //Didn't find a match
              return -1;   
        }

        digitalWrite(13, LOW);
	char** variables = link->s->getVariables(argc-2, &(argv[2]));
	for(int i=0; i<(argc-2); i++) {//Print out all the variables.
		if(variables[i]!=NULL) {
			Serial.print(variables[i]);
                        free(variables[i]);
		} else {
			Serial.print("*");
		}
		Serial.print(",");
		Serial.flush();
                free(variables);
	}
	Serial.print("\n\r");

        Serial.println("Exiting request");
        
	return 0;
}

void addToList(Sensor* item) {
    if(item == NULL){
        return;
    }
    SensorLink* link = (SensorLink*) malloc(sizeof(SensorLink));
    link->next = sensorList;
    link->s = item;
    sensorList = link;
}
