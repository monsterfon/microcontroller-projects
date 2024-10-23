//This is an example of using Red Pitaya with the Red Pitaya Click Shield and Light Click Board. This example measures the light intensity and outputs messages through the command prompt


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rp.h"

// Choose a microbus depending on where the click board is
#define MIKROBUS 1    // 1 == Microbus 1, 2 == Microbus 2

#if MIRKOBUS == 1
    #define AIN_PIN 0    // Microbus 1
#else
    #define AIN_PIN 1    // Microbus 2
#endif


int main (int argc, char **argv) {

    // Initialization of API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    while(1){

        float value;

        // Get light value
        rp_AIpinGetValue(AIN_PIN, &value);
        printf("Measured voltage on AI[%i] = %1.2fV\n", 0, value);

        if (value >= 2.0){
            // Turn ON an LED based on the measured light intensity
            rp_DpinSetState(RP_LED0, RP_HIGH);
            printf("There is LIGHT\n");
        }
        else if (value <= 0.5){
            // Turn OFF an LED based on the measured light intensity
            rp_DpinSetState(RP_LED0, RP_LOW);
            printf("There is no light\n");
        }
        else{
            printf("There is some light\n");
        }
        usleep(1000000);
    }

    // Releasing resources
    rp_Release();

    return EXIT_SUCCESS;
}