#include<Wire.h>
#include<Servo.h>
const int MPU=0x68; //I2C address of MPU
int GyX,GyY,GyZ;
float pitch=0;
float roll=0;
float yaw=0;
float prev=0;
float i=0;

float v_pitch; 
float v_roll;
float v_yaw;
float a_pitch;
float a_roll;
float a_yaw;

Servo servo1,servo2;
int sensorValue= A0;
int LDRvalue= 0;

void setup()
{
  pinMode(8,OUTPUT);
  pinMode(10,OUTPUT);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); //power management register 1
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  servo1.attach(2);
  servo2.attach(4);
}

void loop() {
  LDRvalue= analogRead(sensorValue);
  Serial.println(LDRvalue);
  delay(100);
  Wire.beginTransmission(MPU);
  Wire.write(0x43); //starts with MPU register 43(GYRO_XOUT_H) 
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU,6,true); //requests 6 registers 
  
  GyX=Wire.read()<<8|Wire.read(); 
  GyY=Wire.read()<<8|Wire.read(); 
  GyZ=Wire.read()<<8|Wire.read(); 
  v_pitch=(GyX/131); 
  if(v_pitch==-1) {
    v_pitch=0;
  }
  v_roll=(GyY/131); 
  if(v_roll==1) {
    v_roll=0;
  }
  v_yaw=GyZ/131;
  a_pitch=(v_pitch*0.047); 
  a_roll=(v_roll*0.047); 
  a_yaw=(v_yaw*0.047);
  
  pitch= pitch + a_pitch;
  roll= roll + a_roll;
  yaw= yaw + a_yaw;
  Serial.print(" | pitch = ");
  Serial.print(pitch);
  Serial.print(" | roll = ");
  Serial.print(roll);
  Serial.print(" | yaw = ");
  Serial.println(yaw);
  
  prev = roll - a_roll;
  //Serial.println(prev);
   if (LDRvalue<=800) {
    if (prev!= roll)    // loop to check if the previous value and the current value of roll are same or not 
  {
    if (prev++) // loop to check if there is an increment in the previous value of roll
    {
      digitalWrite(8,HIGH);
      digitalWrite(10,HIGH);  // indicating headlights are turning
      delay(100); 
      digitalWrite(8,LOW); 
      digitalWrite(10,LOW);
      i=map(prev,0,45,90,179); // scaling i value to the roll value and the angle value between 90 to 179
      servo1.write(i);
      servo2.write(i);
      Serial.println(i); // display the angle for our reference
      delay(10);
    }
    else if (prev--)  // loop to check if there is a decrement in the previous value of roll
    {
      digitalWrite(8,LOW); 
      digitalWrite(10,LOW); // indicating headlights are turning
      i= map(prev,-45,0,89,0);  // scaling i value to the roll value and the angle value between 0 to 90
      servo1.write(i);
      servo2.write(i);
      Serial.println(i); // display the angle for our reference
      delay(10);
    } 
  }
  else  //when the previous and the current value of roll are the same
  {
    delay(10);
    digitalWrite(10,HIGH);   //indicating that there is no change in roll
    digitalWrite(8,HIGH);
    servo1.write(i);
    servo2.write(i);   
  }
 }
 else {
  digitalWrite(10,LOW);
  digitalWrite(8,LOW);
  servo1.write(90);
  servo2.write(90);
 }

 
  
 }
