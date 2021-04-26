#ifndef JUGGLE_PAL_H
#define JUGGLE_PAL_H

/* This is adapted from a routine created by Mark Kriegsman */

/*  Usage - juggle_pal();
 *  
 *  thishue
 *  thisfade
 *  numdots
 *  thisbeat
 *  thisbright
 *  thisdiff
 */



void juggle_pal(uint8_t thisdiff,uint8_t thisfade,uint8_t numdots,uint8_t thisbeat) {                                           // Several colored dots, weaving in and out of sync with each other
  
TBlendType currentBlending = LINEARBLEND;                     // NOBLEND or LINEARBLEND
    thisindex = 0;                                              // Reset the hue values.
  fadeToBlackBy(leds, NUM_LEDS, thisfade);
  for( int i = 0; i < numdots; i++) {
    leds[beatsin16(thisbeat+i+numdots,0,NUM_LEDS)] += ColorFromPalette(gCurrentPalette, thisindex , 255, currentBlending);    // Munge the values and pick a colour from the palette
    thisindex += thisdiff;
  }
  
} // juggle_pal()

#endif
