//This is an example of using Red Pitaya with the Red Pitaya Click Shield and Capacitive Touch Click Board. This example turns on the led on the red pitaya board when the cap touch button is pressed

#include <stdio.h>
#include <stdlib.h>

#include "rp.h"

// Choose a microbus depending on where the click board is
#define MIKROBUS 1    // 1 == Microbus 1, 2 == Microbus 2

#if MIRKOBUS == 1
    #define INT_PIN RP_DIO2_P
#else
    #define INT_PIN RP_DIO4_P
#endif


int main (int argc, char **argv) {
  rp_pinState_t state;

  // Initialization of API
  if (rp_Init() != RP_OK) {
      fprintf(stderr, "Red Pitaya API init failed!\n");
      return EXIT_FAILURE;
  }

  // Configure DIO0_N as input
  rp_DpinSetDirection (INT_PIN, RP_IN);

  while(1){
    // Get the button value
    rp_DpinGetState(INT_PIN, &state);
    if (state == RP_HIGH){
      // Turn the light ON/OFF based on the button value
      rp_DpinSetState(RP_LED0, state);
    }
    else{
      // Turn the light ON/OFF based on the button value
      rp_DpinSetState(RP_LED0, state);
    }


  }

  // Releasing resources
  rp_Release();

  return EXIT_SUCCESS;
}