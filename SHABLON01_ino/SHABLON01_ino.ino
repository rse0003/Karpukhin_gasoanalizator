//-слэш - разрез /слэш в гору \слэш с горы
//Маркеры памяти
//void - пустота - космос - ничего - не использует память
//int  - интеджер - целое - память для чисел 12345
//char - символы - буквы
//long - для всего
//------setup - Название
//------() - круглые скобки (SHIFT+9, SHIFT+0) настройки
//------{} - фигурные скобки (SHIFT+Х, SHIFT+Ъ) Начало и конец
//Сложных приказов

#define smokePin A0  // определяем аналоговый выход, к которому подключен датчик [3](https://mcustore.ru/projects/podklyuchenie-datchika-gaza-mq-2-k-arduino/)
int sensorThres = 400;  // пороговое значение АЦП, при котором считаем, что газ есть [3](https://mcustore.ru/projects/podklyuchenie-datchika-gaza-mq-2-k-arduino/)
//----------------------------------------
const byte dynPin = 2; //динамик
//-------------------------------

#include <Wire.h>  // Подключаем библиотеку для работы с аппаратной шиной I2C [1](https://wiki.iarduino.ru/page/OLED_trema/)
#include <iarduino_OLED.h>  // Подключаем библиотеку iarduino_OLED [1](https://wiki.iarduino.ru/page/OLED_trema/)

iarduino_OLED myOLED(0x3C)  // Создаём объект myOLED, указывая адрес дисплея на шине I2C: 0x3C или 0x3D [1](https://wiki.iarduino.ru/page/OLED_trema/)
//---------------------------------
    // Обнаружить газ.
void gas(){
  int analogSensor = analogRead(smokePin);  // считываем значения АЦП с аналогового входа, к которому подключен датчик [3](https://mcustore.ru/projects/podklyuchenie-datchika-gaza-mq-2-k-arduino/)
    Serial.print(analogSensor);  // выводим в порт значение АЦП сигнала с датчика
    // Проверяем, достигнуто ли пороговое значение [3](https://mcustore.ru/projects/podklyuchenie-datchika-gaza-mq-2-k-arduino/)
    if (analogSensor > sensorThres) {  // если значение больше допустимого...
        Serial.println("Gaz!");  // выводим в порт надпись, что газ есть [3](https://mcustore.ru/projects/podklyuchenie-datchika-gaza-mq-2-k-arduino/)
    } else {  // иначе...
        Serial.println("normal");  // выводим в порт надпись, что газа нет [3](https://mcustore.ru/projects/podklyuchenie-datchika-gaza-mq-2-k-arduino/)
    }
    delay(500);  // задержка в 500 миллисекунд [3](https://mcustore.ru/projects/podklyuchenie-datchika-gaza-mq-2-k-arduino/)
}

#include iarduino_RTC time(RTC_DS3231); [2](https://edurobots.org/2017/05/arduino-real-time-clock-ds3231/)
//----------------------------------------------------
    // Подать сигнал при низком заряде аккумулятора.
void signal(){
    tone(dynPin, 100);  // генерируем звук с частотой 100 Гц
    delay(100);  // пауза 500 миллисекунд
    noTone(dynPin);  // выключаем звук
    delay(900);  // снова пауза 500 мс
}
    // Показать вид газа.
void info_gaz(){
  myOLED.print( 123456789 , 0, 7 )  // Выводим целое положительное число начиная с координаты 0x0 [1](https://wiki.iarduino.ru/page/OLED_trema/)
  myOLED.print(-123456789 , 0, 15)  // Выводим целое отрицательное число начиная с координаты 0x15 [1](https://wiki.iarduino.ru/page/OLED_trema/)
  myOLED.print( 123456789 , 0, 23, HEX)  // Выводим целое положительное число начиная с координаты 0x23, в 16-ричной системе счисления [1](https://wiki.iarduino.ru/page/OLED_trema/)
  myOLED.print( 123456789 , 0, 31, OCT )  // Выводим целое положительное число начиная с координаты 0x31, в 8-ричной системе счисления [1](https://wiki.iarduino.ru/page/OLED_trema/)
  myOLED.print(-123.456789, 0, 39)  // Выводим число с плавающей точкой начиная с координаты 0x39, по умолчанию отобразится 2 знака после запятой [1](https://wiki.iarduino.ru/page/OLED_trema/)
  myOLED.print( 123.456789, 0, 47, 3)  // Выводим число с плавающей точкой начиная с координаты 0x47, указывая 3 знака после запятой [1](https://wiki.iarduino.ru/page/OLED_trema/)
  myOLED.print( 123 , 0, 55, BIN)  // Выводим целое положительное число начиная с координаты 0x55, в 2-ичной системе счисления [1](https://wiki.iarduino.ru/page/OLED_trema/) z
  myOLED.print( 123 , 0, 63, 12)  // Выводим целое положительное число начиная с координаты 0x63, в 12-ричной системе счисления [1](https://wiki.iarduino.ru/page/OLED_trema/)
}

void setup() {  
   Serial.begin(9600);  // Устанавливаем скорость порта 9600 бод [3](https://mcustore.ru/projects/podklyuchenie-datchika-gaza-mq-2-k-arduino/)
//------------------------------------------
pinMode(dynPin, OUTPUT);  // настраиваем контакт №2 на выход
//--------------------------------------------------------

}

void loop() {
  
}
