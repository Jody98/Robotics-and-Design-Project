#include <LedControl.h>

//Occhio destro
const uint8_t IMAGES[][8] = {
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
  //Base
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100,
  0b00000000
},{
  //Triste
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b01000010,
  0b01000010,
  0b00111100
},{
  //Pensante
  0b00000000,
  0b00011110,
  0b00111111,
  0b00111111,
  0b00111111,
  0b00111111,
  0b00011110,
  0b00000000
},{
  //Pensante
  0b00000000,
  0b01111000,
  0b11111100,
  0b11111100,
  0b11111100,
  0b11111100,
  0b01111000,
  0b00000000
},{
  //Triste
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000001,
  0b00000011,
  0b00000111,
  0b01111110,
  0b00111100
},{
  //Arrabbiato
  0b00000000,
  0b10000000,
  0b11000000,
  0b11100000,
  0b11110000,
  0b11111000,
  0b01111100,
  0b00111111
},{
  //Felice
  0b00000000,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01000010,
  0b00000000,
  0b00000000,
  0b00000000
},{
  //Eccitato
  0b00111100,
  0b01111110,
  0b01000010,
  0b01000010,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
}};
const int IMAGES_LEN = sizeof(IMAGES)/8;

/*
 DIN connects to pin 12
 CLK connects to pin 11
 CS connects to pin 10 
*/
LedControl lc=LedControl(12,11,10,1);

void setup() {
  lc.clearDisplay(0);
  lc.shutdown(0, false);
  lc.setIntensity(0,5);
}


void displayImage(const byte* image) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, i, j, bitRead(image[i], 7 - j));
    }
  }
}

int i = 0;

void loop() {
  displayImage(IMAGES[i]);
  if (++i >= IMAGES_LEN ) {
    i = 0;
  }
  delay(333);
}
