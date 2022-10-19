#include <FastLED.h>

//This sketch is setup for an Arduino Uno with a momentary pushbutton connected to digital pin 2.

//#define DEBUG               //Write debug information to serial.
#define DEVELOPMENT 7       //Development to manually set mode to pattern under development. Change number to select pattern in switch statement.

//----- Pin Definitions -----
const byte LEDPin = 7;
const byte interruptPin = 2;

//----- Settings -----
const int numLEDs = 20;
const byte brightness = 127;
const byte numPatterns = 7;

//----- Functional Variables -----
bool interruptFlag = false;
CRGB leds[numLEDs];
byte mode = 0;

//----- Debug Variables -----
#ifdef DEBUG
  unsigned int interruptTriggered = 0;
#endif

void setup()
{
  //Interrupt Setup
  //Keeping pinmode outside of compiler directive because button is always attached.
  pinMode(interruptPin, INPUT_PULLUP);
  #ifndef DEVELOPMENT
    attachInterrupt(digitalPinToInterrupt(interruptPin), ChangeMode, RISING);
  #else
    mode = DEVELOPMENT;
  #endif

  //Serial Setup
  #ifdef SerialPrint
    Serial.begin(9600);
  #endif

  //LED Library Setup
  delay(3000);  //LED safety delay.
  FastLED.addLeds<WS2812B, LEDPin, GRB>(leds, numLEDs);
  FastLED.setBrightness(brightness);

  Clear();
}

void loop()
{
  switch (mode)
  {
    case 0:
      Pattern0();
      break;
    case 1:
      Pattern1();
      break;
    case 2:
      Pattern2();
      break;
    case 3:
      Pattern3();
      break;
    case 4:
      //Pattern4(CRGB(255, 0, 0));
      Pattern4(255, 0, 0);
      break;
    case 5:
      //Pattern4(CRGB(0, 0, 255));
      Pattern4(0, 0, 255);
      break;
    case 6:
      Pattern5();
      break;
    case 7:
      Pattern6();
      break;
    default:
      mode = 0;
      Pattern0();
      break;
  }
  
  #ifdef SerialPrint
    Serial.print(millis());
    Serial.print(": Interrupt triggers: ");
    Serial.println(interruptTriggered);
  #endif
}

//Interrupt Service Routine
void ChangeMode()
{
  interruptFlag = true;
  mode++;

  #ifdef DEBUG
    interruptTriggered++;
  #endif
}

//Blank Entire Strip
void Clear()
{
  for (int i = 0; i < numLEDs; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }

  FastLED.show();

  delay(100);
}

//Red/Blue Rise-Fall
//Overall pattern remixed from https://howtomechatronics.com/tutorials/arduino/how-to-control-ws2812b-individually-addressable-leds-using-arduino/
void Pattern0()
{
  //Blue rise
  for (int i = 0; i < numLEDs; i++)
  {
    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    leds[i] = CRGB(0, 0, 255);
    FastLED.show();

    delay(40);
  }

  //Red fall
  for (int i = numLEDs - 1; i >= 0; i--)
  {
    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    leds[i] = CRGB(255, 0, 0);
    FastLED.show();

    delay(40);
  }
}

//Signal Right
//Overall pattern remixed from https://create.arduino.cc/projecthub/bigboystoys13/led-emergency-lights-using-ws2812-rgb-led-module-9800de?ref=similar&ref_id=301530&offset=1
void Pattern1()
{
  //Fill yellow
  for (int i = numLEDs - 1; i >= 0 ; i--)
  {
    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    leds[i] = CRGB(255, 255, 0);
    FastLED.show();

    delay(40);
  }

  //Fill red
  for (int i = numLEDs - 1; i >= 0 ; i--)
  {
    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    leds[i] = CRGB(255, 0, 0);
    FastLED.show();

    delay(40);
  }

  delay(400);
}

//Signal Left
//Overall pattern remixed from https://create.arduino.cc/projecthub/bigboystoys13/led-emergency-lights-using-ws2812-rgb-led-module-9800de?ref=similar&ref_id=301530&offset=1
void Pattern2()
{
  //Fill yellow
  for (int i = 0; i < numLEDs; i++)
  {
    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    leds[i] = CRGB(255, 255, 0);
    FastLED.show();

    delay(40);
  }

  //Fill red
  for (int i = 0; i < numLEDs; i++)
  {
    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    leds[i] = CRGB(255, 0, 0);
    FastLED.show();

    delay(40);
  }

  delay(400);
}

//Wig-Wag
//Overall pattern remixed from https://create.arduino.cc/projecthub/bigboystoys13/led-emergency-lights-using-ws2812-rgb-led-module-9800de?ref=similar&ref_id=301530&offset=1
void Pattern3()
{
  //Set second half red
  for (int i = (numLEDs / 2); i < numLEDs; i++)
  {
    leds[i] = CRGB(255, 0, 0);
  }

  FastLED.show();

  if (interruptFlag)
    interruptFlag = false;

  delay(200);

  //Blank second half
  for (int i = numLEDs / 2; i < numLEDs; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }

  if (interruptFlag)
    interruptFlag = false;

  //Set first half red.
  for (int i = 0; i < numLEDs / 2; i++)
  {
    leds[i] = CRGB(255, 0, 0);
  }

  FastLED.show();

  if (interruptFlag)
    interruptFlag = false;

  delay(200);

  //Clear first half
  for (int i = 0; i < numLEDs / 2; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }

  FastLED.show();

  if (interruptFlag)
    interruptFlag = false;

  delay(200);
}

//Wig-Wag
//void Pattern4(const CRGB& rgb)
void Pattern4(byte red, byte green, byte blue)
{
  byte pattern[8][4] = 
  {
    {1, 1, 1, 0}, 
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 1},
    {1, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 1, 0}, 
    {0, 0, 0, 1}
  };

  //Light cycle
  for (int i = 0; i < 8; i++)
  {
    //First quadrant
    for (int ii = 0; ii < numLEDs / 4; ii++)
      leds[ii] = CRGB(red * pattern[i][0], green * pattern[i][0], blue * pattern[i][0]);
      //leds[ii] = rgb;

    //Second quadrant
    for (int ii = numLEDs / 4; ii < numLEDs / 2; ii++)
      leds[ii] = CRGB(red * pattern[i][1], green * pattern[i][1], blue * pattern[i][1]);
      //leds[ii] = rgb;

    //Third quadrant
    for (int ii = numLEDs / 2; ii < ((numLEDs / 4) * 3); ii++)
      leds[ii] = CRGB(red * pattern[i][2], green * pattern[i][2], blue * pattern[i][2]);
      //leds[ii] = rgb;

    //Forth quadrant
    for (int ii = ((numLEDs / 4) * 3); ii < numLEDs; ii++)
      leds[ii] = CRGB(red * pattern[i][3], green * pattern[i][3], blue * pattern[i][3]);
      //leds[ii] = rgb;

    FastLED.show();

    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    delay(200);
    Clear();
  }
}

//Progress Bar
void Pattern5()
{
  int offset = 1; //One half additional width of marque

  //Move marque forward along strip
  for (int i = (0 - 2 * offset); i < (numLEDs + 2 * offset); i++)
  {
    //Set LEDs
    for (int ii = 0; ii < numLEDs; ii++)
    {
      if (ii >= i - offset && ii <= i + offset)
        leds[ii] = CRGB(0, 255, 0);
      else
        leds[ii] = CRGB(0, 0, 0);
    }

    FastLED.show();

    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    delay(50);
  }

  //Move marque backward along strip
  for (int i = (numLEDs + 2 * offset); i > (0 - 2 * offset); i--)
  {
    //Set LEDs
    for (int ii = numLEDs; ii >= 0; ii--)
    {
      if (ii >= i - offset && ii <= i + offset)
        leds[ii] = CRGB(0, 255, 0);
      else
        leds[ii] = CRGB(0, 0, 0);
    }

    FastLED.show();

    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    delay(50);
  }
}

//Pattern to light one LED at a time for mapping installed LEDs.
void Pattern6()
{
  for(int i = 0; i < numLEDs; i++)
  {
    leds[i] = CRGB(0, 255, 0);

    if(i > 0)
      leds[i - 1] = CRGB(0, 0, 0);

    if (interruptFlag)
    {
      interruptFlag = false;
      break;
    }

    FastLED.show();

    delay(500);
  }

  leds[numLEDs - 1] = CRGB(0, 0, 0);

  FastLED.show();
}