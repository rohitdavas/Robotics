
#define leftCenterSensor   A3
#define leftNearSensor     A4
#define leftFarSensor      A5
#define rightCenterSensor  A2
#define rightNearSensor    A1
#define rightFarSensor     A0

int leftCenterReading;
int leftNearReading;
int leftFarReading;
int rightCenterReading;
int rightNearReading;
int rightFarReading;

int leftNudge;
int replaystage = 0;
int rightNudge;

#define leapTime 200
#define leftMotor1  7
#define leftMotor2  6
#define rightMotor1 5
#define rightMotor2 8
#define led 13

char path[50] = {};
int pathLength = 0;
int readLength = 0;

void setup() {

  pinMode(leftCenterSensor, INPUT);
  pinMode(leftNearSensor, INPUT);
  pinMode(leftFarSensor, INPUT);
  pinMode(rightCenterSensor, INPUT);
  pinMode(rightNearSensor, INPUT);
  pinMode(rightFarSensor, INPUT);

  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  digitalWrite(led, HIGH);
  delay(500);
}
//----------------------------------------------------------
void loop() {
  readSensors();  //debug //Serial.println(leftFarReading);

  if (leftFarReading > 500 && rightFarReading > 500 && (leftCenterReading < 500 || rightCenterReading < 500) ) {
    straight();
  }
  else {
    leftHandWall();
  }
}
//----------------------------------------------------reading the sensors---------------------
void readSensors() {
  leftCenterReading = analogRead(leftCenterSensor); leftNearReading = analogRead(leftNearSensor); leftFarReading = analogRead(leftFarSensor);
  rightCenterReading = analogRead(rightCenterSensor); rightNearReading = analogRead(rightNearSensor); rightFarReading  = analogRead(rightFarSensor);
}
//--------------------------------------------------------------------------
void leftHandWall() {
  //-----------------------case 1 when we have leftfar and rightfar both in black.---T point,junction point--.
  if ( leftFarReading < 500 && rightFarReading < 500)
  {
    digitalWrite(leftMotor1, HIGH); digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
    delay(1000); readSensors();
    if (leftFarReading < 500 && rightFarReading < 500) {
      done();
    }
    if (leftFarReading > 500 && rightFarReading > 500) {
      turnLeft();
    }
  }
  //----------------------------------------------case1 is currect.

  //---------------------------------------------case 2 when we have left side on black.
  if (leftFarReading < 500) { // if you can turn left then turn left
    digitalWrite(leftMotor1, HIGH); digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
    delay(leapTime); readSensors();
    if (leftFarReading > 500 && rightFarReading > 500) {
      turnLeft();
    }
    if (leftFarReading < 500 && rightFarReading < 500) {
      done();
    }
  }
  //----------------------------------------------case 3 when right far is on black.
  if (rightFarReading < 500) {
    digitalWrite(leftMotor1, HIGH); digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
    delay(leapTime-100); readSensors();
    if (leftFarReading < 500) {
      delay(leapTime - 30);  readSensors();
      if (rightFarReading < 500 && leftFarReading < 500) {
        done();
      }
      else {
        turnLeft();
       return; /////////////////////////////////
      }
    }
    delay(leapTime - 30);/////////////////////
    readSensors();
    if (leftFarReading > 500 && leftCenterReading > 500 && rightCenterReading > 500 && rightFarReading > 500) {
      turnRight();
      return;
    }

    if (replaystage == 0) {
      path[pathLength] = 'S';  //Serial.println("s");
      pathLength++;                                 //Serial.print("Path length: "); Serial.println(pathLength);

      if (path[pathLength - 2] == 'B') {
        shortPath();
      }
    }//Serial.println("shortening path");
    straight();
  }

  //------------case 4 when all the sensors read white,turn around to check for black and follow it.//
  readSensors();
  if (leftFarReading > 500 && leftCenterReading > 500 && rightCenterReading > 500 && rightFarReading > 500 && leftNearReading > 500 && rightNearReading > 500)
  {
    turnAround();
  }

}
//--------------------------------------------------------------------------------------------------
void reply(){
  readSensors();
  if (leftFarReading > 500 && rightFarReading > 500) {
    straight();
  }
  else {
    if (path[readLength] == 'D') {
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      delay(leapTime);
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
      endMotion();
    }
    if (path[readLength] == 'L') {
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      delay(leapTime);
      turnLeft();
    }
    if (path[readLength] == 'R') {
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      delay(leapTime);
      turnRight();
    }
    if (path[readLength] == 'S') {
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      delay(leapTime);
      straight();
    }

    readLength++;
  }

  reply();
}








//----------case when done is called.
void done(){
  digitalWrite(leftMotor1, LOW);
    shortDone=1;
  }
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
  replaystage=1;
  path[pathLength]='D';
  pathLength++;
 while(analogRead(leftFarSensor)<500){
   digitalWrite(led, LOW);
   delay(1500);
   digitalWrite(led, HIGH);
   delay(1500);
 }
 delay(8000);
  reply();
}
//---------------------------------------------------------------
//-------------------------case to turn left
void turnLeft() {
  while (analogRead(rightCenterSensor) < 500 || analogRead(leftCenterSensor) < 500) {
    digitalWrite(leftMotor1, LOW); digitalWrite(leftMotor2, HIGH);
    digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
    delay(2);
    //  digitalWrite(leftMotor1, LOW);
    // digitalWrite(leftMotor2, LOW);
    //digitalWrite(rightMotor1, LOW);
    //digitalWrite(rightMotor2, LOW);
    //delay(1);
  }

  while (analogRead(rightCenterSensor) > 500) {
    digitalWrite(leftMotor1, LOW); digitalWrite(leftMotor2, HIGH);
    digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
    delay(2);
    //  digitalWrite(leftMotor1, LOW);
    // digitalWrite(leftMotor2, LOW);
    //digitalWrite(rightMotor1, LOW);
    //digitalWrite(rightMotor2, LOW);
    //delay(1);
  }

  if (replaystage == 0) {
    path[pathLength] = 'L';
    //Serial.println("l");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
    if (path[pathLength - 2] == 'B') {
      //Serial.println("shortening path");
      shortPath();
    }
  }
}
//--------------------------------------------
//-----------------------case to turn right
void turnRight() {
  while (analogRead(rightCenterSensor) < 500) {
    digitalWrite(leftMotor1, HIGH); digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW); digitalWrite(rightMotor2, HIGH);
    delay(2);
    // digitalWrite(leftMotor1, LOW);
    //digitalWrite(leftMotor2, LOW);
    //digitalWrite(rightMotor1, LOW);
    //digitalWrite(rightMotor2, LOW);
    //delay(1);
  }
  while (analogRead(rightCenterSensor) > 500) {
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
    delay(2);
    //  digitalWrite(leftMotor1, LOW);
    //digitalWrite(leftMotor2, LOW);
    //digitalWrite(rightMotor1, LOW);
    //digitalWrite(rightMotor2, LOW);
    //delay(1);
  }
  while (analogRead(leftCenterSensor) > 500) {
    digitalWrite(leftMotor1, HIGH); digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW); digitalWrite(rightMotor2, HIGH);
    delay(2);
    // digitalWrite(leftMotor1, LOW);
    //digitalWrite(leftMotor2, LOW);
    //digitalWrite(rightMotor1, LOW);
    //digitalWrite(rightMotor2, LOW);
    //delay(1);
  }

  if (replaystage == 0) {
    path[pathLength] = 'R';
    //Serial.println("r");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
    if (path[pathLength - 2] == 'B') {
      //Serial.println("shortening path");
      shortPath();
    }
  }

}
//--------------------------------------------------------------------------------------
//-----------------------------case to go straight---------------
void straight() {
  if ( analogRead(leftCenterSensor) > 500) {
    digitalWrite(leftMotor1, HIGH);  digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
    delay(1);
    digitalWrite(leftMotor1, HIGH); digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW); digitalWrite(rightMotor2, LOW);
    delay(1);
    return;
  }
  if (analogRead(rightCenterSensor) > 500) {
    digitalWrite(leftMotor1, HIGH); digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
    delay(1);
    digitalWrite(leftMotor1, LOW); digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
    delay(1);
    return;
  }

  digitalWrite(leftMotor1, HIGH); digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
  delay(1);
  //digitalWrite(leftMotor1, LOW);
  //digitalWrite(leftMotor2, LOW);
  //digitalWrite(rightMotor1, LOW);
  //digitalWrite(rightMotor2, LOW);
  //delay(1);

}
//----------------------------------------------------------------------------
//------------------case to turn around-----------------
void turnAround() {
  digitalWrite(leftMotor1, HIGH); digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
  delay(leapTime+leapTime+leapTime);
  while (analogRead(leftCenterSensor) > 500) {
    digitalWrite(leftMotor1, LOW); digitalWrite(leftMotor2, HIGH);
    digitalWrite(rightMotor1, HIGH); digitalWrite(rightMotor2, LOW);
    //delay(2);
    //digitalWrite(leftMotor1, LOW);
    //digitalWrite(leftMotor2, LOW);
    //digitalWrite(rightMotor1, LOW);
    //digitalWrite(rightMotor2, LOW);
    //delay(1);
  }
  if (replaystage == 0) {
    path[pathLength] = 'B';
    pathLength++;
  }
  straight();
  // Serial.println("b");
  //Serial.print("Path length: ");
  //Serial.println(pathLength);
}


//void short path here.
void shortPath(){
 int shortDone=0;
 if(pathLength<3 || path[pathLength-2] != 'B'){shortDone = 1; return;}
 while(path[pathLength-2] == 'B'){
  if(path[pathLength-3]=='L' && path[pathLength-1]=='R' && shortDone==0){
    pathLength-=3;
    path[pathLength]='B';
    //Serial.println("test1");
    shortDone=1;
  }
   
  if(path[pathLength-3]=='L' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    //Serial.println("test2");
    shortDone=1;
  }
   
  if(path[pathLength-3]=='R' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='B';
    //Serial.println("test3");
    shortDone=1;
  }
  
   
  if(path[pathLength-3]=='S' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    //Serial.println("test4");
    shortDone=1;
  }
     
  if(path[pathLength-3]=='S' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='B';
    //Serial.println("test5");
    shortDone=1;
  }
    if(path[pathLength-3]=='L' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='S';
    //Serial.println("test6");
    shortDone=1;
  }
  
  path[pathLength+1]='D';
  path[pathLength+2]='D';
  pathLength++;
 }//Serial.print("Path length: ");
  //Serial.println(pathLength);
  //printPath();
  return;
}





//void replay here.


//void serial print function
void printPath() {
  Serial.println("+++++++++++++++++");
  int x;
  while (x <= pathLength) {
    Serial.println(path[x]);
    x++;
  }
  Serial.println("+++++++++++++++++");
}


void endMotion() {
  digitalWrite(led, LOW);
  delay(10);
  digitalWrite(led, HIGH);
  delay(10);
  digitalWrite(led, LOW);
  delay(10);
  digitalWrite(led, HIGH);
  delay(10);
  replaystage = 0;
  endMotion();
}
