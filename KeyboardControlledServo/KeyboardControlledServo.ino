

/**
   Use values 35 - 80 as a range for a servos together
*/

#include <Servo.h>

int x;
String str;

int pos = 0;
/*              b   s    e    c     */
int rest[4] = {105, 50,  50,  90};
int mmin[4] = { 38, 25,   5,   0};
int mmax[4] = {172, 90, 120, 105};
int ddelay = 3;
int newPos[4] = {105, 50,  50,  90};
int currentPos[4] = {105, 50,  50,  90};
int inc[4] = { 1, 1, 1, 1};
int ByteReceived;
int sstep[4] = { 1, 1, 1, 20 } ;
String memory[100];
int ttt = 0;


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


    if (ByteReceived == 'R') // Single Quote! This is a character.
    {
      int randNumber[3];

      for (int t = 0 ; t <= 3; t += 1) {
        randNumber[t] = random(mmin[t], mmax[t]);
        newPos[t] = randNumber[t];
        ddelay = random(1, 5);
        SweepTo() ;
      }
    }


    if (ByteReceived == 'r') // Single Quote! This is a character.
    {
      Serial.println();
      Serial.println();
      Serial.print(" Remember ");

      printNewPos();

      String thisString =
        String(currentPos[0], DEC) + "," +
        String(currentPos[1], DEC) + "," +
        String(currentPos[2], DEC) + "," +
        String(currentPos[3], DEC) ;

      memory[ttt] = thisString ;
      for (int tttt = 0 ; tttt <= ttt ; tttt += 1) {
        Serial.println(memory[tttt]);
      }

      /*
         Don't remember
      */
      ttt = ttt + 1;

    }

    if (ByteReceived == 'p') // Single Quote! This is a character.
    {
      Serial.print(" Play Back Base");

      for (int tttt = 0 ; tttt <= ttt ; tttt += 1) {

        Serial.println(memory[tttt]);
      }
      for (int tttt = 0 ; tttt < ttt ; tttt += 1) {
        Serial.println(memory[tttt]);


        pullPos(memory[tttt]);

        Serial.print("mem");
        Serial.print(newPos[0]);
        Serial.print(" mmm ");
        Serial.print(newPos[1]);
        Serial.print(" mmm ");
        Serial.print(newPos[2]);
        Serial.print(" mmm ");
        Serial.print(newPos[3]);
        Serial.print(" mmm ");

        Serial.println();

        if (memory[tttt] != memory[tttt - 1])
          SweepTo();

      }

      for (int tttt = 0 ; tttt <= ttt ; tttt += 1) {

        Serial.println(memory[tttt]);
      }

    }

    if (ByteReceived == 'D') // Single Quote! This is a character.
    {
      Serial.print(" Base +  ");
      newPos[0] = newPos[0] - sstep[0];

    }

    if (ByteReceived == 't')
    {
      Serial.print(" Tight ");
      newPos[1] = 29;
      newPos[2] = 34;
    }

    if (ByteReceived == 'u')
    {
      Serial.print(" High ");
      newPos[1] = 42;
      newPos[2] = 109;
    }

    if (ByteReceived == 'y')
    {
      Serial.print(" Reach tOut ");
      newPos[1] = 88;
      newPos[2] = 49;
    }

    if (ByteReceived == 'C')
    {
      Serial.print(" Base + ");
      newPos[0] = newPos[0] + sstep[0];
    }

    if (ByteReceived == 'w')
    {
      Serial.print(" Shoulder + ");
      newPos[1] = newPos[1] - sstep[1];
    }

    if (ByteReceived == 's')
    {
      Serial.print(" Shoulder - ");
      newPos[1] = newPos[1] + sstep[1];
    }

    if (ByteReceived == 'A')
    {
      Serial.print(" Shoulder + ");
      newPos[2] = newPos[2] - sstep[2];
    }

    if (ByteReceived == 'B')
    {
      Serial.print(" Shoulder - ");
      newPos[2] = newPos[2] + sstep[2];
    }

    if (ByteReceived == 'a')
    {
      Serial.print(" Claw - ");
      newPos[3] = newPos[3] - sstep[3];
    }

    if (ByteReceived == 'd')
    {
      Serial.print(" Claw + ");
      newPos[3] = newPos[3] + sstep[3];
    }

    if (ByteReceived == 'h')
    {

      newPos[0] =  105;
      newPos[1] =   50;
      newPos[2] =   50;
      //  newPos[3] =   50;
      SweepTo() ;
      Serial.print(" HOME ");
    }

    SweepTo() ;
    Serial.println();    // End the line

  }

}

void printNewPos() {
  Serial.print(" ");
  Serial.print(currentPos[0]);
  Serial.print(" : ");
  Serial.print(currentPos[1]);
  Serial.print(" : ");
  Serial.print(currentPos[2]);
  Serial.print(" : ");
  Serial.print(currentPos[3]);
  Serial.print(" ");

}
void pullPos(String memory) {

  int commas[3];
  commas[0] = memory.indexOf(',');
  commas[1] = memory.indexOf(',', commas[0] + 1);
  commas[2] = memory.indexOf(',', commas[1] + 1);

  newPos[0] = memory.substring(0, commas[0]).toInt();
  newPos[1] = memory.substring(commas[0] + 1, commas[1]).toInt();
  newPos[2] = memory.substring(commas[1] + 1, commas[2]).toInt();
  newPos[3] = memory.substring(commas[2] + 1).toInt();

}

void SweepTo()
{

  Serial.println();

  Serial.print("Top of Move To");
  int complete[4] = {0, 0, 0, 0};


  for (int t = 0; t <= 180; t += 1) {

    if (complete[0] == 1 &&
        complete[1] == 1 &&
        complete[2] == 1 &&
        complete[3] == 1) {

      printNewPos();

      break;
    }


    for (int servoNumber = 0; servoNumber <= 3; servoNumber += 1) {
      /*
          Which way are we moving
      */

      if (newPos[servoNumber] < currentPos[servoNumber])
        inc[servoNumber] = -1;
      else inc[servoNumber] = 1;

      Serial.println();

      Serial.print(" Top    ");
      Serial.print( t );
      Serial.print(" Servo:");
      Serial.print(servoNumber);
      Serial.print( " Curr:" );
      Serial.print(currentPos[servoNumber]);
      Serial.print( " New: " );
      Serial.print(newPos[servoNumber]);
      Serial.print( " Min: " );
      Serial.print(mmin[servoNumber]);
      Serial.print( " Max: " );
      Serial.print(mmax[servoNumber]);

      if (currentPos[servoNumber] != newPos[servoNumber] &&
          currentPos[servoNumber] >= mmin[servoNumber] &&
          currentPos[servoNumber] <= mmax[servoNumber])
        currentPos[servoNumber] = currentPos[servoNumber] + inc[servoNumber];
      else {

        Serial.print(" Completed   ");
        Serial.print(servoNumber);

        printNewPos();


        complete[servoNumber] = 1;
        /*
           set to limits
        */
        if (currentPos[servoNumber] < mmin[servoNumber]) {
          currentPos[servoNumber] = mmin[servoNumber] + 5;
          newPos[servoNumber] = mmin[servoNumber]  + 5 ;
          Serial.print(" set to mmin    ");

        }

        if (currentPos[servoNumber] > mmax[servoNumber]) {
          currentPos[servoNumber] = mmax[servoNumber] - 5;
          newPos[servoNumber] = mmax[servoNumber] - 5 ;
          Serial.print(" set to mmax    ");

        }
      }


      /*
          Check limits
      */

      myServo[servoNumber].write(currentPos[servoNumber] );
      delay(ddelay);

    }
  }
}

