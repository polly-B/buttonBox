#include <Button.h>

const int button1Pin = 7;     // the number of the pushbutton pin
Button button1 = Button(button1Pin, PULLUP);

const int button2Pin = 8;     // the number of the pushbutton pin
Button button2 = Button(button2Pin, PULLUP);

const int button3Pin = 12;     // the number of the pushbutton pin
Button button3 = Button(button3Pin, PULLUP);

const int button4Pin = 13;     // the number of the pushbutton pin
Button button4 = Button(button4Pin, PULLUP);

unsigned long trialTime = 0;
unsigned long trialStart = 0;

String triggered = "Not Triggered";

int temt6000Read = 0;
int temt6000Pin = A0;
int temt6000Value = 0;
long temt6000Time = 0;

long runningTime = 0; //Time from photocell trigger
int timeOut = 3000; //max runningTime

int noTriggerTime = 0; //time before photocell was triggered
int startTime = 0; //time that trigger and response loop was started

int buttonState;             // the current reading from the input pin
long buttonTime = 0;         // the last time the output pin was toggled
long responseTime = 0;

int inByte = 0;         // incoming serial byte
int pressed = 1;
int key = 0;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
     ; // wait for serial port to connect. Needed for Leonardo only
   }

  establishContact();  // send a byte to establish contact until receiver responds 
}

void loop(){
  if(Serial.available() > 0){
    inByte = Serial.read();
    if (inByte == 'b'){
      pressed = 1;
      startTime = millis();
      noTriggerTime = 0;
      while (analogRead(temt6000Pin) <= 100 && noTriggerTime<timeOut){
        noTriggerTime = millis() - startTime;
      }
      if (noTriggerTime < timeOut){
        temt6000Time = millis();
        do
        {
          runningTime = millis() - temt6000Time;
          if (button1.isPressed()){
            buttonTime = millis();
            key = 1;
            pressed = 0;
          }
          if (button2.isPressed()){
            buttonTime = millis();
            key = 2;
            pressed = 0;
          }
          if (button3.isPressed()){
            buttonTime = millis();
            key = 3;
            pressed = 0;
          }
          if (button4.isPressed()){
            buttonTime = millis();
            key = 4;
            pressed = 0;
          }
          
        }
        while (pressed && runningTime <= timeOut);
  
        if (runningTime>timeOut) {
          Serial.println("Timed Out");
          pressed = 0;
        }
        else {
          responseTime = buttonTime - temt6000Time;
          Serial.print(responseTime);
          Serial.print("\t");
          Serial.println(key);    
        }  
      }
      else {
        Serial.println("No Trigger");
      }
    } 
    else if (inByte == 'c'){
      trialTime = Serial.parseInt();
      trialStart = millis();
      int temt6000Min = 1024;
      int temt6000Max = 0;
  //    now = millis();
      while (millis() - trialStart < trialTime){
        temt6000Value = analogRead(temt6000Pin);
        if (temt6000Value > temt6000Max){
          temt6000Max = temt6000Value;
        }
        if (temt6000Value < temt6000Min){
          temt6000Min = temt6000Value;
        }
//      now = millis();
        delay(100);
      }
      Serial.print("temt6000Max  ");
      Serial.println(temt6000Max);
      Serial.print("temt6000Min  ");
      Serial.println(temt6000Min);
    }
    else if (inByte=='p'){
      Serial.println("waiting for input");
      }
    else if (inByte=='d'){
      temt6000Time = millis();
      runningTime = 0;
      while (runningTime<=timeOut){
        runningTime = millis() - temt6000Time;
        if(analogRead(temt6000Pin) > 150){
          triggered = "Triggered";
        }
      }
      Serial.println(triggered);
      triggered = "Not Triggered";
    }
  }
}

void establishContact() {
  if (Serial.available() <= 0) {
    Serial.println("Initialised");
  }
}


