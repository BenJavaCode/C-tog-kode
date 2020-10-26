//keypad
#include <string.h>

#define Xor(x, y) (x^y)

const byte ROWS = 4; 
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3}; 
char check[10];

byte *rp = rowPins;
byte *cp = colPins;
//keypad




int outPin = 12;



unsigned char preamble[] = {1,1,1,1,1,1,1,1};
unsigned char *preambleP = preamble;
unsigned char address[] = {0,0,0,0,0,1,1,1};
unsigned char *addressP = address;
unsigned char command[] = {0,1,1,0,1,1,1,1};
unsigned char *commandP = command;
unsigned char errD[8];
unsigned char *errDP = errD;


char read_key();

void setup() {
  
  pinMode(outPin, OUTPUT);
  //keypad
  Serial.begin(9600);

  for(int i = 0; i < sizeof(rowPins)/sizeof(byte); i++){
    pinMode(*rp, OUTPUT);
    rp++;
  }
  for (int i = 0; i < sizeof(colPins)/sizeof(byte); i++){
    pinMode(*cp, INPUT_PULLUP);
    cp++;
  }
  rp = rowPins;

}



void loop() {
  
  byteSend(preambleP, 2);
  preambleP = preamble;
  byteSend(preambleP, 2); // 16 bit preamble
  bitSend(0);
  preambleP = preamble;
  
  
  byteSend(addressP, 0);
  addressP = address;
  
  byteSend(commandP, 0);
  commandP = command;
  
  phaseFlip(addressP, commandP, errDP);
  errDP = errD;
  byteSend(errDP, 1); 

  //keypad 
  
   char key = read_key();
   
   if (key == '1'){ 
      Serial.print(key);
      changeCommand();
   }
   
  
}


int bitSend(int val){ // 1=57 , 0=114
  if(val == 0){
    digitalWrite(outPin, HIGH);
    delayMicroseconds(114);
    digitalWrite(outPin, LOW);
    delayMicroseconds(114);
  }
  if(val == 1){
    digitalWrite(outPin, HIGH);
    delayMicroseconds(57);
    digitalWrite(outPin, LOW);
    delayMicroseconds(57);
  }
}

void byteSend(unsigned char *arrOrders, int status){

  for(int i = 0; i<8; i++){
    bitSend(*arrOrders);
    arrOrders++;
  }
  if(status == 0){
    bitSend(0);
  }
  if(status == 1){
    bitSend(1);
  }
  if (status == 2){
    
  }
}

void phaseFlip(unsigned char *arr, unsigned char *arr2, unsigned char*target){
  unsigned char x;
  
  for(int i = 0; i<8; i++){
    //*target = Xor(*arr^*arr2)
    x = *arr^*arr2;
    *target = x;
    
    target++;
    arr++;
    arr2++;
  }
  
}

//keypad
 

char read_key(){
  for(int row = 0; row < 4; row++)
  {
    digitalWrite(*rp,HIGH);
    rp++;
    digitalWrite(*rp,HIGH);
    rp++;
    digitalWrite(*rp,HIGH);
    rp++;
    digitalWrite(*rp,HIGH);
    
    digitalWrite(rowPins[row],LOW);
    rp = rowPins;

    for(int col = 0; col<3; col++)
    {
      int col_state = digitalRead(colPins[col]);
      if(col_state == LOW)
      {
        while(1){ // for some reason i cant say while col_state == LOW
          
              int col_state = digitalRead(colPins[col]);
              if(col_state == HIGH){
                break;
              }
              
            }
        return hexaKeys[row][col];
        }
      }
    }
    return '\n';
}

void changeCommand(){
  commandP+=4;
  
  for(int i = 0; i<4; i++){
    //*commandP = Xor(1,commandP)
    *commandP = 1^*commandP;
    commandP++;
  }
  commandP = command; //made change here
  
}
