/*
 *
 */
 
int ledPin = 13;                // choose the pin for the LED
// pin at first fret = 6
int greenPin = 5;
int redPin = 7;
int yellowPin = 4;
int bluePin = 3;
int orangePin = 2;
int strumPin = 0;

int greenOut = 8;
int redOut = 9;
int yellowOut = 10;
int blueOut = 12;
int orangeOut = 13;
int startOut = 0;
int strumUpOut = 0;
int strumDownOut = 1;

int firstString = 19;
int secondString = 18;
int thirdString = 17;
int fourthString = 16;
int fifthString = 15;
int sixthString = 14;

int green = 0;                    // variable for reading the pin status
int red = 0;
int yellow = 0;
int blue = 0;
int orange = 0;
int strum = 0;
int state = 5;
int greenOn = 0;
int redOn = 0;
int yellowOn = 0;
int blueOn = 0;
int orangeOn = 0;
int strumOn = 0;

int greenOnTest = 0;
int redOnTest = 0;
int yellowOnTest = 0;
int blueOnTest = 0;
int orangeOnTest = 0;
int strumOnTest = 0;

int greenTimerWait = 20;
int greenTimers[6] = {0,0,0,0,0,0}; //These give timing to properly read green buttons on each string.
int greenTimers2[6] = {0,0,0,0,0,0};
boolean triedGreen[6] = {false, false, false, false, false, false};

#define SET(Byte,Bit) (Byte |= (1<<Bit)) // this takes a byte and turns the identified bit HIGH
#define CLR(Byte,Bit) (Byte &= (~(1<<Bit)))

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  
  pinMode(greenPin, INPUT);     // declare the fret inputs
  pinMode(redPin, INPUT);
  pinMode(yellowPin, INPUT);
  pinMode(bluePin, INPUT);
  pinMode(orangePin, INPUT);
  pinMode(strumPin, INPUT);
  
  digitalWrite(greenPin, HIGH);  // set the pull-up resistors on the fret pins
  digitalWrite(redPin, HIGH);
  digitalWrite(yellowPin, HIGH);
  digitalWrite(bluePin, HIGH);
  digitalWrite(orangePin, HIGH);
  
  
  pinMode(greenOut, OUTPUT);  // set the output pins
  pinMode(redOut, OUTPUT);
  pinMode(yellowOut, OUTPUT);
  pinMode(blueOut, OUTPUT);
  pinMode(orangeOut, OUTPUT);
  digitalWrite(greenOut, HIGH);  // Set these pins to start HIGH
  digitalWrite(redOut, HIGH);    // since the controller looks for
  digitalWrite(yellowOut, HIGH); // grounded pins
  digitalWrite(blueOut, HIGH);
  digitalWrite(orangeOut, HIGH);
  
  pinMode(firstString, OUTPUT);   // The string pins provide current to
  pinMode(secondString, OUTPUT);  // the fret pins
  pinMode(thirdString, OUTPUT);
  pinMode(fourthString, OUTPUT);
  pinMode(fifthString, OUTPUT);
  pinMode(sixthString, OUTPUT);

  digitalWrite(firstString, HIGH);  // set the strings to HIGH to start with
  digitalWrite(secondString, HIGH);
  digitalWrite(thirdString, HIGH);
  digitalWrite(fourthString, HIGH);
  digitalWrite(fifthString, HIGH);
  digitalWrite(sixthString, HIGH);
}

void loop()
{
  readF(firstString);
  readF(secondString);
  readF(thirdString);
  readF(fourthString);
  readF(fifthString);
  readF(sixthString);
  sendButtons();
}

void readF(int guitarString)
{ 
  digitalWrite(guitarString, LOW);
 
  green = digitalRead(greenPin);
  red = digitalRead(redPin);
  yellow = digitalRead(yellowPin);
  blue = digitalRead(bluePin);
  orange = digitalRead(orangePin);// read input value
  strum = digitalRead(strumPin);
 
  if (orange == LOW && blue == LOW  && orangeOn == 0)
  {    
    orangeOn = guitarString;
    triedGreen[guitarString] = false;
  }
  else if (blue == LOW && yellow == LOW && blueOn == 0)
  {
    blueOn = guitarString;
    triedGreen[guitarString] = false;
  }
  else if (yellow == LOW && red == LOW && yellowOn == 0)
  {
    yellowOn = guitarString;
    triedGreen[guitarString] = false;
  }
  else if (red == LOW && green == LOW && redOn == 0)
  {
    redOn = guitarString;
    triedGreen[guitarString] = false;
  }
  else if (green == LOW)
  {
    if (triedGreen[guitarString] == false)
    {
      greenTimers[guitarString] = millis();
      triedGreen[guitarString] = true;
    }
    else //triedGreen == true
    {
      if (millis() - greenTimers[guitarString] > greenTimerWait
          && greenOn == 0)
      {
        greenOn = guitarString;
      }
    }
  }
 
  else  //no buttons pressed
  {
    triedGreen[guitarString] = false;
  }

 /* if (strum == LOW)
  {
    strumOn = guitarString;
  }*/
 
  digitalWrite(guitarString, HIGH);
} //end of readF(guitarString)

void sendButtons()
{
  boolean lightswitch = false;
  
  if (orangeOn != 0)
  {
    digitalWrite(ledPin, HIGH); // turn LED ON
    digitalWrite(orangeOut, LOW);
    lightswitch = true;
  }
  else
  {
    digitalWrite(orangeOut, HIGH);
  }
  if (blueOn != 0)
  {
    digitalWrite(ledPin, HIGH); // turn LED ON
    digitalWrite(blueOut, LOW);
    lightswitch = true;
  }
  else
  {
    digitalWrite(blueOut, HIGH);
  }
  if (yellowOn != 0)
  {
    digitalWrite(ledPin, HIGH); // turn LED ON
    digitalWrite(yellowOut, LOW);
    lightswitch = true;
  }else
  {
    digitalWrite(yellowOut, HIGH);
  }
  if (redOn != 0)
  {
    digitalWrite(ledPin, HIGH); // turn LED ON
    digitalWrite(redOut, LOW);
    lightswitch = true;
  }
  else
  {
    digitalWrite(redOut, HIGH);
  }
  if (greenOn != 0)
  {
    digitalWrite(ledPin, HIGH); // turn LED ON
    digitalWrite(greenOut, LOW);
    lightswitch = true;
  }
  else
  {
    digitalWrite(greenOut, HIGH);
  }
  
  if (strumOn != 0)
  {
    lightswitch = true;
    if (strumOn <= thirdString)
    {
      digitalWrite(ledPin, HIGH);
      digitalWrite(strumDownOut, LOW);
      digitalWrite(strumUpOut, HIGH);
    }
    else
    {
      digitalWrite(ledPin, HIGH);
      digitalWrite(strumUpOut, LOW);
      digitalWrite(strumDownOut, HIGH);
    }
  }
  else
  {
    digitalWrite(strumUpOut, LOW);
    digitalWrite(strumDownOut, LOW);
  }
  if (lightswitch == true) digitalWrite(ledPin, LOW);
 
 
  if(greenOn != greenOnTest || blueOn != blueOnTest || yellowOn !=yellowOnTest
          || redOn != redOnTest || orangeOn != orangeOnTest || strumOn != strumOnTest)
  {
  Serial.print("orange ");
  Serial.print(orangeOn);
  Serial.print(", blue ");
  Serial.print(blueOn);
  Serial.print(", yellow ");
  Serial.print(yellowOn);
  Serial.print(", red");
  Serial.print(redOn);
  Serial.print(", green");
  Serial.print(greenOn);
  Serial.print(", Strum");
  Serial.println(strumOn);
  //Serial.println(dataPacket[5], HEX);
  }
  
 
  greenOnTest = greenOn;
  redOnTest = redOn;
  yellowOnTest = yellowOn;
  blueOnTest = blueOn;
  orangeOnTest = orangeOn;
  strumOnTest = strumOn;
  
  
  
  setAllFalse();
} //end of sendButtons


void setAllFalse()
{
  greenOn = 0;
  redOn = 0;
  yellowOn = 0;
  blueOn = 0;
  orangeOn = 0;
  strumOn = 0;
}
