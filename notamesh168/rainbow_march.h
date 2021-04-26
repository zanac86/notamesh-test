#ifndef RAINBOW_MARCH_H
#define RAINBOW_MARCH_H

/* Usage - rainbow_march();
 *  
 * thisindex
 * thisdiff
 * thisrot
 * thisdir
 */


void rainbow_march(uint8_t thisrot,uint8_t thisdiff) {                                           // The fill_rainbow call doesn't support brightness levels
  
  thisindex +=thisrot*thisdir;
  fill_rainbow(leds, NUM_LEDS, thisindex, thisdiff);               // I don't change deltahue on the fly as it's too fast near the end of the strip.

} // rainbow_march()

#endif
