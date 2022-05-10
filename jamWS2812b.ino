#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>

#define PinLed D6
#define LEDS_PER_SEG 3
#define LEDS_PER_DOT 2
#define LEDS_PER_DIGIT  LEDS_PER_SEG *7
#define LED   88

//const char *ssid     = "Irfan.A";
//const char *password = "irfan0204";
WiFiManager wifi;

int h1;
int h2;
int m1;
int m2;
unsigned long tmrsave=0;
unsigned long tmrsaveHue=0;
unsigned long tmrWarning=0;
int delayWarning(200);
int delayHue(5);
int Delay(500);
int brightnes = 0;
bool dotsOn = false;
bool warningWIFI = false;
static int hue;
int pixelColor;
int peakWIFI=0;

const long utcOffsetInSeconds = 25200;
WiFiUDP ntpUDP;
NTPClient Clock(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

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
  0b1011110,  // [13] E
  0b0111101,  // [14] n(N)
  0b1001110,  // [15] t
  0b1111110,  // [16] e
  0b1000101,  // [17] n
  0b1000100,  // [18] r
  0b1000111,  // [19] o
  0b1100111,  // [20] d
  0b0000001,  // [21] i
  0b1000110,  // [22] c
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  digitalWrite(D4,HIGH);
  pinMode(D4,OUTPUT);
  strip.begin();
  strip.setBrightness(150);
  bool connectWIFI = wifi.autoConnect("JAM DIGITAL","00000000");
  if(!connectWIFI){
    Serial.println("NOT CONNECTED TO AP");
    
  }
  if ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
    peakWIFI++;
    if(peakWIFI == 100){ 
      showError();
    }
  }
  
  Clock.begin();
  showConnect();
  delay(2000);
  digitalWrite(D4,LOW);
}

void loop() {
getClock();
timerRestart();
timerHue();
stateWIFI();
strip.show();

}

void DisplayNumber(byte number, byte segment, uint32_t color) {
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
  Clock.update();
  h1 = Clock.getHours() / 10;
  h2 = Clock.getHours() % 10;
  m1 = Clock.getMinutes() / 10;
  m2 = Clock.getMinutes() % 10;
  int jam = Clock.getHours();
  int menit = Clock.getMinutes();
//  Serial.print(jam);
//  Serial.print(":");
//  Serial.println(menit);
}

void showClock(uint32_t color){
  DisplayNumber(h1,3,color);
  DisplayNumber(h2,2,color);
  DisplayNumber(m1,1,color);
  DisplayNumber(m2,0,color);
}

void showConnect(){
  DisplayNumber( 12, 3,strip.Color(255,0,0));
  DisplayNumber( 17, 2,strip.Color(0,255,0));
  DisplayNumber( 16, 1,strip.Color(0,255,0));
  DisplayNumber( 15, 0,strip.Color(0,255,0));
}

void showDisconnect(){
  DisplayNumber( 20, 3,strip.Color(255,0,0));
  strip.setPixelColor(63 , strip.Color(255,0,0));
  DisplayNumber( 21, 2,strip.Color(255,0,0));
  DisplayNumber( 5, 1,strip.Color(255,0,0));
  DisplayNumber( 22, 0,strip.Color(255,0,0));
}

void showError(){
  DisplayNumber( 13, 3,strip.Color(255,0,0));
  DisplayNumber( 18, 2,strip.Color(255,0,0));
  DisplayNumber( 19, 1,strip.Color(255,0,0));
  DisplayNumber( 18, 0,strip.Color(255,0,0));
}

void stateWIFI(){
  
  unsigned long tmr = millis();
  if(WiFi.status() != WL_CONNECTED) {
      if(tmr - tmrWarning > delayWarning){
        tmrWarning = tmr;
        if(warningWIFI){
      Serial.println("DISCONNECTED");
      digitalWrite(D4,HIGH);
        }
        
        else{ digitalWrite(D4,LOW); }
        warningWIFI = !warningWIFI;
        }    
        
         for(int i = 42; i <= 45; i++){
      strip.setPixelColor(i , strip.Color(0,0,0));
    }
        showDisconnect();
     }
     else{
      digitalWrite(D4,LOW);
      warningWIFI=false;   
      showClock(Wheel((hue+pixelColor) & 255));
     showDots(strip.Color(255,0,0));
      }
}     

void showDots(uint32_t color) {
  unsigned long tmr = millis();
  if(tmr - tmrsave > Delay){
    tmrsave = tmr;
  if (dotsOn) {
    for(int i = 42; i <= 45; i++){
      strip.setPixelColor(i , color);
    }

  } else {
    for(int i = 42; i <= 45; i++){
      strip.setPixelColor(i , strip.Color(0,0,0));
    }
  }
  dotsOn = !dotsOn;  
}
strip.show();
}


void timerHue(){
  unsigned long tmr = millis();
  if(tmr - tmrsaveHue > delayHue){
    tmrsaveHue = tmr;
  if(pixelColor <256){
    pixelColor++;
    if(pixelColor==255){
      pixelColor=0;
    }
  }
}

  for(int hue=0; hue<strip.numPixels(); hue++) {
    hue++;
      //strip.setPixelColor(hue,Wheel((i+pixelColor) & 255));
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void timerRestart(){
   Clock.update();
   int jam = Clock.getHours();
   int menit = Clock.getMinutes();
   int detik = Clock.getSeconds();

   if(jam == 0 && menit == 0 && detik == 0){
    ESP.restart();
   }
   if(jam == 12 && menit == 0 && detik == 0){
    ESP.restart();
   }
   if(jam == 18 && menit == 0 && detik == 0){
    ESP.restart();
   }
}
