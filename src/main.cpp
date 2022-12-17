#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <Servo.h>
//#include <string>

// define keycode for LCD push button
#define None 0
#define Select 1
#define Left 2
#define Up 3
#define Down 4
#define Right 5
#define DHTPIN 28
#define pin_for_moisture_sensor A1
#define DHTTYPE DHT11
#define servopin 13


int drive_pwm = 8;
int drive_direction = 26;
int angle_offset = 110;
int input_speed = 0;
int speed = 0;
int steer_angle = 0;
int steer_angle_clamped = 0;
bool forward = true;

Servo myservo;

void Move(bool forward, int speed)
{
  if (speed == 0) {
    analogWrite(drive_pwm, 0); 
  }
  else if (forward)
  {
    digitalWrite(drive_direction, LOW);
    delay(50);
    analogWrite(drive_pwm, speed);
  }
  else
  {
    digitalWrite(drive_direction, HIGH);
    delay(50);
    analogWrite(drive_pwm, speed);
  }
}

void setup()
{
  pinMode(drive_pwm, OUTPUT);
  pinMode(drive_direction, OUTPUT);

  myservo.attach(servopin);
  Serial.begin(38400);
}
void loop()
{
  int trash = 0;

  if (Serial.available() > 0)
  {
    String data = Serial.readStringUntil('\n');
    Serial.print("You sent me: ");
    Serial.println(data);
    
    char kResponseBodyFormat[] = "{\"HB\":%d,\"IO\":%d,\"WO\":%d,\"DM\":\"%c\",\"CMD\":[%d,%d]}\n";
    int actual_arguments = sscanf(
        data.c_str(), kResponseBodyFormat,
        &trash, &trash, &trash, &trash, &input_speed, &steer_angle);
    
    //Validate input
    Serial.println(actual_arguments);
    Serial.println(speed);
    Serial.println(steer_angle);
    steer_angle_clamped = min(140, max(steer_angle + angle_offset, 90));
    Serial.print("Clamped angle for servo is ");
    Serial.println(steer_angle_clamped);
  }

  delay(100);
  if(input_speed > 0) {
    forward = true;
  }
  else {
    forward = false;
  }
  
  speed = abs(input_speed);
  Move(forward, speed); // pass in true for forward, false for reverse
  myservo.write(steer_angle_clamped);
}