int IRSensorPIN = 2;
int mLastStatus;
bool mIsStateChange;

int NORMAL_SOUND_PIN = 6;
int LAUGH_SOUND_PIN = 7;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    3

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 8

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() 
{
  pinMode (IRSensorPIN, INPUT);
  mLastStatus = LOW;
  mIsStateChange = false;

  pinMode (NORMAL_SOUND_PIN, OUTPUT);
  pinMode (LAUGH_SOUND_PIN, OUTPUT);
  
  digitalWrite(NORMAL_SOUND_PIN, HIGH); 
  digitalWrite(LAUGH_SOUND_PIN, HIGH); 
  Serial.begin(9600); // open the serial port at 9600 bps
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void playLaughSound() {
  digitalWrite(NORMAL_SOUND_PIN, HIGH); 
  digitalWrite(LAUGH_SOUND_PIN, LOW); 
  Serial.println("playLaughSound!"); 
  delay(5000);
}

void playNormalSound() {
  digitalWrite(NORMAL_SOUND_PIN, LOW); 
  digitalWrite(LAUGH_SOUND_PIN, HIGH);  
  Serial.println("playNormalSound!"); 
}

void turnOnLed() {
  colorWipe(strip.Color(255,   0,   0), 5); // Red
}

void turnOffLed() {
  strip.clear();
  strip.show();
}

void loop()
{
  int currentStatus = digitalRead (IRSensorPIN);
  if (mLastStatus == HIGH && currentStatus == LOW) {
    if(mIsStateChange == false) {
      mIsStateChange = true;
      Serial.println("Something appearing!");
    }
  } else {
    mIsStateChange = false;
  }
  mLastStatus = currentStatus;

  if(mIsStateChange == true) {
    turnOnLed();
    playLaughSound();
  } else {
    turnOffLed();
    playNormalSound();
  }
  delay(50);
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
