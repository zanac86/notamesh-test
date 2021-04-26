# notamesh

Автор оригинала https://github.com/atuline

Но самих проектов на github нет. Есть другие.

## notamesh4ESP32

Проект для 8266.

Работает и на `Wemos D1 mini` и на `NodeMCU v3`. Лучше выбирать `Generic 8266 Module`.

В проекте оторваны 4 кнопки.

```c++
//                          Настройка FastLED
#define MAX_LEDS      60         // Максимальное количество светодиодов,  очень влияет на память 
#define COLOR_ORDER   GRB         //Очередность цвета светодиодов  RGB или GRB
#define CHIPSET       WS2812B     //Тип микросхемы светодиодов в гирлянде
#define LED_DT        2          // Номер пина куда подключена гирлянда
```

Раньше был выбор пина `D4`. Но теперь вот так.

## notamesh168

Проекты для AVR. Лента подключена к `D10`.

### notamesh168_micro

В Arduino выбирать плату `Arduino Leonardo`. Прошивается черещ USB.

### notamesh168_pro_mini

В Arduino выбирать плату `Arduino Pro Mini`.

Для программирования нужен USB-UART переходник.

| USB-UART | Pro Mini |
|----------|----------|
| +5       | VCC      |
| GND      | GND      |
| TXD      | RX1      |
| RXD      | TX0      |

После сборки проекта, в самом начале прошивки надо успеть нажать кнопку reset на плате.
