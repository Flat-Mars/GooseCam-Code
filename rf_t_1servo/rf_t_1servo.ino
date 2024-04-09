#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "U_R_DR.h"
#include "U_L_DR.h"
#include "U_L_DL.h"
#include "D_L_DL.h"
#include "D_R_DR.h"
#include "D_R_DL.h"
#include "D.h"
#include "U.h"
#include "R.h"
#include "L.h"
#include "DL.h"
#include "DR.h"
#include "D_R.h"
#include "D_L.h"
#include "U_L.h"
#include "U_R.h"
#include "D_DL.h"
#include "U_DL.h"
#include "U_DR.h"
#include "D_DR.h"
#include "R_DL.h"
#include "R_DR.h"
#include "L_DR.h"
#include "L_DL.h"
#include "U_R_DL.h"
#include "D_L_DR.h"
#include "duckcam.h" //include all custom bitmaps

#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SSD1306_NO_SPLASH //remove the default adafruit bitmap
#define SCREEN_WIDTH 128 //define OLED screen width 
#define SCREEN_HEIGHT 64 //define OLED screen height
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //OLED screen drivers

#define SDA_PIN D18 
#define SCL_PIN D19

#define LOGO_HEIGHT 64 //bitmap height
#define LOGO_WIDTH 128 //bitmap width

RF24 radio(7, 8); //define radio CE & CSN pins
const byte addresses[][6] = {"00001", "00002"};

void setup() 
{
  Serial.begin(9600); //begin serial communication
  
  radio.begin(); //initialise the radio
  radio.openWritingPipe(addresses[1]); //open a pipe for writing arrays
  radio.openReadingPipe(1, addresses[0]); //open a reading pipe
  radio.setPALevel(RF24_PA_MIN); //set power amplifier level
  radio.stopListening(); //transmit mode
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    duckcam, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(5000); //display the initialisation bitmap for 5 seconds
}

void loop() 
{
  delay(50);
  char buffer[3];
  int rudder = analogRead(A6); //analog read A6 and save value to variable "rudder"
  buffer[0] = map(rudder, 0, 1023, 0, 180); //map the rudder value from 0 to 180 and write the value to buffer 0
  
  int head = analogRead(A3); //analog read A3 and save the value to variable "head"
  buffer[1] = map(head, 0, 1023, 0, 180); //map the head value from 0 to 180 and write the value to buffer 1

  int motor = analogRead(A7); //analog read A7 and save the value to variable "motor"
  buffer [2] = map(motor, 0, 1023, 0, 255); //map the motor value from 0 to 180 and write the value to buffer 2
  radio.write(buffer, 3); //send the buffer values via radio communication

  display.clearDisplay(); //clear the display

  int ruddero = map(analogRead(A6), 0, 1023, 0, 180); //analog read A6 and save value to variable "ruddero"
  int heado = map(analogRead(A3), 0, 1023, 0, 180); //analog read A3 and save the value to variable "heado"
  int motoro = map(analogRead(A7), 0, 1023, 0, 255); //analog read A7 and save the value to variable "motoro"
  
  if (ruddero >= 90 && ruddero <= 100 && motoro >= 115 && motoro <= 130 && heado >= 90 && heado <= 100) //if the joystick values are between these values
  {
  display.clearDisplay();
  display.display(); //display a blank screen
  }

  else if (ruddero < 90 && motoro >= 115 && motoro <= 130 && heado >= 90 && heado <= 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    L, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display left arrow only
  }

  else if (ruddero > 90 && motoro >= 115 && motoro <= 130 && heado >= 90 && heado <= 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    R, LOGO_WIDTH, LOGO_HEIGHT, 1); 
  display.display(); //display right arrow only
  }

  else if (ruddero >= 90 && ruddero <= 100 && motoro < 115 && heado >= 90 && heado <= 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    U, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display up arrow only
  }

  else if (ruddero < 90 && motoro < 115 && heado >= 90 && heado <= 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    U_L, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display an up and left arrow only
  }

  else if (ruddero > 100 && motoro < 115 && heado >= 90 && heado <= 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    U_R, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display an up and right arrow only
  }

  else if (ruddero >= 90 && ruddero <= 100 && motoro > 130 && heado >= 90 && heado <= 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    D, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display a down arrow only
  }

  else if (ruddero < 90 && motoro > 130 && heado >= 90 && heado <= 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    D_L, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display a down and left arrow only
  }

  else if (ruddero > 100 && motoro > 130 && heado >= 90 && heado <= 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    D_R, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display a down and right arrow only
  }

  else if (ruddero >= 90 && ruddero <= 100 && motoro >= 115 && motoro <= 130 && heado < 90) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    DR, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display duck head right only
  }

  else if (ruddero >= 90 && ruddero <= 100 && motoro >= 115 && motoro <= 130 && heado > 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    DL, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display duck head left only
  }

  else if (ruddero < 90 && motoro >= 115 && motoro <= 130 && heado > 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    L_DL, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display left arrow and duck head left only
  }

  else if (ruddero > 100 && motoro >= 115 && motoro <= 130 && heado > 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    R_DL, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display right arrow and duck head left only
  }

  else if (ruddero < 90 && motoro >= 115 && motoro <= 130 && heado < 90) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    L_DR, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display left arrow and duck head right only
  }

  else if (ruddero > 150 && motoro >= 115 && motoro <= 130 && heado < 90) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    R_DR, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display right arrow and duck head right only
  }
  
  else if (ruddero >= 90 && ruddero <= 100 && motoro < 115 && heado > 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    U_DL, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display up arrow and duck head left
  }

  else if (ruddero >= 90 && ruddero <= 100 && motoro > 130 && heado > 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    D_DL, LOGO_WIDTH, LOGO_HEIGHT, 1); //display down arrow and duck head left only
  display.display();
  }

  else if (ruddero >= 90 && ruddero <= 100 && motoro < 115 && heado < 90) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    U_DR, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display up arrow and duck head right only
  }

  else if (ruddero >= 90 && ruddero <= 100 && motoro > 130 && heado < 90) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    D_DR, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display down arrow and duck head right only
  }

  else if (ruddero < 90 && motoro < 115 && heado < 90) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    U_L_DR, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display up arrow, left arrow and duck head right
  }
  
  else if (ruddero > 100 && motoro < 115 && heado < 90) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    U_R_DR, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display up arrow, right arrow and duck head right
  }

  else if (ruddero < 90 && motoro < 115 && heado > 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    U_L_DL, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display up arrow, left arrow and duck head left 
  }
  
  else if (ruddero > 100 && motoro < 115 && heado > 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    U_R_DL, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display up arrow, right arrow and duck head left
  }

  else if (ruddero < 90 && motoro > 130 && heado > 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    D_L_DL, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display down arrow, left arrow and duck head left
  }

  else if (ruddero > 100 && motoro > 130 && heado > 100) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    D_R_DL, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display down arrow, right arrow and duck head left
  }

  else if (ruddero < 90 && motoro > 130 && heado < 90) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    D_L_DR, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display down arrow, left arrow and duck head right
  }

  else if (ruddero > 100 && motoro > 130 && heado < 90) //if the joystick values are between these values
  {
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    D_R_DR, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); //display down arrow, right arrow and duck head right
  }

  else //catch all
  {
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("ERROR");
  display.display(); //display an error message
  }

  Serial.print (ruddero); //serial print ruddero for debugging 
  Serial.print (" ");
  Serial.print (heado); //serial print heado for debugging
  Serial.print (" ");
  Serial.println (motoro); //serial print motoro for debugging
}
