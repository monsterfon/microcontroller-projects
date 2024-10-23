//This is an example of using Red Pitaya with the Red Pitaya Click Shield and Motion Click Board. This example measures the motion in front of the sensor and outputs messages through the command prompt


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rp.h"

// Choose a microbus depending on where the click board is
#define MIKROBUS 1    // 1 == Microbus 1, 2 == Microbus 2

#if MIRKOBUS == 1
    #define INT_PIN RP_DIO2_P    // Microbus 1
#else
    #define INT_PIN RP_DIO4_P    // Microbus 2
#endif


int main (int argc, char **argv) {
    rp_pinState_t state;

    // Initialization of API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    rp_DpinSetDirection (INT_PIN, RP_IN);

    while(1){
        // Get button value
        rp_DpinGetState(INT_PIN, &state);

        if (state == RP_HIGH){
            // Turn LED 0 ON if no motion is detected
            rp_DpinSetState(RP_LED0, RP_HIGH);
            printf("There is no motion.\n");
        }
        else{
            // Turn LED 0 OFF if motion is detected
            rp_DpinSetState(RP_LED0, RP_LOW);
            printf("There is motion.\n");
        }
        usleep(1000000);

    }

    // Releasing resources
    rp_Release();

    return EXIT_SUCCESS;
}
