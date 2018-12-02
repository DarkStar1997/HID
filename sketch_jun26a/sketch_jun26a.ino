#include <Bounce2.h>

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;


// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;     
float yaw = 0;

uint16_t button1;
uint8_t i;

Bounce btn1 = Bounce();
Bounce btn2 = Bounce();
Bounce btn3 = Bounce();
Bounce btn4 = Bounce();


void setup() 
{
  
  Serial.begin(115200);
  //Serial.begin(19200);

  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  btn1.attach(3, INPUT_PULLUP);
  btn1.interval(15);
  btn2.attach(4, INPUT_PULLUP);
  btn2.interval(15);
  btn3.attach(5, INPUT_PULLUP);
  btn3.interval(15);
  btn4.attach(6, INPUT_PULLUP);
  btn4.interval(15);
}

void loop()
{
  //timer = millis();
 /*Serial.print(analogRead(A4));
 Serial.print(" ");
 Serial.print(analogRead(A5));
 Serial.print(" ");
 Serial.println(analogRead(A4)-analogRead(A5));*/

  btn1.update();
  delay(7);
  btn2.update();
  delay(7);
  btn3.update();
  delay(7);
  btn4.update(); 
  delay(7);
  Vector normAccel = mpu.readNormalizeAccel();

  // Calculate Pitch & Roll
  double pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  double roll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
  
  // Read normalized values 
  /*Vector norm = mpu.readNormalizeGyro();

  // Calculate Pitch, Roll and Yaw
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;*/

  // Output raw
  //Serial.print(" Pitch = ");
  Serial.print((int)((pitch+180)*100.0)+130000);
  //Serial.print((int)((pitch+180)*100.0));
  Serial.print("z");
  //Serial.print(" Roll = ");
  Serial.print((int)((roll+180)*100.0)+130000);
  //Serial.print((int)((roll+180)*100.0));
  Serial.print("z");  
  
  //Serial.print(" Yaw = ");
  Serial.print((int)((yaw+180)*100.0)+130000);
  //Serial.print(0);
  Serial.print("z");

  button1 = 1000*(!btn1.read()) + 100*(!btn2.read())+10*(!btn3.read())+(!btn4.read());
  
  Serial.print((int)button1 + 130000);
  Serial.print("z");

  Serial.print((int)analogRead(A1) + 130000);
  Serial.print("z");

  Serial.print((int)analogRead(A2) + 130000);
  Serial.print("i");
  
  Serial.println();

}
