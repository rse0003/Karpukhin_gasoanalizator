#include <Wire.h>  // Подключаем библиотеку для I2C
#include <Adafruit_GFX.h>  // Графическая библиотека для дисплея
#include <Adafruit_SSD1306.h>  // Библиотека для OLED-дисплея SSD1306

// Определяем параметры экрана
#define SCREEN_WIDTH 128  // Ширина дисплея в пикселях
#define SCREEN_HEIGHT 64  // Высота дисплея в пикселях
#define OLED_RESET    -1  // Сброс дисплея не используется

// Создаем объект дисплея
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Определяем пины
#define MQ2_PIN A0         // Пин для считывания данных с датчика газа MQ2
#define BATTERY_PIN A1     // Пин для мониторинга напряжения батареи
#define BUZZER_PIN 3       // Пин для подключения пьезодинамика
#define BUTTON_PIN 2       // Пин кнопки включения/выключения

// Переменные состояния
bool deviceOn = false;             // Включено ли устройство
unsigned long lastButtonPress = 0; // Время последнего нажатия кнопки
const int co2Threshold = 400;      // Порог тревоги по концентрации газа

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);           // Устанавливаем пин бузера как выход
  pinMode(BUTTON_PIN, INPUT_PULLUP);     // Кнопка с подтяжкой к питанию
  pinMode(MQ2_PIN, INPUT);               // Вход с датчика газа
  pinMode(BATTERY_PIN, INPUT);           // Вход для измерения напряжения батареи

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), toggleDevice, FALLING); // Прерывание на кнопку

  // Инициализация дисплея
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Если дисплей не инициализировался — зависаем
  }

  // Приветственное сообщение на экране
  display.clearDisplay();      // Очистить дисплей
  display.setTextSize(1);      // Размер текста
  display.setTextColor(SSD1306_WHITE); // Цвет текста — белый
  display.setCursor(0,0);      // Позиция курсора
  display.println("ГАЗОАНАЛИЗАТОР"); // Вывод текста
  display.display();           // Отображение на экране
  delay(2000);                 // Задержка 2 секунды
}

void loop() {
  if (!deviceOn) {       // Если устройство выключено
    displayOff();        // Показать сообщение и не делать ничего
    return;
  }

  int gasValue = analogRead(MQ2_PIN);  // Считываем значение с датчика газа
  float batteryVoltage = analogRead(BATTERY_PIN) * (5.0 / 1023.0) * 2; // Напряжение батареи (через делитель)

  display.clearDisplay();         // Очищаем дисплей
  display.setCursor(0, 0);        // Устанавливаем курсор в начало
  display.print("CO2 уровень: "); // Текст "CO2 уровень:"
  display.println(gasValue);      // Выводим значение газа

  display.setCursor(0, 20);           // Следующая строка
  display.print("Батарея: ");         // Текст "Батарея:"
  display.print(batteryVoltage, 1);   // Напряжение батареи с одной цифрой после запятой
  display.println(" В");              // Единицы измерения

  if (gasValue > co2Threshold) {      // Если газ превышает порог
    display.setCursor(0, 40);         // Следующая строка
    display.println("ОПАСНО: Газ!!!");// Предупреждение
    tone(BUZZER_PIN, 1000);           // Включаем звук 1 кГц
  } else {
    noTone(BUZZER_PIN);               // Иначе выключаем звук
  }

  if (batteryVoltage < 3.3) {         // Если напряжение батареи ниже нормы
    display.setCursor(0, 52);         // Строка предупреждения
    display.println("НИЗКИЙ ЗАРЯД!"); // Предупреждение о батарее
    tone(BUZZER_PIN, 500);            // Включаем звук 500 Гц
  }

  display.display();        // Обновляем экран
  delay(500);               // Задержка 0.5 секунды
}

void toggleDevice() {
  unsigned long now = millis();             // Получаем текущее время
  if (now - lastButtonPress > 500) {        // Защита от дребезга кнопки
    deviceOn = !deviceOn;                   // Меняем состояние устройства
    if (!deviceOn) {                        // Если выключено
      noTone(BUZZER_PIN);                   // Выключаем звук
      displayOff();                         // Показать экран выключения
    }
    lastButtonPress = now;                  // Сохраняем время нажатия
  }
}

void displayOff() {
  display.clearDisplay();                   // Очищаем дисплей
  display.setCursor(0, 0);                  // Устанавливаем курсор
  display.println("Устройство выключено");  // Сообщение о выключении
  display.display();                        // Обновляем экран
  delay(100);                               // Короткая задержка
}
