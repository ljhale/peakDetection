/*Final Julian Code [4/11/18] */

#include <Servo.h>
#define SERVO_PIN 9//19
#define MOSFET_PIN 8//8
#define MYO_PIN A0//A0
#define OPEN 0
#define CLOSE 180
#define PULSEWIDTH 50 //in ms
#define TRAVELDELAY 650 //in ms
#define SYSTEMDELAY 1500 
#define THRESH 810 //(0,1024)
#define RELAXTHRESH 810 //
Servo myservo;
int position = OPEN;

void setup() {
  //Setup IO
  myservo.attach(SERVO_PIN);
  pinMode(MOSFET_PIN, OUTPUT);

  //Set Initial State
  digitalWrite(MOSFET_PIN,HIGH);
  myservo.write(OPEN);
  delay(TRAVELDELAY);
  digitalWrite(MOSFET_PIN,LOW);
  Serial.begin(9600);
  
  
}

void loop() {

//Wait for Muscle Signal
while ( analogRead(MYO_PIN) < THRESH ) {
  //Serial.println(analogRead(MYO_PIN));
  }

//Count up Signal Width 
int count = 0;
while (analogRead(MYO_PIN) > THRESH) {
  delay(1);
  count += 1;
  if (count>=PULSEWIDTH) {break;}
}

//Toggle Servo Logic
if (count >= PULSEWIDTH){

  //OPEN to CLOSE
  if (position == OPEN){
    digitalWrite(MOSFET_PIN,HIGH);
    position = CLOSE;
    myservo.write(CLOSE);
    delay(TRAVELDELAY);
    Serial.println("arm closed");
    //digitalWrite(MOSFET_PIN,LOW);
  }
  //CLOSE to OPEN
 else if (position == CLOSE){
    //digitalWrite(MOSFET_PIN,HIGH);
    position = OPEN;
    myservo.write(OPEN);
    delay(TRAVELDELAY);
    digitalWrite(MOSFET_PIN,LOW);
    Serial.println("arm relaxed");
  }

  //Keep Signal to Actuation Delay around 1 second.
  delay(SYSTEMDELAY-PULSEWIDTH-TRAVELDELAY);
}

//Wait for arm to relax again
while ( analogRead(MYO_PIN) > RELAXTHRESH ) {}

}




 

