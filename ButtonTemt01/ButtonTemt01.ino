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
unsigned long trialSample = 0;
// unsigned long now;

int temt6000Read = 0;
int temt6000Pin = 0;
int temt6000Value = 0;
long temt6000Time = 0;

int potPin = A1;
int potValue = 0;
long potTime = 0;


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
      while (analogRead(temt6000Pin) <= 100){
      }

      temt6000Time = millis();

      do
      {
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
      while (pressed);

      responseTime = buttonTime - temt6000Time;
      Serial.print(responseTime);
      Serial.print("\t");
      Serial.println(key);      
    }
    
  else if (inByte == 'p'){
    trialTime = Serial.parseInt();
    trialSample = Serial.parseInt();
    trialStart = millis();
    potTime = millis();
    temt6000Read = 0;
    pressed = 1;
    while (millis() - trialStart <= trialTime) {
      if (millis() - potTime >= trialSample){
        potValue = analogRead(potPin);
        potTime = millis();
        Serial.print(millis() - trialStart);
        Serial.print("  pot  ");
        Serial.println(potValue);
      }
      if (analogRead(temt6000Pin) >= 100 && temt6000Read == 0){
        Serial.print(millis() - trialStart);
        Serial.println("  temt6000");
        temt6000Time = millis();
        temt6000Read = 1;
      }
      if (button1.isPressed() && pressed == 1){
        Serial.print(millis() - trialStart);
        Serial.println("  button1");
        pressed = 0;
      }
      if (button2.isPressed() && pressed == 1){
        Serial.print(millis() - trialStart);
        Serial.println("  button2");
        pressed = 0;
      }
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
}
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println('a');
    delay(300);
  }
}


