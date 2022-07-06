#include <StateMachine.h>
#include <LedControl.h>
#include <Servo.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define STATE_DELAY 2000
#define PIN_LED0 7
#define PIN_LED1 12
#define PIN_MATRIX_LOADCS 2
#define PIN_MATRIX_CLK 3
#define PIN_MATRIX_DATAIN 4
#define PIN_SERVOLEFT 5
#define PIN_SERVORIGHT 6 
#define PIN_FSR A0 
#define PIN_BUTTON1 8
#define PIN_BUTTON2 9
#define PIN_MP3PLAYERRX 10
#define PIN_MP3PLAYERTX 11
#define PIN_MICROPHONE 13

//OcchioSinistro
const uint8_t IMAGESLEFT[][8] = {
{ //Nullo == 0
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
},{ //Base == 1 [1]
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100,
  0b00000000
},{ //Palpebre == 2 [2]
  0b00000000,
  0b01100000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b01100000,
  0b00000000
},{ //Pensante == 3 [3]
  0b00000000,
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100
},{ //Pensante == 3 [4]
  0b00111100,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100,
  0b00000000,
  0b00000000
},{ //Trieste == 4 [5]
  0b00111000,
  0b01110000,
  0b11100000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b01000000,
  0b00000000
},{
  0b10000000,
  0b10000000,
  0b11000000,
  0b11100000,
  0b11110000,
  0b11111000,
  0b01111100,
  0b00111110
},{
  0b00000000,
  0b00011100,
  0b00001110,
  0b00001110,
  0b00001110,
  0b00001110,
  0b00011100,
  0b00000000
},{
  0b00000000,
  0b00001110,
  0b00000011,
  0b00000011,
  0b00000011,
  0b00000011,
  0b00001110,
  0b00000000
}};


//Occhio Destro
const uint8_t IMAGESRIGHT[][8] = {
{
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
},{
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100,
  0b00000000
},{
  0b00000000,
  0b01100000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b01100000,
  0b00000000
},{
  0b00000000,
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100
},{
  0b00111100,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100,
  0b00000000,
  0b00000000
},{
  0b00000000,
  0b01000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11100000,
  0b01110000,
  0b00111000
},{
  0b00111110,
  0b01111100,
  0b11111000,
  0b11110000,
  0b11100000,
  0b11000000,
  0b10000000,
  0b10000000
},{
  0b00000000,
  0b00011100,
  0b00001110,
  0b00001110,
  0b00001110,
  0b00001110,
  0b00011100,
  0b00000000
},{
  0b00000000,
  0b00001110,
  0b00000011,
  0b00000011,
  0b00000011,
  0b00000011,
  0b00001110,
  0b00000000
}};

int animat_base[12] = {1, 2, 1, 3, 4, 1, 200, 200, 500, 500, 500, 0}; 
int animat_base_listen[6] = {1, 2, 1, 5000, 200, 0}; //il robot mentre ascolta sbatte solo le palpebre senza guardarsi intorno


const int IMAGES_LEN = sizeof(IMAGESLEFT)/8;

LedControl lc = LedControl(PIN_MATRIX_DATAIN, PIN_MATRIX_CLK, PIN_MATRIX_LOADCS, 2);

Servo myServoLeft; 
Servo myServoRight;

unsigned long delaytime = 1500;

SoftwareSerial mySoftwareSerial(PIN_MP3PLAYERRX, PIN_MP3PLAYERTX); 
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


int posLeft = 0;
int posRight = 0;
int count = 0;
int count_state_2 = 0;
int flag_init = 0;
int flag_hato = 0;
int flag_yellow = 0;
int threshold = 10;
float sum_val = 0.0;
int avg_size = 10; // number of analog readings to average
float R_0 = 4700.0; // known resistor value in [Ohms]
float Vcc = 5.0; // supply voltage
float R_FSR;

StateMachine machine = StateMachine();

State* S0 = machine.addState(&state0); //waitToPutOnHead1
State* S1 = machine.addState(&state1); //happyMode+raccontami
State* S2 = machine.addState(&state2); //listening1
State* S3 = machine.addState(&state3);  //angryMode
State* S4 = machine.addState(&state4);  //excitedPlaySound1
State* S45 = machine.addState(&state45);
State* S5 = machine.addState(&state5);  
State* S6 = machine.addState(&state6);  
State* S7 = machine.addState(&state7);  
State* S8 = machine.addState(&state8);  
State* S9 = machine.addState(&state9);  
State* S10 = machine.addState(&state10);  
State* SINIT1 = machine.addState(&stateINIT1);  
State* SRESET = machine.addState(&stateRESET);

enum states{
  waitToPutOnHead1,
  happyMode_raccontami,
  listening1, 
  angryMode,
  excitedPlaySound1,
};

void loop() {
  
  
  machine.run();
  delay(STATE_DELAY);
  float sum_val = 0.0; // variable for storing sum used for averaging
  for (int ii=0;ii<avg_size;ii++){
    sum_val+=(analogRead(PIN_FSR)/1023.0)*5.0; // sum the 10-bit ADC ratio
    delay(10);
  }
  sum_val/=avg_size; // take average

  R_FSR = (R_0/1000.0)*((Vcc/sum_val)-1.0); // calculate actual FSR resistance
 
  Serial.println(R_FSR); // print to serial port
  delay(10);
}

//Movimento di entrambi i motori, ogni volta che termina un'emozione torna alla posizione di base
void movementHappy(int finalPosition, int middlePosition, int transPosition, int myDelay, int numberOfMovement){
  
  while(count < numberOfMovement) {
    for (posLeft = 180 - finalPosition, posRight = finalPosition; posLeft >= middlePosition + 10 && posRight <= middlePosition; posLeft -= 1, posRight += 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);  
      delay(myDelay);                       
    }

    for (posLeft = middlePosition + 10, posRight = middlePosition; posLeft <= 180 - finalPosition && posRight >= finalPosition; posLeft += 1, posRight -= 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);
      delay(myDelay);          
    }
    
    count++;
  }
  count = 0;
  for (posLeft = 180 - finalPosition, posRight = finalPosition; posLeft >= middlePosition + 10 && posRight <= middlePosition; posLeft -= 1, posRight += 1) { 
      myServoLeft.write(posLeft); 
      myServoRight.write(posRight); 
      delay(myDelay);                       
  }

  //Torna alla posizione base dopo 2 secondi
  delay(2000);

  for (posLeft = middlePosition + 10, posRight = middlePosition; posLeft <= 180 - finalPosition && posRight >= finalPosition; posLeft += 1, posRight -= 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);
      delay(myDelay);          
  }

  delay(2000);
}


//Movimento di entrambi i motori, ogni volta che termina un'emozione torna alla posizione di base
void movementSad(int finalPosition, int middlePosition, int transPosition, int myDelay, int numberOfMovement){

  while(count < numberOfMovement) {
     for (posLeft = 180 - finalPosition, posRight = finalPosition;  posLeft <= 180 - middlePosition && posRight >= middlePosition; posLeft += 1, posRight -= 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);  
      delay(myDelay);          
    }
    
    for (posLeft = 180 - middlePosition, posRight = middlePosition; posLeft >= 180 - finalPosition && posRight <= finalPosition ; posLeft -= 1, posRight += 1) { 
      myServoLeft.write(posLeft);  
      myServoRight.write(posRight);
      delay(myDelay);                       
    }    
    
    count++;
  }
  count = 0;
  for (posLeft = 180 - finalPosition, posRight = finalPosition;  posLeft <= 180 - middlePosition && posRight >= middlePosition; posLeft += 1, posRight -= 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);  
      delay(myDelay);
  }

  //Torna alla posizione base dopo 2 secondi
  delay(2000);

  for (posLeft = 180 - middlePosition, posRight = middlePosition; posLeft >= 180 - finalPosition && posRight <= finalPosition ; posLeft -= 1, posRight += 1) { 
      myServoLeft.write(posLeft);  
      myServoRight.write(posRight);
      delay(myDelay);                       
  }  

  delay(2000);  
}


//Movimento di entrambi i motori, ogni volta che termina un'emozione torna alla posizione di base
void movementAngry(int finalPosition, int middlePosition, int transPosition, int myDelay, int numberOfMovement){

 while(count < numberOfMovement) {
     for (posLeft = 180 - finalPosition, posRight = finalPosition;  posLeft <= 180 - middlePosition && posRight >= middlePosition; posLeft += 1, posRight -= 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);  
      delay(myDelay);          
    }
    
    for (posLeft = 180 - middlePosition, posRight = middlePosition; posLeft >= 180 - finalPosition && posRight <= finalPosition ; posLeft -= 1, posRight += 1) { 
      myServoLeft.write(posLeft);  
      myServoRight.write(posRight);
      delay(myDelay);                       
    }    
    
    count++;
  }
  count = 0;
  for (posLeft = 180 - finalPosition, posRight = finalPosition;  posLeft <= 180 - middlePosition && posRight >= middlePosition; posLeft += 1, posRight -= 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);  
      delay(myDelay);
  }

  //Torna alla posizione base dopo 2 secondi
  delay(2000);

  for (posLeft = 180 - middlePosition, posRight = middlePosition; posLeft >= 180 - finalPosition && posRight <= finalPosition ; posLeft -= 1, posRight += 1) { 
      myServoLeft.write(posLeft);  
      myServoRight.write(posRight);
      delay(myDelay);                       
  }  

  delay(2000);  
}

//Movimento di entrambi i motori, ogni volta che termina un'emozione torna alla posizione di base
void movementExcited(int finalPosition, int middlePosition, int transPosition, int myDelay, int numberOfMovement){

  while(count < numberOfMovement) {
    for (posLeft = 180 - finalPosition, posRight = finalPosition; posLeft >= middlePosition + 10 && posRight <= middlePosition; posLeft -= 1, posRight += 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);  
      delay(myDelay);                       
    }

    for (posLeft = middlePosition + 10, posRight = middlePosition; posLeft <= 180 - finalPosition && posRight >= finalPosition; posLeft += 1, posRight -= 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);
      delay(myDelay);          
    }
    
    count++;
  }
  count = 0;
  for (posLeft = 180 - finalPosition, posRight = finalPosition; posLeft >= middlePosition + 10 && posRight <= middlePosition; posLeft -= 1, posRight += 1) { 
      myServoLeft.write(posLeft); 
      myServoRight.write(posRight); 
      delay(myDelay);                       
  }

  //Torna alla posizione base dopo 2 secondi
  delay(2000);

  for (posLeft = middlePosition + 10, posRight = middlePosition; posLeft <= 180 - finalPosition && posRight >= finalPosition; posLeft += 1, posRight -= 1) { 
      myServoLeft.write(posLeft);
      myServoRight.write(posRight);
      delay(myDelay);          
  }

  delay(2000); 
}


void setupToreset() {
  posLeft = 0;
  posRight = 0;
  count_state_2 = 0;
  flag_init = 0;
  flag_yellow = 0;
  count = 0;
  return;
}


void setup() {
  pinMode(PIN_FSR, INPUT);
  pinMode(PIN_MICROPHONE, INPUT);
  pinMode(PIN_LED0, OUTPUT);
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_BUTTON1, INPUT_PULLUP);  
  pinMode(PIN_BUTTON2, INPUT_PULLUP);

  Serial.begin(9600);
  myServoLeft.attach(PIN_SERVOLEFT);  
  myServoRight.attach(PIN_SERVORIGHT);
  
  myServoLeft.write(135);
  myServoRight.write(45);

  digitalWrite(PIN_LED0, LOW);
  digitalWrite(PIN_LED1, LOW);


  //#######LEDS########
  //Inizializziamo le varie matrici in serie
  int devices=lc.getDeviceCount();
  
  for(int address=0;address<devices;address++) {
    lc.shutdown(address,false);
    if(address == 0){
      lc.setIntensity(address,10);  
    }
    else if(address == 1){
      lc.setIntensity(address, 10);
    }
    
    lc.clearDisplay(address);
  }  

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(30);  //Set volume value. From 0 to 30  
}

bool print_message(const char *m) {
  Serial.print("print_message: ");
  Serial.println(m);
  return true; 
}

//############################STATI################################

void state0(){
  //stato normale, rimane in attesa di essere messo in testa
  Serial.println("stato 0");
  if(flag_hato == 0){
    flag_hato = 1;
    delay(2000);
    myDFPlayer.play(15); //"Hi, i'm Hato!"
  }
  delay(1000);
  sum_val = 0.0;
  for(int i = 0; i < 6; i++) {
    displayImage(0, IMAGESLEFT[animat_base[i]]);  //Occhio sinistro address == 0
    displayImage(1, IMAGESRIGHT[animat_base[i]]);
    for (int ii=0;ii<avg_size;ii++){
    sum_val+=(analogRead(PIN_FSR)/1023.0)*5.0; // sum the 10-bit ADC ratio
    delay(10);
  }
  
  sum_val/=avg_size; // take average

  R_FSR = (R_0/1000.0)*((Vcc/sum_val)-1.0); // calculate actual FSR resistance
    if (R_FSR <= threshold) {
      machine.transitionTo(S1);
      break;
    }
    delay(animat_base[i+6]);
  }
}

void state1(){
      myDFPlayer.play(1); //"Oh, Hi"
      Serial.println("stato 1");
      displayImage(0, IMAGESLEFT[7]);  //Occhio sinistro address == 0
      displayImage(1, IMAGESRIGHT[7]); //Occhio destro address == 1
      movementHappy(45, 85, 85, 6, 0);
      //delay(1000);
      if(flag_init == 0){
        myDFPlayer.play(2); //"Tell me what Happened"
      }
      else if(flag_init == 1){
        myDFPlayer.play(13); //"Now it's your turn"
      }
      machine.transitionTo(S2); 
}

void state2(){
  //stato listening, parte un timer, se qualcuno urla in questo lasso di tempo si passa allo state3, altrimenti quando scadel il timer si passa allo stato 4
  
  Serial.println("stato 2");
  for(int i = 0; i < 3; i++) {
    displayImage(0, IMAGESLEFT[animat_base_listen[i]]);  //Occhio sinistro address == 0
    displayImage(1, IMAGESRIGHT[animat_base_listen[i]]);
    delay(animat_base_listen[i+3]);
    count_state_2 += 1;
    if (digitalRead(PIN_MICROPHONE) == 1) {
        Serial.println(digitalRead(PIN_MICROPHONE));
        machine.transitionTo(S3);
      }
   }
 if(count_state_2 > 5){ //parametro che più aumenta e più aumenta la fase di ascolto del robot(25 == 20 secondi circa)
  
  machine.transitionTo(S4);
  count_state_2 = 0;
 }
}

void state3(){
  //stato angry, fa movimenti angry mentre dice di non urlare
  myDFPlayer.play(12); //"Hey don't scream, let your friend explain what happened"
  Serial.println("stato 3");
  displayImage(0, IMAGESLEFT[6]);  //Occhio sinistro address == 0
  displayImage(1, IMAGESRIGHT[6]); //Occhio destro address == 1
  movementAngry(45, 5, 5, 4, 3);
  delay(500);
  machine.transitionTo(S2);
}

void state4(){
  //stato sad dopo che uno dei due bambini ha parlato
  Serial.println("stato 4");
  displayImage(0, IMAGESLEFT[5]); 
  displayImage(1, IMAGESRIGHT[5]);  
  movementSad(45, 5, 5, 6, 0);
  delay(500);
  if(flag_init == 0){
    myDFPlayer.play(4); //"Oh, I see, now let's hear your friend's point of view"
    myDFPlayer.play(10); //"Pass me to your friend"
    delay(1000);
  }
  machine.transitionTo(SINIT1);
}

void state45(){
  if(flag_init == 2){
    machine.transitionTo(S5);
  }
  else if(flag_init == 3){
    sum_val = 0.0;
    for(int i = 0; i < 6; i++) {
      displayImage(0, IMAGESLEFT[animat_base[i]]);  //Occhio sinistro address == 0
      displayImage(1, IMAGESRIGHT[animat_base[i]]);
      for (int ii=0;ii<avg_size;ii++){
        sum_val+=(analogRead(PIN_FSR)/1023.0)*5.0; // sum the 10-bit ADC ratio
        delay(10);
      }
  
      sum_val/=avg_size; // take average

      R_FSR = (R_0/1000.0)*((Vcc/sum_val)-1.0); // calculate actual FSR resistance
      if (R_FSR <= threshold) {
        displayImage(0, IMAGESLEFT[7]); 
        displayImage(1, IMAGESRIGHT[7]); 
        movementHappy(45, 85, 85, 6, 0);
        machine.transitionTo(S5);
        break;
      }
      delay(animat_base[i+6]);
    }
  }
}

void state5(){
  //stato listening seconda fase
  for(int i = 0; i < 3; i++) {
    displayImage(0, IMAGESLEFT[animat_base_listen[i]]);  //Occhio sinistro address == 0
    displayImage(1, IMAGESRIGHT[animat_base_listen[i]]);
    delay(animat_base_listen[i+3]);
    count_state_2 += 1;
    if (digitalRead(PIN_MICROPHONE) == HIGH) {
        count_state_2 -= 1;
        machine.transitionTo(S6);
    }
  }
  if(count_state_2 > 5){ //parametro che più aumenta e più aumenta la fase di ascolto del robot
    count_state_2 = 0;
    machine.transitionTo(S7);
  }
}

void state6(){
  //stato angry, fa movimenti angry mentre dice di non urlare
  myDFPlayer.play(12); //"Hey don't scream, let your friend explain what happened"
  displayImage(0, IMAGESLEFT[6]);  //Occhio sinistro address == 0
  displayImage(1, IMAGESRIGHT[6]); //Occhio destro address == 1
  movementAngry(45, 5, 5, 4, 3);
  delay(500);
  machine.transitionTo(S5);
}

void state7(){
  // stato excited quando uno dei due spiega
  Serial.println("stato 7");
  displayImage(0, IMAGESLEFT[7]); 
  displayImage(1, IMAGESRIGHT[7]);
  movementHappy(45, 85, 85, 6, 0);
  delay(500);
  if(flag_init == 2){
     myDFPlayer.play(6); //"Ok,good job! Now it's your friend turn"
  }
  machine.transitionTo(SINIT1);
}

void state8(){
  // stato excited con led accesi 
  Serial.println("stato 8");
  displayImage(0, IMAGESLEFT[7]); 
  displayImage(1, IMAGESRIGHT[7]);
  movementHappy(45, 85, 85, 6, 0);
  digitalWrite(PIN_LED0, HIGH);
  digitalWrite(PIN_LED1, HIGH);
  machine.transitionTo(S9);
}

void state9(){
  // stato in cui si attende la lettura dei bottoni
  if (flag_yellow == 0) {
    myDFPlayer.play(11);  //"Squeeze the RED tentacles together if you want to make peace"
    flag_yellow = 1;
  }
  if (digitalRead(PIN_BUTTON1) == LOW && digitalRead(PIN_BUTTON2) == LOW){
    delay(500);
    digitalWrite(PIN_LED0, LOW);
    digitalWrite(PIN_LED1, LOW);
    displayImage(0, IMAGESLEFT[8]); 
    displayImage(1, IMAGESRIGHT[8]);
    machine.transitionTo(S10);
  }
}

void state10(){
  Serial.println("stato 10");
  myDFPlayer.play(9); //"Good job! I'm happy you solved your issue"
  movementExcited(45, 85, 85, 4, 4);
  delay(300);
  myDFPlayer.play(14); //"Goodbye guys"
  machine.transitionTo(SRESET);
}

void stateINIT1(){
  //stato per rilevare cappello rimosso 
  sum_val = 0.0;
  for (int i=0;i<avg_size;i++){
    sum_val+=(analogRead(PIN_FSR)/1023.0)*5.0; // sum the 10-bit ADC ratio
    delay(10);
  }
  sum_val/=avg_size; // take average

  R_FSR = (R_0/1000.0)*((Vcc/sum_val)-1.0); // calculate actual FSR resistance
  
  if(R_FSR <= threshold && (flag_init == 0 || flag_init == 2)){
      for(int i = 0; i < 3; i++) {
        displayImage(0, IMAGESLEFT[animat_base_listen[i]]);  //Occhio sinistro address == 0
        displayImage(1, IMAGESRIGHT[animat_base_listen[i]]);
        delay(animat_base_listen[i+3]);
      }
  }
  else{
    if (flag_init == 0) {
      flag_init++;
      machine.transitionTo(S0);
    }
    else if (flag_init == 1 || flag_init == 2) { 
      flag_init++;
      if(flag_init == 2){
        myDFPlayer.play(5); //"I'm sorry you too fought, now let's do an exercise: try to explain what your friend said"
        delay(500);
      }
      count_state_2 = 0;
      machine.transitionTo(S45);
    }
    else if (flag_init == 3) {
      myDFPlayer.play(7); //"Perfect! Very good guys"
      delay(500);
      machine.transitionTo(S8);
    }
  }  
}


void stateRESET() {
  //stato che chiamiamo quando resettiamo
  Serial.println("Reset");
  setupToreset();
  machine.transitionTo(S0);
}


void displayImage(int address, const byte* image) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setLed(address, i, j, bitRead(image[i], 7 - j));
    }
  }
}
