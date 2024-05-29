#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "menu.h"
#include "vars.h"
#include "handlers.h"

void _menuFunc(uint8_t menuS)
{
    switch(menuS)
    {
        case 0:
        {
            previousVideo();
            break;
        }

        case 1:
        {
            nextVideo();
            break;
        }

        case 2:
        {
            lowerSpeed();
            break;
        }

        case 3:
        {
            higherSpeed();
            break;
        }
    }
}

void previousVideo()
{
    char str[7]="prev.";
    sendUART(str);
}

void nextVideo()
{
    char str[7]="next.";
    sendUART(str);
}

void lowerSpeed()
{
    char str[7]="lSpee.";
    sendUART(str);
}

void higherSpeed()
{
    char str[7]="hSpee.";
    sendUART(str);
}
