#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// SPI OLED пины (замени при необходимости)
#define OLED_MOSI   11
#define OLED_CLK    13
#define OLED_DC     8
#define OLED_CS     10
#define OLED_RESET  9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

const int mq2Pin = A0;       // Аналоговый вход MQ-2
const int buzzerPin = 3;     // Пищалка
const int buttonPin = 2;     // Кнопка

const int dangerThreshold = 30;  // Порог опасного уровня (пример)
bool alarmOn = false;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Gas Monitor");
  display.display();
  delay(2000);
}

void loop() {
  int sensorValue = analogRead(mq2Pin);

  Serial.print("MQ2 sensor: ");
  Serial.println(sensorValue);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("CO2: ");
  display.print(sensorValue);
  display.println(" ppm");

  if(sensorValue > dangerThreshold) {
    alarmOn = true;
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println("WARNING! High CO2");
    tone(buzzerPin, 1000);
  } else {
    alarmOn = false;
    noTone(buzzerPin);
  }

  display.display();

  if(digitalRead(buttonPin) == LOW) {
    alarmOn = false;
    noTone(buzzerPin);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Alarm reset");
    display.display();
    delay(2000);
  }

  delay(500);
}
