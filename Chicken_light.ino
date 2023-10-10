
#include <TimerOne.h>

#define RLY_LEFT  3
#define RLY_RIGHT 2
#define LED_POWER 4
#define LED_FORWARD 5
#define LED_REVERSE 6
#define LIGHT_SENSOR 0
#define SENSIVITY 1
#define TIMER_1 2
#define TIMER_2 3

#define TIMER_X 30        // coefficient that sets the maximum delay in seconds

#define HYSTERESIS 10

#define DEBUGGING 0

byte NewEvent = 0;
byte Function = 0;
unsigned int light_value, sensivity_value, timer1_value, timer2_value, delay_X1, delay_X2;

void setup() {

pinMode(RLY_LEFT, OUTPUT);       // RELAY LEFT OUTPUT
pinMode(RLY_RIGHT, OUTPUT);      // RELAY RIGHT OUTPUT
pinMode(LED_POWER, OUTPUT);      // LED "POWER" OUTPUT
pinMode(LED_FORWARD, OUTPUT);    // LED "FORWARD" OUTPUT
pinMode(LED_REVERSE, OUTPUT);    // LED "REVERSE" OUTPUT

digitalWrite(LED_POWER, 1);      // Turn ON led "POWER"

Serial.begin(115200);

Timer1.attachInterrupt(timerIsr);
Timer1.initialize(100000); 

}

void timerIsr() {

light_value = analogRead(LIGHT_SENSOR);
sensivity_value = analogRead(SENSIVITY);
timer1_value = analogRead(TIMER_1);
timer2_value = analogRead(TIMER_2);

if (light_value > (sensivity_value+HYSTERESIS)) { NewEvent = 1; }    // High sunlight level, gates open
if (light_value < (sensivity_value-HYSTERESIS)) { NewEvent = 2; }    // Low sunlight level, gates closed

// ........low level...<-.*.->...high level...........

//////////////////////////////////////////

if (DEBUGGING) { 
                Serial.print("|   ");
                Serial.print(light_value);     Serial.print("   |   ");
                Serial.print(sensivity_value); Serial.print("   |   ");
                Serial.print(timer1_value);    Serial.print("   |   ");
                Serial.print(timer2_value);  Serial.println("   |   ");
               }

}



void loop() {


switch(NewEvent) { 
                  case 1: if(Function == 0) 
                          {
                           if (DEBUGGING) {Serial.println("|   GATE OPEN   |");}
                           
                           delay_X1 = timer1_value * TIMER_X;
                           digitalWrite(RLY_RIGHT, 1);   
                           digitalWrite(LED_FORWARD, 1); delay(delay_X1); digitalWrite(LED_FORWARD, 0); 
                           digitalWrite(RLY_RIGHT, 0);   
                           Function = 1;
                          } 
                          break;

                  case 2: if(Function == 1) 
                          {
                           if (DEBUGGING) {Serial.println("|   GATE CLOSE   |");}
                           
                           delay_X2 = timer2_value * TIMER_X;
                           digitalWrite(RLY_LEFT, 1);   
                           digitalWrite(LED_REVERSE, 1); delay(delay_X2); digitalWrite(LED_REVERSE, 0); 
                           digitalWrite(RLY_LEFT, 0); 
                           Function = 0;
                          } 
                          break;                
                  }
              
      
}
