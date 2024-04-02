#include <TOTP.h>
#include <sha1.h>
uint8_t hmacKey[] = {0x74, 0x65, 0x73, 0x74};
TOTP totp = TOTP(hmacKey, 4);

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10 // Number of snowflakes in the animation example

#include <microDS3231.h>
MicroDS3231 rtc;


#include <GyverPower.h>


void setup() {
  Serial.begin(9600);

  rtc.setTime(BUILD_SEC + 10, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);

  pinMode(3, INPUT_PULLUP);
  power.autoCalibrate();
  power.setSleepMode(POWERDOWN_SLEEP);
  attachInterrupt(1, wakeup, FALLING);
  //attachInterrupt(digitalPinToInterrupt(12), wakeup, FALLING);
  
  // if(!display.begin(SSD1306_SWITCHCAPVCC)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;); // Don't proceed, loop forever
  // }
  // display.clearDisplay();
  // display.setTextSize(3);             // Normal 1:1 pixel scale
  // display.setTextColor(SSD1306_WHITE);        // Draw white text
  // display.setCursor(0,0);             // Start at top-left corner
  // display.println(F("123456"));
  // display.display();
  // delay(2000); // Pause for 2 seconds

  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(3);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  char* newCode = totp.getCode(rtc.getUnix(0) - 10800);
  display.println(newCode);
  display.display();

  delay(5000);

  display.clearDisplay();
  display.display();
  power.sleep(SLEEP_FOREVER);

}

void wakeup()
{
  power.wakeUp();  

}
