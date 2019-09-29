#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>  //Lib to play auido (https://github.com/TMRh20/TMRpcm)
#include <pcmConfig.h>
#include <pcmRF.h>
#include <Servo.h>  // servo library

const int SPEAK_PIN = 8;
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;

Servo servoRightWing;  // servo control object
TMRpcm audio; //Lib object is named "audio"

long duration;
int distance;
bool near = false;
bool lastNear = false;
int countMismatch = 0;
int CS_PIN = 53;

void setup()
{
  Serial.begin(9600); // Starts the serial communication

  pinMode(CS_PIN, OUTPUT); //declaring CS pin as output pin
  pinMode(13, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input
  audio.speakerPin = SPEAK_PIN; //Auido out on pin 9
  audio.setVolume(5);
  audio.quality(1);

  if (SD.begin(CS_PIN))
  {
    Serial.println("SD card is initialized and it is ready to use");
  } else
  {
    Serial.println("SD card is not initialized");
    return;
  }
  servoRightWing.attach(12, 1500, 2000);
}


void loop()
{

  do {
    // Clears the trigPin
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_PIN, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance: ");
    //Serial.println(distance);
    if (distance < 20) {
      near = true;
    } else {
      near = false;
    }

    if (lastNear != near) {
      countMismatch++;
    } else if (countMismatch > 0) {
      countMismatch--;
    }

  } while ( countMismatch <= 10 );

  if (near) {
    servoRightWing.write(0);
    digitalWrite(13, HIGH);
    Serial.println("Triggered near region.");
    audio.play("autobots3.wav");
    /*for (int i = 1000; i > 300; i = i - 50) {
      tone(SPEAK_PIN, i, 500);
      delay(100);
    }*/
  } else {
    Serial.println("Triggered far region.");
    servoRightWing.write(180);
    digitalWrite(13, LOW);
    //tone(SPEAK_PIN, 1000, 400);
   /* for (int i = 300; i <= 1000; i = i + 50) {
      tone(SPEAK_PIN, i, 500);
      delay(100);
    }*/

  }
  lastNear = near;
  countMismatch = 0;

}
