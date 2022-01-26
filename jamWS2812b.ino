#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
//#include <ArduinoJson.h>

#define PinLed D6
#define LEDS_PER_SEG 3
#define LEDS_PER_DOT 2
#define LEDS_PER_DIGIT  LEDS_PER_SEG *7
#define LED   88

const char *ssid     = "IrfanRetmi";
const char *password = "00002222";

int hl;
int hr;
int ml;
int mr;
unsigned long tmrsave=0;
unsigned long tmrsaveHue=0;
unsigned long tmrWarning=0;
int delayWarning(200);
int delayHue(2);
int Delay(500);
int brightnes = 0;
bool dotsOn = false;
bool warningWIFI = false;
static int hue;
int pixelColor;

const long utcOffsetInSeconds = 25200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

Adafruit_NeoPixel strip(LED,PinLed,NEO_GRB + NEO_KHZ800);

long numberss[] = { 
//  7654321
  0b0111111,  // [0] 0
  0b0100001,  // [1] 1
  0b1110110,  // [2] 2
  0b1110011,  // [3] 3
  0b1101001,  // [4] 4
  0b1011011,  // [5] 5
  0b1011111,  // [6] 6
  0b0110001,  // [7] 7
  0b1111111,  // [8] 8
  0b1111011,  // [9] 9
  0b0000000,  // [10] off
  0b1111000,  // [11] degrees symbol
  0b0011110,  // [12] C(elsius)
};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(100);
//  WiFi.begin(ssid, password);
//  while ( WiFi.status() != WL_CONNECTED ) {
//    delay ( 500 );
//    Serial.print ( "." );
//  }
//  timeClient.begin();
}
int counter=0;
void loop() {
  
  counter++;
  
    if(counter==10){counter=0;}
  
  Serial.println(counter);
  Display(counter,0,strip.Color(255,0,0));
  Display(counter,1,strip.Color(0,255,0));
  Display(counter,2,strip.Color(0,0,255));
  Display(counter,3,strip.Color(255,255,255));
  
strip.show();
delay(500);
}

void Display(byte number, byte segment, uint32_t color) {
  // segment from left to right: 3, 2, 1, 0
  byte startindex = 0;
  switch (segment) {
    case 0:
      startindex = 0;
      break;
    case 1:
      startindex = LEDS_PER_DIGIT;
      break;
    case 2:
      startindex = LEDS_PER_DIGIT * 2 + LEDS_PER_DOT * 2;
      break;
    case 3:
      startindex = LEDS_PER_DIGIT * 3 + LEDS_PER_DOT * 2;
      break;    
  }

   for (byte i=0; i<7; i++){                // 7 segments
    for (byte j=0; j<LEDS_PER_SEG; j++) {             // LEDs per segment
      strip.setPixelColor(i * LEDS_PER_SEG + j + startindex , (numberss[number] & 1 << i) == 1 << i ? color : strip.Color(0,0,0));
      //strip.setPixelColor(i * LEDS_PER_SEG + j + startindex] = ((numbers[number] & 1 << i) == 1 << i) ? color : color(0,0,0);
      strip.show();
    }
  } 
  
  //yield();
}

void getClock(){
  
}

void showClock(){
  
}

void showConnect(){
  
}

void showDisconnect(){
  
}

void setColorDigit(){
  
}

void setColorDots(){
  
}
