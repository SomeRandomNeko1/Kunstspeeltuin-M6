const unsigned int buttonPin = 7;
const unsigned int ledPin = 13;
const unsigned int BAUD_RATE = 9600;
int buttonState = 0;
int oldButtonState = LOW;
int ledState = LOW;

#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    94
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

unsigned long previousMillis = 0;
unsigned long interval = 1000; // Animation in milliseconds

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  delay(1000); // powerup safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = CRGBPalette16(
                     CRGB::Purple,
                     CRGB::Red,
                     CRGB::Blue,
                     CRGB::Purple,
                     CRGB::Red,
                     CRGB::Blue,
                     CRGB::Purple,
                     CRGB::Red,
                     CRGB::Blue,
                     CRGB::Purple,
                     CRGB::Red,
                     CRGB::Blue,
                     CRGB::Purple,
                     CRGB::Red,
                     CRGB::Blue,
                     CRGB::Purple
                   );

  currentBlending = LINEARBLEND;
  Serial.begin(BAUD_RATE);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  Serial.print("Button State: ");
  Serial.print(buttonState);

  if (buttonState != oldButtonState && buttonState == HIGH) {
    ledState = (ledState == LOW ? HIGH : LOW);
    digitalWrite(ledPin, ledState);

    Serial.print(" - LED State: ");
    Serial.println(ledState);

    if (ledState == HIGH) {
      previousMillis = millis(); 

      while (millis() - previousMillis <= interval) {
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1;

        fill_solid(leds, NUM_LEDS, currentPalette[startIndex % 16]);
        FastLED.show(); // Update LED met new color
      }

      if (digitalRead(ledPin) == LOW) {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show(); // Update LED met new color
      }
    } else {
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show(); // Update LED met new color
    }
    delay(50);
  }
  oldButtonState = buttonState;
}
