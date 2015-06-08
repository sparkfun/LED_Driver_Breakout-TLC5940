#include "Tlc5940.h"

//LED Matrix Stuff

int pixel[12][6] = { //locations of LED channels in meatspace
  {56,64,44,35,15,11},
  {55,63,43,34,14,73},
  {54,62,42,33,13,9},
  {53,61,41,32,12,8},
  {60,72,40,31,23,7},
  {59,71,38,30,22,6},
  {58,70,37,29,21,5},
  {57,69,39,28,20,4},
  {52,68,48,27,19,3},
  {51,67,47,26,18,2},
  {50,66,46,25,17,1},
  {49,65,45,24,16,0}
};

int pixelstate[12][6] = { //LED PWM values
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
};

  int x;
  int y;
  
  
// Sensor Stuff  
  int lvldoor;
  int lvlwall;
  
// Pseudo-Radnom Stuff  
  int dice;

// Mode Stuff  
boolean modeflag = 0;

// Pong Stuff  
boolean xdir=0;
boolean ydir=0;
boolean movecycle=1;

// Spectrum Analyzer Stuff  
int Spectrum[7]; 
int spectrumReset=5;
int spectrumStrobe=6;
int spectrumAnalog=3;
int scaleFactor = 15;


void setup()
{
  
  Tlc.init();
  
  pinMode(spectrumReset, OUTPUT);
  pinMode(spectrumStrobe, OUTPUT);
  
    //Init spectrum analyzer
  digitalWrite(spectrumStrobe,LOW);
    delay(1);
  digitalWrite(spectrumReset,HIGH);
    delay(1);
  digitalWrite(spectrumStrobe,HIGH);
    delay(1);
  digitalWrite(spectrumStrobe,LOW);
    delay(1);
  digitalWrite(spectrumReset,LOW);
    delay(5);
    
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
    digitalWrite(A6, HIGH);    
    digitalWrite(A7, HIGH);    

}

void loop()
{
  
  //Mode Selecta
  if(digitalRead(A6)==LOW){modeflag=0;};
  if(digitalRead(A7)==LOW){modeflag=1;};
  
if(modeflag==0){
  readSpectrum();
  
for (int y=0; y<6; y++){
  for(int x=(Spectrum[y]/scaleFactor); x>=0; x--){
  Tlc.set(pixel[x][y], 4000);}};

Tlc.update();
delay(50);
Tlc.clear();}

if(modeflag==1){
  
    !movecycle;
    
//reach into meatspace  
  lvldoor = map(analogRead(A0), 40, 200, 2, 0);
  lvldoor = constrain(lvldoor,0,2);
  lvlwall = map(analogRead(A1), 30, 200, 2, 0);
  lvlwall = constrain(lvlwall,0,2);

//move the ball
if(movecycle){
  if(xdir==0 && x<10){x++;}
  if(xdir==1 && x>=1){x--;}
  if(xdir==0 && x==10){xdir=1;};
  if(xdir==1 && x==1){xdir=0;};

  if(ydir==0 && y<5){y++;}
  if(ydir==1 && y>=0){y--;}
  if(ydir==0 && y==5){ydir=1;};  
  if(ydir==1 && y==0){ydir=0;};}

//light the ball  
  pixelstate[x][y]=4000;

//check if lose  
  if(x==10 && lvldoor*2!=y && (lvldoor*2)+1!=y){
    loseCondition();
  }

//fade the field (tail on the ball)
  for (int y=0; y<6; y++){
  for(int x=1; x<11; x++){
  if(pixelstate[x][y]>0){
  pixelstate[x][y]=pixelstate[x][y]-1000;}
}
}

//ballfield update
for (int y=0; y<6; y++){
  for(int x=0; x<12; x++){
  Tlc.set(pixel[x][y], pixelstate[x][y]);}
};

//Paddles Cleanup
for(int i=0; i<6; i++){
 Tlc.set(pixel[0][i], 0);  
 Tlc.set(pixel[11][i], 0);  

}

//Paddles update
  Tlc.set(pixel[11][lvldoor*2], 4095);  Tlc.set(pixel[11][(lvldoor*2)+1], 4095);

  Tlc.update();

delay(250);
  
}
  
  
  
  
  
  
}

// Read 7 band equalizer.
void readSpectrum()
{
  // Band 0 = Lowest Frequencies.
  byte Band;
  for(Band=0;Band <7; Band++)
  {
    Spectrum[Band] = (analogRead(spectrumAnalog) + analogRead(spectrumAnalog) ) >>1; //Read twice and take the average by dividing by 2
    digitalWrite(spectrumStrobe,HIGH);
    digitalWrite(spectrumStrobe,LOW);     
  }
}

void loseCondition(){
   
  for (int iterate=0; iterate<50; iterate++){

    for (int channel = 0; channel < NUM_TLCS * 16; channel ++) {
    dice = random(0,50);
    Tlc.set(channel, dice*80);
    Tlc.update();
    }
delay(75);
    
}; 
  
}

