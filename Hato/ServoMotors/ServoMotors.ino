#include <Servo.h>

Servo myServoLeft; 
Servo myServoRight;

int posLeft = 0;
int posRight = 0;
int count = 0;

//Enum per le diversee emozioni
enum emotions {
  happy,
  sad,
  angry,
  excited
};

/*const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 10, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); */



void setup() {
  //Inizializzazione dei Servo Motors
  myServoLeft.attach(5);  
  myServoRight.attach(6);
}


//Scelta delle diverse emozioni
void loop() {
      movementHappy(45, 85, 85, 6, 0);
      movementSad(45, 5, 5, 6, 0);
      movementAngry(45, 5, 5, 4, 3);
      movementExcited(45, 85, 85, 4, 4);
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

