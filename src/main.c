#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "vars.h"
#include "graphics.h"
#include "init.h"

int main()
{
    _hwInit();

    timeMax = atoi(duration); // Convert string to integer

    //_graphics();

    if(0)
    {
        return 1;
    }

    while(1)
    {
        Interrupt_enableSleepOnIsrExit();
        PCM_gotoLPM0InterruptSafe();
    }
}
