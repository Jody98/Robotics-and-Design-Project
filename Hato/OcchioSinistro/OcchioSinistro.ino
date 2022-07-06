#include <LedControl.h>

//OcchioSinistro
const uint8_t IMAGESLEFT[][8] = {
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
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b01000010,
  0b01000010,
  0b00111100
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
  0b00000000,
  0b00000000,
  0b10000000,
  0b11000000,
  0b11100000,
  0b01111110,
  0b00111100
},{
  0b00000000,
  0b00000001,
  0b00000011,
  0b00000111,
  0b00001111,
  0b00011111,
  0b00111110,
  0b11111100
},{
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01000010,
  0b00000000,
  0b00000000,
  0b00000000
},{
  0b00111100,
  0b01111110,
  0b01000010,
  0b01000010,
  0b00000000,
  0b00000000,
  0b00000000,
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
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b01000010,
  0b01000010,
  0b00111100
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
  //Triste == 2
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000001,
  0b00000011,
  0b00000111,
  0b01111110,
  0b00111100
},{
  //Arrabbiato == 3
  0b00000000,
  0b10000000,
  0b11000000,
  0b11100000,
  0b11110000,
  0b11111000,
  0b01111100,
  0b00111111
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
},{
  //Eccitato == 5
  0b00111100,
  0b01111110,
  0b01000010,
  0b01000010,
  0b00000000,
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

void loop() {
  int devices = lc.getDeviceCount();
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      delay(delaytime);
      displayImage(0, IMAGESLEFT[i]);  //Occhio sinistro address == 0
      displayImage(1, IMAGESRIGHT[i]); //Occhio destro address == 1
      if (++i >= IMAGES_LEN ) {
        i = 0;
      }        
    }
  }
}
