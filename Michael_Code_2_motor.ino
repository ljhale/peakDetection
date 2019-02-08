/*Michael Code [4/11/18] */

#include <Servo.h>
#define TI_SERVO_PIN 11//10

#define MRP_SERVO_PIN 10 // check in with hardware to make sure 
#define MOSFET_PIN_1 13//13
#define MOSFET_PIN_2 3//3

#define MYO_PIN A2//A2
#define OPEN 0
#define CLOSE 180
#define PULSEWIDTH 50 //in ms
#define TRAVELDELAY 650 //in ms
#define SYSTEMDELAY 1500 
#define L_THRESH 150 //550, (0,1024)
#define H_THRESH 150 //get better number from someone
#define RELAXTHRESH 150 // 550 

// #define the location of the new stuff, and probably change the old stuff

Servo ti_servo; // controls thumb and index
Servo mrp_servo; // controls middle, ring, pinky fingers
int ti_pos = OPEN;// position of ti_servo
int mrp_pos = OPEN;//position of mrp_servo
int mos_1 = MOSFET_PIN_1;
int mos_2 = MOSFET_PIN_2;
//
void setup() {
  //Setup IO
  ti_servo.attach(TI_SERVO_PIN);
  mrp_servo.attach(MRP_SERVO_PIN);
  pinMode(MOSFET_PIN_1, OUTPUT);
  pinMode(MOSFET_PIN_2, OUTPUT);
  
/* Set up the stuff needed from hardware*/

  //Set Initial State
  digitalWrite(MOSFET_PIN_1,HIGH);
  digitalWrite(MOSFET_PIN_2,HIGH);
  ti_servo.write(OPEN);
  mrp_servo.write(OPEN);
  delay(TRAVELDELAY);
  digitalWrite(MOSFET_PIN_1,LOW);
  digitalWrite(MOSFET_PIN_2, LOW);
  Serial.begin(9600);
}

//Make an actual template function since this is absolute trash
void servo_logic(int &position, Servo &servo_1, Servo &servo_2, int mos_1, int mos_2 ){ // see what else needs to be passed in, probably the delay or serial ?
  //OPEN to CLOSE
  if (position == OPEN){
    //digitalWrite(MOSFET_PIN_1,HIGH);
    position = CLOSE;
    servo_1.write(CLOSE);
    servo_2.write(CLOSE);
    delay(TRAVELDELAY);
    Serial.println("arm closed");
    digitalWrite(mos_1,LOW);
    digitalWrite(mos_2,LOW);
  }
  //CLOSE to OPEN
 else if (position == CLOSE){
    digitalWrite(mos_1,HIGH);
    digitalWrite(mos_2,HIGH);
    position = OPEN;
    servo_1.write(OPEN);
    servo_2.write(OPEN);
    delay(TRAVELDELAY);
    //digitalWrite(MOSFET_PIN,LOW);
    Serial.println("arm relaxed");
  }
}

void servo_logic(int &pos, Servo &servo, int mos){
    if (position == OPEN){
    //digitalWrite(MOSFET_PIN_1,HIGH);
    pos = CLOSE;
    servo.write(CLOSE);
    delay(TRAVELDELAY);
    Serial.println("arm closed");
    digitalWrite(mos,LOW);
  }
  //CLOSE to OPEN
 else if (position == CLOSE){
    digitalWrite(mos,HIGH);
    pos = OPEN;
    servo.write(OPEN);
    delay(TRAVELDELAY);
    //digitalWrite(MOSFET_PIN,LOW);
    Serial.println("arm relaxed");
  }
}



void loop() {

//Wait for Muscle Signal
while ( analogRead(MYO_PIN) < L_THRESH ) {
  Serial.println(analogRead(MYO_PIN));
  }

//Count up Signal Width 
int count_low = 0,count_high = 0, count_both = 0;
while (analogRead(MYO_PIN) > L_THRESH && analogRead(MYO_PIN) < H_THRESH) {
  delay(1);
  count_low += 1;
  if (count_low>=PULSEWIDTH) {break;} // do we need the break statement if the function is defined?
}

//Toggle Servo Logic
if (count_low >= PULSEWIDTH){
  servo_logic(ti_pos, ti_servo,mos_1); 
    //Keep Signal to Actuation Delay around 1 second.
  delay(SYSTEMDELAY-PULSEWIDTH-TRAVELDELAY);
}

while(analogRead(MYO_PIN) > H_THRESH && H_THRESH == L_THRESH){
  delay(1);
  count_both += 1;
  if(count_both >= PULSEWIDTH){break;}
}

//Toggle Servo Logic
if (count_low >= PULSEWIDTH){
  servo_logic(ti_pos, ti_servo, mrp_servo,mos_1, mos_2); 
    //Keep Signal to Actuation Delay around 1 second.
  delay(SYSTEMDELAY-PULSEWIDTH-TRAVELDELAY);
}

while(analogRead(MYO_PIN) > H_THRESH){
  delay(1);
  count_high += 1; 
  if(count_high >= PULSEWIDTH){break;}
}

//Toggle Servo Logic
if(count_high >= PULSEWIDTH){
  servo_logic(mrp_pos,ti_servo, mrp_servo,mos_1, mos_2);
      //Keep Signal to Actuation Delay around 1 second.
  delay(SYSTEMDELAY-PULSEWIDTH-TRAVELDELAY);
}


//Wait for arm to relax again
while ( analogRead(MYO_PIN) > RELAXTHRESH ) {}
// How would this be adapted for a double motor system?
}
