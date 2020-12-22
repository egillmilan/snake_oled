#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define UP_button          2
#define DOWN_button        3
#define RIGHT_button       4
#define LEFT_button        5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

bool takki;


void setup() {
  pinMode(UP_button, INPUT_PULLUP);     //Activate internal pull-up resistors of atmega328p chip
  pinMode(DOWN_button, INPUT_PULLUP);   //whenever digitalRead() returns low, a button is being
  pinMode(RIGHT_button, INPUT_PULLUP);  //pressed
  pinMode(LEFT_button, INPUT_PULLUP);

  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();

  initialize();
}


void initialize(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(24, 14);
    display.println(F("Yttu a takka,"));
    display.setCursor(20, 34);
    display.println(F("elsku jolavinur"));
    display.display();
}


void hoax_message(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(31, 24);
  display.println(F("Ertu ekki i"));
  display.setCursor(1, 34);
  display.println(F("hugbunadarverkfraedi?"));
  display.display();
  delay(3000);
  
  display.clearDisplay();
  display.setCursor(25, 24);
  display.println(F("Forritadu tha"));
  display.setCursor(13, 33);
  display.println(F("thinn eigin leik!"));
  display.display();
  delay(3000);
  
  display.clearDisplay();
  display.setCursor(1, 45);
  display.println(F("(djok sorri hafdi ekki tima)"));
  display.display();
  delay(1500);
  display.clearDisplay();
  display.display();
}


void loop() {
  if (!digitalRead(2) || !digitalRead(3) || !digitalRead(4) || !digitalRead(5)){
    hoax_message();
  }
}
