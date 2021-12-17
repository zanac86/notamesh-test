//
#ifndef CONFETTI_PAL_H
#define CONFETTI_PAL_H

/* This is adapted from the confetti routine created by Mark Kriegsman */

/*  Usage - confetti_pal();
 *   
 *  thisfade
 *  thisdelay
 *  currentPalette and targetPalette
 *  thisdiff
 *  thisindex
 *  thisinc
 *  thisbright
 */


void confetti_pal() {                                                                                               // random colored speckles that blink in and fade smoothly
  if ((EndLed - StartLed)>=10) {
    fadeToBlackBy(&leds[StartLed], (EndLed - StartLed), thisfade);        //Задухание светодиодов на значение thisfade 
    #if MAX_LEDS < 255
      uint8_t  pos = StartLed+ random8 (EndLed - StartLed); 
    #else
      uint16_t pos = StartLed+ random16(EndLed - StartLed);
    #endif
    leds[pos] = ColorFromPalette(gCurrentPalette, thisindex + random8(thisdiff)/4 , 255, currentBlending);     // Munge the values and pick a colour from the palette
    thisindex = thisindex + thisinc;                                                                                  // base palette counter increments here.
  }
} // confetti_pal()

#endif
