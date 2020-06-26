#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dacY; //up and down movements
Adafruit_MCP4725 dacX; //left and right movements

//init volatages for joysticks
const int highX = 4000;      //high end voltage
const int highY = 3900;      //high end voltage
const int midX = (highX / 2);      //middle volatage
const int midY = (highY / 2);      //high end voltage
const int low = 0;           //low voltage

//initializing value for sensor
int sensorValue;
int LDRvalue;

//define input vals
const byte dacXin = A1;
const byte dacYin = A2;
const byte LDR = A3;

//defining relay pins
const int relay1 = 9;
const int relay2 = 10;
const int relay3 = 11;
const int relay4 = 12;

//defining chars used for inputs
const char Z = 'Z';
const char X = 'X';
//const char P = 'P';
const char L = 'L';
const char N = 'N';
const char S = 'S';
const char E = 'E';
const char W = 'W';
const char Y = 'Y';

//time keeping
const long eggTime = 250;
unsigned long previousTimeE = 0;
unsigned long previousTimeW = 0;
unsigned long previousTimeN = 0;
unsigned long previousTimeS = 0;
unsigned long previousEggTime = 0;
unsigned long currentTime = 0;

boolean egg = false;
boolean firstTick = true;


void setup(void) {

  TWBR = 12; // 400 khz
  Serial.begin(9600);

  //init relays
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);

  //init dac
  dacY.begin(0x60);
  dacX.begin(0x61);
  move(Z);

  Serial.print("Setting up: ");
  for (int i = 10; i > 0; i--) {
    Serial.print(i);
    delay(1000);
  }
  Serial.print("\n");

}

void loop() {
  if(firstTick) {
    travel(N, 200);
    travel(E, 200);
    firstTick = false;
  }
  travel(S, 500); //Serial.println(analogRead(LDR));
  travel(W, 500); //Serial.println(analogRead(LDR));
  travel(N, 500); //Serial.println(analogRead(LDR));
  travel(E, 500); //Serial.println(analogRead(LDR));

}

void travel(char d, long t) {
  boolean trigger = true;
  currentTime = millis();
  switch (d) {
    case N:
      move(d);
      previousTimeN = currentTime;
      while (trigger) {
        currentTime = millis();
        checkEgg(d);
        if (currentTime - previousTimeN >= t) {
          move(Y);
          trigger = false;
        }
      }
      break;
    case S:
      move(d);
      previousTimeS = currentTime;
      while (trigger) {
        currentTime = millis();
        checkEgg(d);
        if (currentTime - previousTimeS >= t) {
          move(Y);
          trigger = false;
        }
      }
      break;
    case E:
      move(d);
      previousTimeE = currentTime;
      while (trigger) {
        currentTime = millis();
        checkEgg(d);
        if (currentTime - previousTimeE >= t) {
          move(X);
          trigger = false;
        }
      }
      break;
    case W:
      move(d);
      previousTimeW = currentTime;
      while (trigger) {
        currentTime = millis();
        checkEgg(d);
        if (currentTime - previousTimeW >= t) {
          move(X);
          trigger = false;
        }
      }
      break;
  }

}

void checkEgg(char d) {
  LDRvalue = analogRead(LDR);
  if (LDRvalue < 80 && LDRvalue > 68) {
    Serial.print(LDRvalue);
    Serial.print(" <- LDR READING\n");
    currentTime = millis();
    if (egg) { //if its not the first tick
      Serial.print("IFEGG");
      if (currentTime - previousEggTime >= eggTime) { //if the LDR reads correct val for certain time
        Serial.print("Egg detected, entering egg phase\n");
        eggPhase();
        egg = false; //restart egg timer flag
      }
    } else { //if its the first tick
      egg = true; //make it so it doesnt enter here until LDR reads no egg
      previousEggTime = currentTime;  //start timer to see if egg or false reading
      Serial.print("ELSEEGG"); //gets stuck here sometime for some reason?      
    }
  } else {  //if no egg reading present
    egg = false;
  }

}

void eggPhase() {
  /* Steps:
    - "Oh?" press Z to continue
    - *wait 12 seconds for egg to hatch and pokemon to show* press Z to continue
    - *wait 3 seconds for dialog to appear about naming pokemon* press B to select "no"
      - naming is disabled so ignore ^

    - egg counter ++
    - return to egg npc or continue hatching eggs
  */

  press(Z);
  unsigned long hatchTimer = millis(); //starts timer
  delay(8000); //waits until hatch animation starts
  LDRvalue = analogRead(LDR);
  while (LDRvalue > 190) {  //waits until LDR < 190 meaning next dialog box
    LDRvalue = analogRead(LDR);
    delay(10);
  }
  currentTime = millis();
  hatchTimer = currentTime - hatchTimer; //time since timer started
  Serial.print(hatchTimer);
  Serial.print(" <- Time taken to hatch\n");
  if (hatchTimer > 14900) {
    //shiny hatch
    Serial.print("SHINY HATCH!");
    press(X);
    move(E);
    delay(800);
    press(Z);
    delay(500);
    press(Z);
    delay(9999999999);
  }
  delay(1000);
  press(Z);   //advance out of egg hatching sequence
  delay(3000);
  travel(N, 200);
  move(N);
  travel(E, 2000);
  move(Y);
  //eggCounter++;
}


void press(char d) {
  int r;
  switch (d) {
    case Z:
      r = relay1;
      break;
    case X:
      r = relay2;
      break;
    case L:
      r = relay3;
      break;
    case S:
      r = relay4;
      break;
  }
  Serial.print(d + "\n");
  digitalWrite(r, LOW);
  delay(100);
  digitalWrite(r, HIGH);
}

void move(char d) {

  switch (d) {
    case 'N':
      dacY.setVoltage(highY, false);   //set voltage
      sensorValue = analogRead(dacYin);   //print voltage reading & direction
      Serial.print(sensorValue);
      Serial.print(" - Up\n");
      break;
    case 'S':
      dacY.setVoltage(low, false);
      sensorValue = analogRead(dacYin);
      Serial.print(sensorValue);
      Serial.print(" - Down\n");
      break;
    case 'E':
      dacX.setVoltage(highX, false);
      sensorValue = analogRead(dacXin);
      Serial.print(sensorValue);
      Serial.print(" - Right\n");
      break;
    case 'W':
      dacX.setVoltage(low, false);
      sensorValue = analogRead(dacXin);
      Serial.print(sensorValue);
      Serial.print(" - Left\n");
      break;
    case 'Y':
      dacY.setVoltage(midY, false);
      sensorValue = analogRead(dacYin);
      Serial.print(sensorValue);
      Serial.print(" - Y axis steady\n");
      break;
    case 'X':
      dacX.setVoltage(midX, false);
      sensorValue = analogRead(dacXin);
      Serial.print(sensorValue);
      Serial.print(" - X axis steady\n");
      break;
  }

}
