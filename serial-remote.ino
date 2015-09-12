/* 
 * Basic motor controls for 2-motor bot using pwm.  
*/

//Set pins
const int pwmLPin =  23;
const int pwmRPin = 22;
const int forwardRPin = 14;
const int reverseRPin = 13;
const int forwardLPin = 15;
const int reverseLPin = 16;
const int minPwm = 128;

int pwrLeft = minPwm;
int pwrRight = minPwm;

#define HWSERIAL Serial1

int loopCounter = 0;

void setup()   
{
  //Initial pin setup                
  pinMode(pwmLPin, OUTPUT);
  pinMode(pwmRPin, OUTPUT);
  pinMode(forwardRPin, OUTPUT);
  pinMode(reverseRPin, OUTPUT);
  pinMode(forwardLPin, OUTPUT);
  pinMode(reverseLPin, OUTPUT);

  Serial.begin(9600);
  HWSERIAL.begin(38400);
}

void bump()
{
    analogWrite(pwmLPin, 256);
    analogWrite(pwmRPin, 256);
    delay(250);
}

//Move forward at given rate (n/255 duty cycle)
void moveForward(int duty, int rampTimeMS)
{
  //Set both wheels forward direction
  digitalWrite(forwardRPin, HIGH);
  digitalWrite(reverseRPin, LOW);
  digitalWrite(forwardLPin, HIGH);
  digitalWrite(reverseLPin, LOW);

  bump();
  //Slow ramp up
  for(int i=0; i<100; i++)
  {
      analogWrite(pwmLPin, max(minPwm, i*duty/100.0));
      pwrLeft = max(minPwm, i*duty/100.0);
      analogWrite(pwmRPin, max(minPwm, i*duty/100.0));
      pwrRight = max(minPwm, i*duty/100.0);
      delay(rampTimeMS/100.0);
  }  
}

//Move backward at given rate (n/255 duty cycle)
void moveBackward(int duty, int rampTimeMS)
{
  //Set both wheels reverse direction
  digitalWrite(forwardRPin, LOW);
  digitalWrite(reverseRPin, HIGH);
  digitalWrite(forwardLPin, LOW);
  digitalWrite(reverseLPin, HIGH);

  bump();
  //Slow ramp up
  for(int i=0; i<100; i++)
  {
      analogWrite(pwmLPin, max(minPwm, i*duty/100.0));
      pwrLeft = max(minPwm, i*duty/100.0);
      analogWrite(pwmRPin, max(minPwm, i*duty/100.0));
      pwrRight = max(minPwm, i*duty/100.0);
      delay(rampTimeMS/100.0);
  }  
}

//Use a 1s accel ramp to avoid wheel spin
void rightTurn(int duty, int rampTimeMS)
{
  //Right wheel backward
  digitalWrite(forwardRPin, LOW);
  digitalWrite(reverseRPin, HIGH);
  //Left wheel forward
  digitalWrite(forwardLPin, HIGH);
  digitalWrite(reverseLPin, LOW);

  bump();
  //Slow ramp up
  for(int i=0; i<100; i++)
  {
      analogWrite(pwmLPin, max(minPwm, i*duty/100.0));
      pwrLeft = max(minPwm, i*duty/100.0);
      analogWrite(pwmRPin, max(minPwm, i*duty/100.0));
      pwrRight = max(minPwm, i*duty/100.0);
      delay(rampTimeMS/100.0);
  }  
}

void leftTurn(int duty, int rampTimeMS)
{
  //Right wheel forward
  digitalWrite(forwardRPin, HIGH);
  digitalWrite(reverseRPin, LOW);
  //Left wheel backward
  digitalWrite(forwardLPin, LOW);
  digitalWrite(reverseLPin, HIGH);

  bump();
  //Slow ramp up
  for(int i=0; i<100; i++)
  {
      analogWrite(pwmLPin, max(minPwm, i*duty/100.0));
      pwrLeft = max(minPwm, i*duty/100.0);
      analogWrite(pwmRPin, max(minPwm, i*duty/100.0));
      pwrRight = max(minPwm, i*duty/100.0);
      delay(rampTimeMS/100.0);
  }  
}

void rightCircle(int duty)
{
    analogWrite(pwmLPin, 255);
    pwrLeft = 255;
    analogWrite(pwmRPin, 200);
    pwrRight = 200;
}

void incLeft(int increment)
{
    pwrRight += increment;

    if(pwrRight > 0)
    {
      if(pwrRight < 110)
        pwrRight = 110;
      Serial.println("Positive right!");      
      digitalWrite(forwardRPin, HIGH);
      digitalWrite(reverseRPin, LOW);      
      analogWrite(pwmRPin, pwrRight);
    }
    else
    {
      if(pwrRight > -110)
        pwrRight = -110;
      Serial.println("Negative right!");
      digitalWrite(forwardRPin, LOW);
      digitalWrite(reverseRPin, HIGH);
      analogWrite(pwmRPin, -pwrRight); 
    }
}

void incRight(int increment)
{
  pwrLeft += increment;
  if(pwrLeft > 0)
  {
    if(pwrLeft < 110)
      pwrLeft = 110;
      
    Serial.println("Positive left!");
    digitalWrite(forwardLPin, HIGH);
    digitalWrite(reverseLPin, LOW);    
    analogWrite(pwmLPin, pwrLeft);
  }
  else
  {
    if(pwrLeft > -110)
      pwrLeft = -110;
    //move backwards
    Serial.println("Negative left!");
    digitalWrite(forwardLPin, LOW);
    digitalWrite(reverseLPin, HIGH);   
    analogWrite(pwmLPin, -pwrLeft);
  }
    
}

void allStop()
{
  analogWrite(pwmLPin, 0);
  analogWrite(pwmRPin, 0);

  pwrRight = 0;
  pwrLeft = 0;
}

void loop()                     
{
  int incomingByte = 0;

  loopCounter++;
  
  if (Serial.available() > 0) 
  {
    loopCounter = 0;
    incomingByte = Serial.read();
    Serial.print("USB received: ");
    Serial.println(incomingByte, DEC);
                HWSERIAL.print("USB received:");
                HWSERIAL.println(incomingByte, DEC);
  }
  else if (HWSERIAL.available() > 0)
  {
    loopCounter = 0;
    incomingByte = HWSERIAL.read();
    Serial.print("UART received: ");
    Serial.println(incomingByte, DEC);
                HWSERIAL.print("UART received:");
                HWSERIAL.println(incomingByte, DEC);
  }

  if(loopCounter == 1000000)
  {
    //if(loopCounter == 10000)
    Serial.println("Stopping due to no input");
    incomingByte = 's';
  }

  switch(incomingByte)
  {
    case 's':
      Serial.println("All stop aye!");
      allStop();
      break;
    case 'w':
      Serial.println("Forward aye!");
      moveForward(200, 0);
      break;
    case 'a':
      Serial.println("Left turn aye!");
      leftTurn(255, 0);
      break;
    case 'd':
      Serial.println("Right turn aye!");
      rightTurn(255, 0);
      break;
    case 'x':
      Serial.println("Reverse aye!");
      moveBackward(200, 0);
      break;
    case 'u':
     Serial.println("Forward left!");
     incLeft(20);
     break;
    case 'i':
     Serial.println("Forward right!");
     incRight(20);
     break;
   case 'j':
     Serial.println("Reverse left!");
     incLeft(-10);
     break;     
   case 'k':
     Serial.println("Reverse right!");
     incRight(-10);
     break;    
    default:
     break;
  }
  

  
  /*
  moveForward(200, 250);
  delay(5000);
  allStop();
  delay(1000);

  moveBackward(255, 250);
  delay(1000);
  allStop();
  delay(1000);

  rightTurn(255, 250);
  delay(1000);
  allStop();
  delay(1000);


  moveForward(200, 250);
  delay(5000);
  allStop();
  delay(1000);

  moveBackward(255, 250);
  delay(1000);
  allStop();
  delay(500);


  leftTurn(255, 250);
  delay(1000);ug
  allStop();
  delay(500);
  */
}
