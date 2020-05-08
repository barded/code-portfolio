/*
Deandra Bardell
Created: Feb 15 18
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lcd.h"

int set_tod_from_secs(int time_of_day_sec, tod_t *tod){
// Accepts time of day in seconds as an argument and modifies the
// struct pointed at by tod to fill in its hours, minutes,
// etc. fields.  If time_of_day_sec is invalid (negative or larger
// than the number of seconds in a day) does not tod and returns 1 to
// indicate an error. Otherwise returns 0 to indicate success.

  if (time_of_day_sec < 0 || time_of_day_sec > 86400) {  // invalid time, error
    return 1;
  }

  tod->hours = time_of_day_sec / 3600;  // rounds down to hour we are currently in
  if (tod->hours == 0) {               //   12 am case
    tod->hours = 12;
    tod->ispm = 0;
  } else if (tod->hours > 12) {    // 12-hour time, not 24-hour time
    tod->hours = tod->hours - 12;    //  subtract 12 to get 12-hour time
    tod->ispm = 1;                    //  set pm
  } else if (tod->hours == 12) {       //  case for 12pm
    tod->ispm = 1;
  }  else{                            //  else set to am
    tod->ispm = 0;
  }

  short extrahour = time_of_day_sec % 3600;   //  excess amount after finding the hour
  tod->minutes = extrahour / 60;            //  round down to minute we're in
  tod->seconds = extrahour % 60;         //  excess after dividing is seconds

  return 0;

}

int set_display_bits_from_tod(tod_t tod, int *display){
// Accepts a tod and alters the bits in the int pointed at by display
// to reflect how the LCD clock should appear. If any fields of tod
// are negative or too large (e.g. bigger than 12 for hours, bigger
// than 59 for min/sec), no change is made to display and 1 is
// returned to indicate an error. Otherwise returns 0 to indicate
// success.
//
// May make use of an array of bit masks corresponding to the pattern
// for each digit of the clock to make the task easier.

  if (tod.hours > 12 || tod.minutes > 59 || tod.seconds > 59 || tod.ispm > 1) {
    return 1;              // invalid time, error
  }

  int arr[11];         //  array to store bit sequences for 0-9 and blank
  arr[0] = 0b0111111;
  arr[1] = 0b0000110;
  arr[2] = 0b1011011;
  arr[3] = 0b1001111;
  arr[4] = 0b1100110;
  arr[5] = 0b1101101;
  arr[6] = 0b1111101;
  arr[7] = 0b0000111;
  arr[8] = 0b1111111;
  arr[9] = 0b1101111;
  arr[10] = 0b0000000;    //   blank

  int state = 0b0000000000000000000000000000000000000; // initial lcd bits
  int am = 0b01;
  int pm = 0b10;

  if (tod.ispm) {         // set am or pm
    state = state | pm;
  } else{
    state = state | am;
  }

  state = state << 7;     //  left shift for first clock element


  if (tod.hours > 9) {        //  if the time is 10,11,or 12
    state = state | arr[1];       //     insert 1 with inclusive or

    state = state << 7;

    state = state | arr[tod.hours % 10];   //  to get ones place hour

  } else{                      // hour is 1-9
    state = state  | arr[10];    //  tens element should be blank
    state = state  << 7;

    state = state | arr[tod.hours];     // insert hour bits
  }

  state = state << 7;              //   insert minutes block
  state = state | arr[tod.minutes / 10];   //  tens digit of minutes, rounds down
  state = state << 7;
  state = state | arr[tod.minutes % 10];   //   ones digit of minutes

  *display = state ;     //    set display

  return 0;
}


int lcd_update(){
// Examines the TIME_OF_DAY_SEC global variable to determine hour,
// minute, and am/pm.  Sets the global variable LCD_DISPLAY_PORT bits
// to show the proper time.  If TIME_OF_DAY_SEC appears to be in error
// (to large/small) makes no change to LCD_DISPLAY_PORT and returns 1
// to indicate an error. Otherwise returns 0 to indicate success.
//
// Makes use of the set_tod_from_secs() and
// set_display_bits_from_tod() functions.

  tod_t temp;   // temp variable for time

  int result = set_tod_from_secs(TIME_OF_DAY_SEC, &temp);
  if (result == 1) {
    return 1;     // invalid time, error
  }

  result = set_display_bits_from_tod(temp, &LCD_DISPLAY_PORT);

  if (result == 1) {
    return 1;        //   invalid time, error
  } else{
    return 0;     //  no errors
  }
}
