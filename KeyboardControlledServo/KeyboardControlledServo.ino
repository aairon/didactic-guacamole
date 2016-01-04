

/**
   Use values 35 - 80 as a range for a servos together
*/

#include <Servo.h>

int x;
String str;

int pos = 0;
/*              b   s    e    c     */
int rest[4] = {105, 50,  50,  90};
int mmin[4] = {  0, 35,   5,   0};
int mmax[4] = {180, 80, 120, 105};
int ddelay = 2;
int newPos[4] = {105, 50,  50,  90};
int currentPos[4] = {105, 50,  50,  90};
int inc[4] = { 1, 1, 1, 1};
int ByteReceived;

Servo myServo[4];
Servo testServo;

int servoPin[4] = { 8, 9, 10, 11 };

void setup() {
  Serial.begin(115200);

  for (int t = 0 ; t <= 3; t += 1) {
    myServo[t].attach(servoPin[t]);
  }

  for (int t = 0 ; t <= 3; t += 1) {
    myServo[t].write(rest[t]);
  }

  randomSeed(analogRead(0));


}

void loop()
{

  if (Serial.available() > 0)
  {

    ByteReceived = Serial.read();
    Serial.print(ByteReceived);
    Serial.print("        ");
    Serial.print(ByteReceived, HEX);
    Serial.print("       ");
    Serial.print(char(ByteReceived));

    if (ByteReceived == 'a') // Single Quote! This is a character.
    {
      Serial.print(" LED ON ");
      newPos[0] = newPos[0] + 5;

    }

    if (ByteReceived == 'd')
    {
      Serial.print(" LED OFF");
      newPos[0] = newPos[0] - 5;
    }


    if (ByteReceived == 'w')
    {
      Serial.print(" Shoulder");
      newPos[1] = newPos[1] + 5;
    }

    if (ByteReceived == 's')
    {
      Serial.print(" Shoulder");
      newPos[1] = newPos[1] - 5;
    }


    if (ByteReceived == 'h')
    {

      newPos[0] =  105;
      newPos[1] =   50;
      newPos[2] =   50;
      newPos[3] =   50;
      SweepTo() ;
      Serial.print(" HOME ");
    }

    SweepTo() ;
    Serial.println();    // End the line

  }

}

void SweepTo()
{

  Serial.println("Top of Move To");
  int complete[4] = {0, 0, 0, 0};


  for (int t = 0; t <= 180; t += 1) {

    if (complete[0] == 1 &&
        complete[1] == 1 &&
        complete[2] == 1 &&
        complete[3] == 1) break;

    Serial.println();

    for (int servoNumber = 0; servoNumber <= 3; servoNumber += 1) {
      /*
          Which way are we moving
      */
      if (newPos[servoNumber] <= currentPos[servoNumber])
        inc[servoNumber] = -1;
      else inc[servoNumber] = 1;

      if (currentPos[servoNumber] != newPos[servoNumber] &&
          currentPos[servoNumber] > mmin[servoNumber] &&
          currentPos[servoNumber] < mmax[servoNumber])
        currentPos[servoNumber] = currentPos[servoNumber] + inc[servoNumber];
      else {

        Serial.print(" Completed   ");
        Serial.print(servoNumber);


        complete[servoNumber] = 1;
        /*
           set to limits
        */
        if (currentPos[servoNumber] <= mmin[servoNumber]) {
          currentPos[servoNumber] == mmin[servoNumber] + 1;
          newPos[servoNumber] == mmin[servoNumber] + 1;
        }

        if (currentPos[servoNumber] >= mmax[servoNumber]) {
          currentPos[servoNumber] == mmax[servoNumber] - 1;
          newPos[servoNumber] == mmax[servoNumber] - 1  ;
        }
      }

      /*
          Check limits
      */


      // Serial.println(t);
      Serial.print(" Bottom    ");
      Serial.print( t );
      Serial.print(" Servo:");
      Serial.print(servoNumber);
      Serial.print( " Curr:" );
      Serial.print(currentPos[servoNumber]);
      Serial.print( " New: " );
      Serial.print(newPos[servoNumber]);
      Serial.println();

      myServo[servoNumber].write(currentPos[servoNumber] );
      delay(ddelay);

    }
  }
}

