#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>


// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
// in which case, set this #define pin to -1!
#define TFT_DC     8

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)

// For 1.44" and 1.8" TFT with ST7735 use
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// For 1.54" TFT with ST7789
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
//#define TFT_SCLK 13   // set these to be whatever pins you like!
//#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

int row;
int x;
int length;
char* str;
float p = 3.1415926;
const int del = 5000;
const unsigned int TRIG_PIN = 7;
const unsigned int ECHO_PIN = 6;
char receivedChar;
boolean newData = false;
const float line_space = 2.0*0.714375;

float darr[16];
float val[16];
float total;
int num;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
  Serial.print("Hello! ST77xx TFT Test");

  // Use this initializer if you're using a 1.8" TFTs

  //tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

  // Use this initializer (uncomment) if you're using a 1.44" TFT
  tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, green tab

  // Use this initializer (uncomment) if you're using a 0.96" 180x60 TFT
  //tft.initR(INITR_MINI160x80);   // initialize a ST7735S chip, mini display

  // Use this initializer (uncomment) if you're using a 1.54" 240x240 TFT
  //tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels

  Serial.println("Initialized");

  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);
}

void loop() {
  for (int line = 2; line < 16; line++)
  {
    while (!newData)
    {
      if (Serial.available() > 0)
      {
        receivedChar = Serial.read();
        if(receivedChar == 'a')
        {
        newData = true;
        receivedChar = ' ';
        }
      }
    }
    total = 0;
    newData = false;
    darr[line - 2] = line * 1.0 * line_space;
    Serial.println("Ready");
    delay(3000);
    Serial.println("Go");
    for (num = 1; num < 20; num++)
    {
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);
      const unsigned long duration = pulseIn(ECHO_PIN, HIGH);
      //Serial.println(duration);
      float distance = duration*1.0  / 29.15 / 2.0;
      Serial.println(distance, 5);
      if (distance <= 0.1)
      {
        num -= 1;
      }
      else
      {
        Serial.println(num, DEC);
        total += distance;
        delay(200);
      }
      //delay(100);
    }
    Serial.println("Done");
    val[line - 2] = total / num * 1.0;
  }
  Serial.println();
  for (int count = 0; count < 14; count++)
  {
    Serial.print(darr[count], 3);
    Serial.print(", ");
    Serial.print(val[count], 3);
    Serial.println();
  }
  delay(50000);
}

