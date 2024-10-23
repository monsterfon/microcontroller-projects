//This is an example of using Red Pitaya with the Red Pitaya Click Shield and iso adc 3 click board. This example measures the voltage +-250mV with a 12 bit ADC and a great isolation amplifier 


#include "rp_hw.h" //you must include this manually using scp linux command
//you can find it here https://github.com/RedPitaya/RedPitaya/blob/master/rp-api/api-hw/include/rp_hw.h
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <i2c/smbus.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#define FAIL "\033[91m[FAIL]\e[0m"

// Function to initialize the I2C communication
int setup() {
    int deviceAddress = 0b1001101;
    const char *filename = "/dev/i2c-0";
    
    int fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("Failed to open I2C %s\n", FAIL);
        printf("I2C %s\n\n", FAIL);
        return -1;
    }

    if (ioctl(fd, I2C_SLAVE, deviceAddress) < 0) {
        printf("Setting I2C_SLAVE %s\n", FAIL);
        printf("I2C %s\n\n", FAIL);
        close(fd);
        return -1;
    }

    return fd;
}

//convert the adc value to the correct voltage
double convertToVoltage(int adcValue) {
    int adcRange = 3174;
    double voltageRange = 0.62;

    double voltage = (adcValue / (double)adcRange) * voltageRange - 0.31;
    printf("\nADC Voltage: %.4lf V\n", voltage);
    
    //OVERVOLTAGE: if you keepthis up you can damage the device
    if (voltage < -0.25 || voltage > 0.25) {
        printf("\nWarning: This device is rated for a voltage range of -+250mV\n\n");
    }

    return voltage;
}


// Function to read data from MCP3221 over I2C
int readValue(int fd) {
    char buf[4]; // Buffer for MCP3221 communication
    memset(buf, 0, 4);
    int x = read(fd, buf, 2);
    if (x != 2) {
        printf("Read operation failed %d %s\n", x, FAIL);
        return -1;
    } else {
        int value = ((buf[0] & 0x0F) << 8) | buf[1];
        printf("\nADC Value: %d\n", value);
        printf("\nDevice supports +-250mV at 22.3 ksps. Connect 5V DC to Vext \n");
        return value;
    }
}

int main(int argc, char **argv) {
    // Configure I2C
    int fd = setup();
    if (fd < 0) {
        return -1;
    }

    // Read data from ISO ADC 3 CLICK
    int adcValue = readValue(fd);
    // Measure the voltage
    double voltage = convertToVoltage(adcValue);

    // Close the I2C file
    close(fd);
    return (adcValue < 0) ? -1 : 0;
}
