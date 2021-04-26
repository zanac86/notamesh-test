/*
 * notamesh - IR controlled 'synchronized' led lighting effects using FastLED across multiple Arduino controllers.
 * 
 *       By: Andrew Tuline
 *     Date: October, 2018
 *      URL: www.tuline.com
 *    Email: atuline@gmail.com
 *     Gist: https://gist.github.com/atuline
 *   GitHub: https://github.com/atuline
 * Pastebin: http://pastebin.com/u/atuline
 *  Youtube: https://www.youtube.com/user/atuline/videos
 * 
 */

//  Переделка и дополнение
//  Январь 2020
//  Бикин Дмитрий
//  d.bikin@bk.ru
//  Обсуждение проекта https://mysku.ru/blog/aliexpress/68990.html
//  Проект делается на чистом энтузиазме и в свободное время
//  Яндекс кошелек     410011655180757

/////////////////////////////////////////////////////////////////////////////////
///                               Настройка
/////////////////////////////////////////////////////////////////////////////////

#define KOL_LED       30                                  // Сколько светодиодов в гирлянде при первом включении

//////////////////////////////////////////////////////////////////////////////// Параметры гирлянды
////
#define COLOR_ORDER   GRB                                 //Очередность цвета светодиодов  RGB или GRB
#define CHIPSET       WS2812B                              //Тип микросхемы светодиодов в гирлянде

#define LED_DT        2                                   // Номер пина куда подключена гирлянда
//#define LED_CK      11                                  // Номер пина для подключения тактов, применяется для светодиодов WS2801 или APA102
                                                          //ЕСЛИ НЕ ИСПОЛЬЗУЕТСЯ ЗАКОМЕНТИРОВАТЬ

#define POWER_V       5                                   // напряжение блока питания в Вольтах
#define POWER_I       2000                                // Ток блока питания в милиАмперах


//#define GLITER_ON     0                                    // Включить мерание при запуске
#define DEMO_MODE     2                                    // Режим при включении 
                                                           // 0 - 0 программа
                                                           // 1 - демо режим с последовательным перебором эффектов
                                                           // 2 - демо режим с случайнвым перебором эффектов

#define DEMO_TIME     20                                   // Время отображение эффекта в демо режиме в секундах

#define DIRECT_TIME   10                                   // Через сколько секунд менять направление если 0 - не меняем
#define PALETTE_TIME  30                                   // Через сколько секунд менять палитру если 0 - не меняем

#define LOG_ON        0                                    // Включить лог  1 - включить лог



#if KOL_LED >255
  #error "Kоличество светодиодов в гирлянде не должно превышать 255"
#endif 


/*------------------------------------------------------------------------------------------
--------------------------------------- Начало переменным ---------------------------------
------------------------------------------------------------------------------------------*/


#define qsubd(x, b)  ((x>b)?255:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.

#define NOTAMESH_VERSION 103                                  // Just a continuation of seirlight and previously aalight.

#include "FastLED.h"                                          // https://github.com/FastLED/FastLED


#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#if IR_ON == 1
  int RECV_PIN = PIN_IR;
  IRrecv irrecv(RECV_PIN);
  decode_results results;
#endif

// Serial definition
#define SERIAL_BAUDRATE 57600                                 // Or 115200.


// Fixed definitions cannot change on the fly.

#define MAX_LEDS  KOL_LED



// Initialize changeable global variables.
#define  NUM_LEDS KOL_LED                                      // Number of LED's we're actually using, and we can change this only the fly for the strand length.

#define max_bright  255                                       // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[MAX_LEDS];                                   // Initialize our LED array. We'll be using less in operation.

 CRGBPalette16 gCurrentPalette;                               // Use palettes instead of direct CHSV or CRGB assignments
 CRGBPalette16 gTargetPalette;                                // Also support smooth palette transitioning


extern const TProgmemRGBGradientPalettePtr gGradientPalettes[]; // These are for the fixed palettes in gradient_palettes.h
extern const uint8_t gGradientPaletteCount;                     // Total number of fixed palettes to display.
uint8_t gCurrentPaletteNumber = 0;                              // Current palette number from the 'playlist' of color palettes
uint8_t currentPatternIndex = 0;                                // Index number of which pattern is current

uint8_t ledMode = 0;                                            // Starting mode is typically 0. Change INITMODE if you want a different starting mode.
#define maxMode 17                                              // Maximum number of modes.

#define demotime DEMO_TIME                                      // Set the length of the demo timer.



// Generic/shared routine variables ----------------------------------------------------------------------
uint8_t palchg = 3;                                            // 0=no change, 1=similar, 2=random
uint16_t thisdelay = 10;                                       // Standard delay
int8_t  thisdir = 1;                                           // Standard direction is either -1 or 1. Used as a multiplier rather than boolean.
uint8_t thisindex = 0;                                         // Standard palette index

long summ=0;



// Display functions -----------------------------------------------------------------------

// Support functions
#include "addglitter.h"

// Display routines
#include "confetti_pal.h"
#include "gradient_palettes.h"
#include "juggle_pal.h"
#include "matrix_pal.h"
#include "plasma.h"
#include "rainbow_march.h"
#include "rainbow_beat.h"
#include "blendwave.h"
#include "fire.h"

/*------------------------------------------------------------------------------------------
--------------------------------------- Start of code --------------------------------------
------------------------------------------------------------------------------------------*/




//-------------------OTHER ROUTINES----------------------------------------------------------
void strobe_mode(uint8_t newMode, bool mc){                   // mc stands for 'Mode Change', where mc = 0 is strobe the routine, while mc = 1 is change the routine

  if(mc) {
    fill_solid(leds,NUM_LEDS,CRGB(0,0,0));                    // Clean up the array for the first time through. Don't show display though, so you may have a smooth transition.
    #if LOG_ON == 1
      Serial.print("Mode: "); 
      Serial.println(newMode);
      Serial.println(millis());
    #endif
  }
 
  switch (newMode) {                                          // First time through a new mode, so let's initialize the variables for a given display.

    case  0: if(mc) {thisdelay=10;} blendwave(); break; 
    case  1: if(mc) {thisdelay=10;} rainbow_beat(); break;
    case  2: if(mc) {thisdelay=10;} juggle_pal(64,16,2,8); break;
    case  3: if(mc) {thisdelay=40;} matrix_pal(0,200,6); break;
    case  4: if(mc) {thisdelay=50;} matrix_pal(1,100,10); break;
    case  5: if(mc) {thisdelay=10;} plasma(11,23,4,18); break;
    case  6: if(mc) {thisdelay=10;} rainbow_march(1,1); break;
    case  7: if(mc) {thisdelay=10;} rainbow_march(2,10); break;
    case  8: if(mc) {thisdelay=10;} plasma(23,15,6,7); break;
    case  9: if(mc) {thisdelay=20;} confetti_pal(32,2,1); break;
    case 10: if(mc) {thisdelay=30;} matrix_pal(0,50,0); break;
    case 11: if(mc) {thisdelay=20;} confetti_pal(64,8,2); break;
    case 12: if(mc) {thisdelay=10;} plasma(8,7,9,13); break;
    case 13: if(mc) {thisdelay=10;} juggle_pal(20,32,4,12); break;
    case 14: if(mc) {thisdelay=10;} rainbow_march(1,5); break;
    case 15: if(mc) {thisdelay=10;} plasma(11,17,20,23); break;
    case 16: if(mc) {thisdelay=20;} confetti_pal(64,1,1); break;
    case 17: if(mc) {thisdelay=10;} fire(); break;
  } // switch newMode
  
} // strobe_mode()



void demo_check(){
  
#if DEMO_MODE >0                                                    // Is the demo flag set? If so, let's cycle through them.
    uint8_t secondHand = (millis() / 1000) % (maxMode*demotime);  // Adjust for total time of the loop, based on total number of available modes.
    static uint8_t lastSecond = 99;                               // Static variable, means it's only defined once. This is our 'debounce' variable.
    
    if (lastSecond != secondHand) {                               // Debounce to make sure we're not repeating an assignment.
      lastSecond = secondHand;
        if(secondHand%demotime==0) {                              // Every 10 seconds.
          #if DEMO_MODE ==2                                                    // Is the demo flag set? If so, let's cycle through them.
            ledMode = random8(0,maxMode); 
          #else
            if (ledMode >= maxMode) ledMode = 0;          // демо 1 
                    else ledMode++;
          #endif
          // meshwait();
          gCurrentPaletteNumber = random8(0,gGradientPaletteCount); 
          gTargetPalette = gGradientPalettes[gCurrentPaletteNumber];
          strobe_mode(ledMode,1);                                 // Does NOT reset to 0.
      } // if secondHand
    } // if lastSecond

  #endif
  
} // demo_check()

void setup() {

#if LOG_ON == 1
  Serial.begin(SERIAL_BAUDRATE);                                                  // Setup serial baud rate

  Serial.println(" "); Serial.println("---SETTING UP---");
#endif
  delay(1000);                                                                    // Slow startup so we can re-upload in the case of errors.

  LEDS.setBrightness(max_bright);                                                 // Set the generic maximum brightness value.
  #if LED_CK
    LEDS.addLeds<CHIPSET, LED_DT, LED_CK, COLOR_ORDER>(leds, MAX_LEDS);
  #else 
    LEDS.addLeds<CHIPSET, LED_DT, COLOR_ORDER >(leds, MAX_LEDS);                   //Для светодиодов WS2812B
  #endif
  set_max_power_in_volts_and_milliamps(POWER_V, POWER_I);                         //Настройка блока питания

  random16_set_seed(4832);                                                        // Awesome randomizer of awesomeness
  random16_add_entropy(analogRead(2));
  int ranstart = random16();

    ledMode = 0;

  gCurrentPalette = CRGBPalette16(CRGB::Black);
  gTargetPalette = (gGradientPalettes[0]);

  strobe_mode(ledMode, 1);                                                        // Initialize the first sequence

  #if LOG_ON == 1
    Serial.println("---SETUP COMPLETE---");
  #endif
} // setup()

//------------------MAIN LOOP---------------------------------------------------------------
void loop() {

 
  demo_check();                                                               // If we're in demo mode, check the timer to see if we need to increase the strobe_mode value.

  EVERY_N_MILLISECONDS(50) {                                                  // Smooth palette transitioning runs continuously.
    uint8_t maxChanges = 24; 
      nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette, maxChanges);  
  }

  #if PALETTE_TIME>0
    EVERY_N_SECONDS(PALETTE_TIME) {                                            // If selected, change the target palette to a random one every PALETTE_TIME seconds.
      if (palchg==3) {
        gCurrentPaletteNumber = addmod8(gCurrentPaletteNumber, 1, gGradientPaletteCount);
      }
      gTargetPalette = gGradientPalettes[gCurrentPaletteNumber];                // We're just ensuring that the gTargetPalette WILL be assigned.
  #if LOG_ON == 1
    Serial.println("Palette1");
  #endif
    }
  #endif
  
  #if DIRECT_TIME > 0
    EVERY_N_SECONDS(DIRECT_TIME) {                                            // Меняем направление
      thisdir = thisdir*-1;  
  #if LOG_ON == 1
    Serial.println("thisdir");
  #endif
    }
  #endif
  
  EVERY_N_MILLIS_I(thistimer, thisdelay) {                                    // Sets the original delay time.
    thistimer.setPeriod(thisdelay);                                           // This is how you update the delay value on the fly.
    strobe_mode(ledMode, 0);                                                  // Strobe to display the current sequence, but don't initialize the variables, so mc=0;
  }
  #ifdef GLITER_ON
    addglitter(10);                                                           // If the glitter flag is set, let's add some.
  #endif
  
   FastLED.show();                                                             // Power managed display of LED's.
  
} // loop()
