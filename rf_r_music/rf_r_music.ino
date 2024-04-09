#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define SERVO_1_PIN  5 //assign servo_1_pin to D5
#define SERVO_2_PIN  6 //assign servo_2_pin to D6

#define MOTOR1A 9 //define motor speed pin
#define MOTOR1B 3 //define motor direction pin

#define MOTOR_DEAD_ZONE   50 //define motor deadzone

#define dpin_sensor 19 //define digital pin of moisture sensor
#define apin_sensor A6 //define analog pin of moisture sensor
#define buzzer A0 //define buzzer pin 

RF24 radio(7, 8); //define radio CE & CSN pins
const byte addresses[][6] = {"00001", "00002"};
Servo Servo1;
Servo Servo2;

static int8_t Send_buf[6] = {0};

#define CMD_SEL_DEV 0X35 //select the SD card of the audio board
#define DEV_TF 0X01
#define CMD_SET_VOLUME 0X31 //set volume of audio board
#define CMD_PLAY_W_VOL 0X31 //play with volume
#define CMD_SET_PLAY_MODE 0X33 //set play mode of audio board
#define ALL_CYCLE 0X00 //cycle all songs
#define SINGLE_CYCLE 0X01 //single repeat
#define CMD_PLAY_COMBINE 0X45 

void sendCommand(int8_t command, int16_t dat);

void setup() 
{
  Serial.begin(9600); //begin serial communication

  Servo1.attach(SERVO_1_PIN); //attach servo_1_pin to servo1
  Servo2.attach(SERVO_2_PIN); //attach servo_2_pin to servo2

  radio.begin(); //initialise the radio
  radio.openWritingPipe(addresses[0]); //open a pipe for writing arrays
  radio.openReadingPipe(1, addresses[1]); //open a reading pipe
  radio.setPALevel(RF24_PA_MIN); //set power amplifier level
  radio.startListening(); //receive mode

  Serial1.begin(9600); //begin serial communication
	delay(500);
  sendCommand(CMD_SEL_DEV, DEV_TF); //select the SD card of the audio board
	delay(200);
	playWithVolume(0X1201); //set play volume

  pinMode(dpin_sensor, OUTPUT);
  digitalWrite(dpin_sensor, LOW);
}

void loop() 
{ 
  if (radio.available()) 
  {
    char buffer[3];
    radio.read(buffer, 3); //read the three buffer values from the radio transceiver
    int angle1 = buffer[0]; //write buffer 0 to variable angle1
    int angle2 = buffer[1]; //write buffer 1 to variable angle2

    int speed = map(buffer[2], 255, 0, -255, 255); //map buffer 2 to speed & direction values

  if (speed < -MOTOR_DEAD_ZONE) //if speed is below -motor dead zone
    {
      digitalWrite(MOTOR1A, HIGH);
      analogWrite(MOTOR1B, -speed);
      Serial.print('B'); //rotate the motor backwards
    }
  else if(speed > MOTOR_DEAD_ZONE) //if speed is above motor dead zone
    {
      digitalWrite(MOTOR1B, HIGH);
      analogWrite(MOTOR1A, speed);
      Serial.print('F'); //rotate the motor forwards
    } 
    else //else 
    { 
      digitalWrite(MOTOR1A, HIGH);
      digitalWrite(MOTOR1B, HIGH);
      Serial.print('S'); //stall
    }

    Servo1.write(angle1); //write angle1 to servo1
    Servo2.write(angle2); //write angle2 to servo2
    
    Serial.print(angle1); //serial print angle1 for debugging 
    Serial.print(" ");
    Serial.print(angle2); //serial print angle2 for debugging
    Serial.print(" ");
    Serial.println(speed); //serial print speed for debugging
  } 
  else 
  {
    delay(1);
  }

  Serial.println(readSensor());
  if (readSensor() < 500) //if the moisture sensor reads below 500
  {
    tone (buzzer, 400); //sound the buzzer
  }
  else //else
  {
    noTone (buzzer); //nothing
  }
}

void playWithVolume(int16_t dat) 
{
  mp3_6bytes(CMD_PLAY_W_VOL, dat); //play 6 byte mp3 with designated volume
}

void sendCommand(int8_t command, int16_t dat = 0)
{
  if((command == CMD_PLAY_W_VOL)||(command == CMD_SET_PLAY_MODE)||(command == CMD_PLAY_COMBINE))
  	return;
  else if(command < 0x50)
  { 
	mp3_6bytes(command, dat);
  }
  else return;
}

void mp3_6bytes(int8_t command, int16_t dat)
{
  Send_buf[0] = 0x7e;
  Send_buf[1] = 0x04;
  Send_buf[2] = command; 
  Send_buf[3] = (int8_t)(dat >> 8);
  Send_buf[4] = (int8_t)(dat);
  Send_buf[5] = 0xef;
  sendBytes(6); //write data to audio board to play music
}

void sendBytes(uint8_t nbytes)
{
  for(uint8_t i=0; i < nbytes; i++)
  {
    Serial1.write(Send_buf[i]) ;
  }
}

int readSensor() 
{
  digitalWrite(dpin_sensor, HIGH); //set the digital moisture sensor pin to high
  delay(20);
  int val = analogRead(apin_sensor); //read the analog moisture sensor pin value
  digitalWrite(dpin_sensor, LOW); //set the digital moisture sensor pin to low
  return val;
}