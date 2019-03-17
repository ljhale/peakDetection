/*Michael Code [2/11/19] */

#include <Servo.h>
#define TI_SERVO_PIN 11//11
#define MRP_SERVO_PIN 10 //10
#define MOSFET_PIN_1 13//13
#define MOSFET_PIN_2 3//3
#define MYO_PIN A1//A2
#define OPEN 0
#define CLOSE 180
#define PULSEWIDTH 50 //in ms
#define TRAVELDELAY 650 //in ms
#define SYSTEMDELAY 1500 
#define THRESH 150 //550, (0,1024)
#define RELAXTHRESH 150 // 550 

// #define the location of the new stuff, and probably change the old stuff

Servo ti_servo; // controls thumb and index.
Servo mrp_servo; // controls middle, ring, pinky fingers.
int ti_pos = OPEN;// position of ti_servo.
int mrp_pos = OPEN;//position of mrp_servo.
int mos_1 = MOSFET_PIN_1; //mosfet that goes with the TI servo.
int mos_2 = MOSFET_PIN_2; //mosfet that goes with the MRP servo.
int flex_count = 0;

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

void servo_logic(int &pos, Servo &servo, int mos){
    if (pos == OPEN){
    //digitalWrite(MOSFET_PIN_1,HIGH);
    pos = CLOSE;
    servo.write(CLOSE);
    delay(TRAVELDELAY);
    Serial.println("Closed motor connected to mosfet pin at: ");
    Serial.println(mos);
    digitalWrite(mos,LOW);
  }
  //CLOSE to OPEN
  else{
    digitalWrite(mos,HIGH);
    pos = OPEN;
    servo.write(OPEN);
    delay(TRAVELDELAY);
    //digitalWrite(MOSFET_PIN,LOW);
    Serial.println("Opened motor connected to mosfet pin at: ");
    Serial.println(mos);
  }
}

void loop() {

//Wait for Muscle Signal
while ( analogRead(MYO_PIN) < THRESH ) {
  Serial.println(analogRead(MYO_PIN));
  }

//Count up Signal Width 
int count = 0;
while (analogRead(MYO_PIN) > THRESH) {
  delay(1);
  count += 1;
  if (count>=PULSEWIDTH) {break;} // do we need the break statement if the function is defined?
}

//Toggle Servo Logic
//Keep Signal to Actuation Delay around 1 second.
if (count >= PULSEWIDTH){
  // We start up with a closed hand
  if(flex_count %4 ==0){
    // open thumb and index finger - finger gun?
    servo_logic(ti_pos, ti_servo,mos_1); 
    delay(SYSTEMDELAY-PULSEWIDTH-TRAVELDELAY);
  }
  else if (flex_count %4 == 1){
    //open entire hand by opening middle, ring, and pinky fingers.
    servo_logic(mrp_pos, mrp_servo,mos_2); 
    delay(SYSTEMDELAY-PULSEWIDTH-TRAVELDELAY);
  }
  else if (flex_count %4 ==2){
    // close thumb and index to get open mrp only position
    servo_logic(ti_pos, ti_servo,mos_1); 
    delay(SYSTEMDELAY-PULSEWIDTH-TRAVELDELAY);
  }
  else if (flex_count %4 ==3){
    //close mrp to get closed hand
    servo_logic(mrp_pos, mrp_servo,mos_2); 
    delay(SYSTEMDELAY-PULSEWIDTH-TRAVELDELAY);
  }
  ++flex_count;
}

//Wait for arm to relax again
while ( analogRead(MYO_PIN) > RELAXTHRESH ) {}
}
