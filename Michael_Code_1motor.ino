/*Michael Code [2/9/19] */

#include <Servo.h>
#define SERVO_PIN 11//10

#define MOSFET_PIN 13//13

#define MYO_PIN A2//A2
#define OPEN 0
#define CLOSE 180
#define PULSEWIDTH 50 //in ms
#define TRAVELDELAY 650 //in ms
#define SYSTEMDELAY 1500 
#define THRESH 550 //550, (0,1024)
#define RELAXTHRESH 550 // 550 

// #define the location of the new stuff, and probably change the old stuff

Servo servo; // controls thumb and index
//int pos = OPEN;// position of ti_servo
int mos = MOSFET_PIN;
//
void setup() {
  //Setup IO
  servo.attach(SERVO_PIN);
  pinMode(MOSFET_PIN, OUTPUT);
  
/* Set up the stuff needed from hardware*/

  //Set Initial State
  digitalWrite(MOSFET_PIN,HIGH);
  servo.write(OPEN);
  delay(TRAVELDELAY);
  digitalWrite(MOSFET_PIN,LOW);
  Serial.begin(9600);
}

void servo_logic( Servo &servo, int mos){
    if (position == OPEN){
    //digitalWrite(MOSFET_PIN_1,HIGH);
    position = CLOSE;
    servo.write(CLOSE);
    delay(TRAVELDELAY);
    Serial.println("arm closed");
    digitalWrite(mos,LOW);
  }
  //CLOSE to OPEN
 else if (position == CLOSE){
    digitalWrite(mos,HIGH);
    position = OPEN;
    servo.write(OPEN);
    delay(TRAVELDELAY);
    //digitalWrite(MOSFET_PIN,LOW);
    Serial.println("arm relaxed");
  }
}

void loop() {

//Wait for Muscle Signal
while ( analogRead(MYO_PIN) < THRESH ) {
  Serial.println(analogRead(MYO_PIN));
  // Return Battery Display Information
  }

//Count up Signal Width 
int count_low = 0,count_high = 0, count_both = 0;
while (analogRead(MYO_PIN) > THRESH) {
  delay(1);
  count_low += 1;
  if (count_low>=PULSEWIDTH) {break;} 
}

//Toggle Servo Logic
if (count_low >= PULSEWIDTH){
  servo_logic(pos, servo,mos); 
    //Keep Signal to Actuation Delay around 1 second.
    // Return Battery Display Information
  delay(SYSTEMDELAY-PULSEWIDTH-TRAVELDELAY);
}

//Wait for arm to relax again
while ( analogRead(MYO_PIN) > RELAXTHRESH ) {}
// How would this be adapted for a double motor system?
}
