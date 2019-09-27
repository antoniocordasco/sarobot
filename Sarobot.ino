/**************************************************************************

 Sarobot

 The purpose of this project is to create a device that can press an external phisical button, after a specific amount of time.
 The components used are:
  - an Arduino nano
  - a servo motor, to press external the button
  - an OLED display, to show the amount of time remaining
  - a button, to switch between seconds and minutes and reset the timer
  - a potentiometer, to set the number of seconds or minutes
  
 **************************************************************************/

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h> 


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const String VERSION = "1.0";

static const unsigned char PROGMEM sarobot_bmp1[] =
{ B00000001, B11000000,
  B00000001, B10000000,
  B00000001, B10000000,
  B00000111, B11100000,
  B00001100, B00110000,
  B00001010, B01010000,
  B01011000, B00011000,
  B01011000, B00011000,
  B01001111, B11110000,
  B01111111, B01011110,
  B01111111, B11111110,
  B00001111, B11110010,
  B00001111, B11110010,
  B00000111, B11100010,
  B00000011, B11000000,
  B00000001, B10000000 };

static const unsigned char PROGMEM sarobot_bmp2[] =
{ B00000011, B10000000,
  B00000001, B10000000,
  B00000001, B10000000,
  B00000111, B11100000,
  B00001100, B00110000,
  B00001010, B01010000,
  B00011000, B00011010,
  B00011000, B00011010,
  B00001111, B11110010,
  B01111111, B01011110,
  B01111111, B11111110,
  B01001111, B11110000,
  B01001111, B11110000,
  B01000111, B11100000,
  B00000011, B11000000,
  B00000001, B10000000 };


  
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

int potPin = A1;
int switchPin = 7;
int buttonPin = 2;
String secsOrMins = "secs";
long timerSecs = -1;
long timerStartSecs = -1;
long nowSecs = -1;
bool missionAccomplished = true;

int servoPin = 9;
int startAngle = 10;
int stopAngle = 50;
Servo Servo1; 

void displayAnimation(void) {


  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Sarobot");
  display.print("v" + VERSION);
  display.display();
  delay(3000);
  
  int y = (display.height() - LOGO_HEIGHT) / 2;
  for(int i = 0; i<= 128; i = i + 4) {
    display.clearDisplay();
    display.display();
    if (i%8 == 0) {
      display.drawBitmap(i, y, sarobot_bmp1, LOGO_WIDTH, LOGO_HEIGHT, 1);
    } else {
      display.drawBitmap(i, y, sarobot_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 1);
    }
    display.display();
    delay(100);
  }  

  
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setCursor(0, 0);
  display.println("Press button to start");
  display.print("timer");
  display.display();
  delay(3000);
}

long getSecondsFromPot() {
  float normalizedVal = (float)analogRead(potPin) / 1023;

  Serial.println(normalizedVal);

  if (secsOrMins == "mins") {
    return (normalizedVal * 590 + 10) * 60;
  } else {
    return normalizedVal * 590 + 10;
  }
}

void restartTimer() {  
  missionAccomplished = false;
  timerStartSecs = millis() / 1000;
  timerSecs = getSecondsFromPot();

  Serial.print("Timer set to ");
  Serial.print(timerSecs);
  Serial.println(" secs");
}

void pressButtonThroughServo() {
  Servo1.write(stopAngle);
  delay(200);
  Servo1.write(startAngle); 
  delay(400);  
}

void setup() {
  Serial.begin(9600);
  Serial.println("booting up...");
  
  Servo1.attach(servoPin); 
  Servo1.write(startAngle); 
  delay(1000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  displayAnimation();
  
  // unfortunately, when the servo motor finds some strong resistance, it resets the arduino board.
  // for this reason we cannot reset the timer in the setup() method, as this would cause the servo to press the button multiple times
  // restartTimer();  
}

void displayRemainingTime() {

  long secsRemaining = timerSecs + timerStartSecs - nowSecs;
  int hours = secsRemaining / 3600;
  int mins = (secsRemaining % 3600) / 60;
  int secs = secsRemaining % 60;
  
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print(hours);
  display.print(":");
  display.print(mins);
  display.print(":");
  display.print(secs);
  display.display();      // Show initial text
}

void loop() {

  nowSecs = millis() / 1000;

  if (!missionAccomplished) {    
    displayRemainingTime();
  }


  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("Button pressed");

    // switching the time unit from seconds to minutes or vice versa
    if (secsOrMins == "mins") {
      secsOrMins = "secs";
    } else {
      secsOrMins = "mins";
    }

    restartTimer();
  } else {
    if (!missionAccomplished && nowSecs - timerStartSecs >= timerSecs) {
      missionAccomplished = true;
      Serial.println("mission accomplished");  
      pressButtonThroughServo();
    }    
  }

  delay(200);
}
