#include <LedControl.h>


const int OUT_PIN = 8;
//OcchioSinistro
const uint8_t IMAGESLEFT[][8] = {
{
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
  0b00011110,
  0b00111111,
  0b00111111,
  0b00111111,
  0b00111111,
  0b00011110,
  0b00000000
  
},{
  0b00000000,
  0b01111000,
  0b11111100,
  0b11111100,
  0b11111100,
  0b11111100,
  0b01111000,
  0b00000000
},{
  0b00000000,
  0b10000000,
  0b11000000,
  0b11100000,
  0b11110000,
  0b11111000,
  0b01111100,
  0b00111111
},{
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01000010,
  0b00000000,
  0b00000000,
  0b00000000
}};

//Occhio Destro
const uint8_t IMAGESRIGHT[][8] = {
{
  //Base == 0
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100,
  0b00000000
},{
  //Pensante == 1
  0b00000000,
  0b00011110,
  0b00111111,
  0b00111111,
  0b00111111,
  0b00111111,
  0b00011110,
  0b00000000
},{
  //Pensante == 1
  0b00000000,
  0b01111000,
  0b11111100,
  0b11111100,
  0b11111100,
  0b11111100,
  0b01111000,
  0b00000000
},{
  //Arrabbiato == 3
  0b00000000,
  0b00000001,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00011111,
  0b00111110,
  0b11111100
},{
  //Felice == 4
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01000010,
  0b00000000,
  0b00000000,
  0b00000000
}};
const int IMAGES_LEN = sizeof(IMAGESLEFT)/8;


//12 == DataIn
//11 == CLK
//10 == LOAD(CS)
LedControl lc = LedControl(12, 11, 10, 8);

unsigned long delaytime = 1500;

void setup() { 
  Serial.begin(9600);
  pinMode(7, INPUT); 
  //Inizializziamo le varie matrici in serie
  int devices=lc.getDeviceCount();
  
  for(int address=0;address<devices;address++) {
    lc.shutdown(address,false);
    lc.setIntensity(address,20);
    lc.clearDisplay(address);
  }  
}

void displayImage(int address, const byte* image) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setLed(address, i, j, bitRead(image[i], 7 - j));
    }
  }
}

int i = 0;
int neverDone = 0;

void loop() {
    delay(delaytime);
    
  Serial.println(digitalRead(OUT_PIN));
    if (digitalRead(OUT_PIN) == 1) {
      displayImage(0, IMAGESLEFT[3]);
      displayImage(1, IMAGESRIGHT[3]);
      i = 0;
    }
    if (digitalRead(7) == HIGH && neverDone == 0) {
      neverDone = 1;
      displayImage(0, IMAGESLEFT[4]);
      displayImage(1, IMAGESRIGHT[4]);
      delay(2000);
      i = 0;
    }
    if (digitalRead(OUT_PIN) == 0) {
      
      
    displayImage(0, IMAGESLEFT[i]);  //Occhio sinistro address == 0
    displayImage(1, IMAGESRIGHT[i]); //Occhio destro address == 1
      
    }
    if (++i >= 3) {
      i = 0;
    }        
}
