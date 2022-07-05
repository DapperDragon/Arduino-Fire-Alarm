/*
 * B00330262 Practiacal Assessment 1
 * Systems Programming Concepts
 * 1920_COMP09111_01
 * 27 - 02 - 2020
 * Low light alarm system
 * 
*/
  
// Include libraries for the LCD, SPI and the Wireless Modules
#include <LiquidCrystal.h>
#include <SPI.h>
#include "RF24.h"


// Define Pins used by external LED and switch
#define LED_PIN A1
#define SWITCH_PIN A0


// Variables
// pin numbers, light value and alarm controlling bools
const int buzzer = 8;
int sensorPin = A2;
int lightLevel;
bool alarmIsOn;
bool buzzerSounding;


// Create LCD and set pins used
const int rs = 9, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// Create radio, the address it will use and messege it will send
RF24 radio(7, A5);
byte address[6] = "00001";
char message[] = "Alert from B00330262!";



void setup(){

  // Set buzzer and LED pins as outputs
  // Switch pin as input
  pinMode(buzzer, OUTPUT); 
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);


  // set bool values
  // initially false as alarm is not going off
  alarmIsOn = false;
  buzzerSounding = false;


  // initialise serial monitor and wait for it to connect
  Serial.begin(9600);
  while (!Serial);
  
  
  // set up the 16 x 2 LCD
  // static Light level Message
  lcd.begin(16, 2);
  lcd.print("Light Level:");
  

  // Radio setup, addres for writing, power level, data rates and channel
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(104);
  radio.stopListening();

}




void loop(){

  // Light level read from LDR
  // prints to serial monitor for debugging
  lightLevel = analogRead(sensorPin);
  Serial.println(lightLevel); 

  // print light level to bottom left of LCD
  lcd.setCursor(0, 1); 
  lcd.print(lightLevel);
 

  // Alarm will go off when light level falls below certain level - 200
  // lights external LED and sends warning messege
  if(lightLevel < 200){
    alarmIsOn = true;
    digitalWrite(LED_PIN, HIGH);
    radio.write(&message, sizeof(message));

    // Sound buzzer if not aready on
    if(buzzerSounding == false){
      tone(buzzer, 1000); // Send 1KHz sound signal...
      buzzerSounding = true;
    }  
  }

  
  // if alarm is not going off, LED will go out
  else{
    digitalWrite(LED_PIN, LOW);
    alarmIsOn = false;
  }


  // Stop buzzer when switch is pressed
  // also sends messege to serial monitor for debugging
  if (digitalRead(SWITCH_PIN) == LOW){
    Serial.println("Button pressed!");
    noTone(buzzer);
    buzzerSounding = false;
  }

  
  // loop after 2 seconds
  else
  delay(2000);
}
