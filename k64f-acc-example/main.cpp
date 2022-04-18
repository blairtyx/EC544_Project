/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

/* mbed-fest hands-on session sample 2
 * bare-metal blinky with classic style code
 */

#include "mbed.h"
#include "FXOS8700Q.h"

#define threshold 45.0

I2C                     i2c( PTE25, PTE24 );
FXOS8700QAccelerometer  acc( i2c, FXOS8700CQ_SLAVE_ADDR1 );

DigitalOut led_x( LED1 );
DigitalOut led_y( LED2 );
// DigitalOut led_z( LED3 );

int main(void)
{
    float x, y;

    // Enable the accelerometer
    acc.enable();

    while (true) {

        // Fetch 3-axis
        acc.getX( x );
        acc.getY( y );
        // acc.getZ( z );

        // LED on/off
        if ( x > 0 )
            led_x = 1;
        else
            led_x = 0;

        if ( y > 0 )
            led_y = 1;
        else
            led_y = 0;

        // if ( z > 0 )
        //     led_z = 1;
        // else
        //     led_z = 0;

        float th, xx, yy;
        th = threshold*(-1);
        xx = x*90.0;
        yy = y*90.0;

        // Output
        if (yy > th || xx > threshold || xx < th)
            printf("Position Warning!");
        else
            printf("Good Posture.");

        printf("X:%6.1f, Y:%6.1f\r\n", xx, yy);
        wait_ns(10000);
    }
}
