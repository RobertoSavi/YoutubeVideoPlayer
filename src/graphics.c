#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "graphics.h"
#include "vars.h"

void _graphics()
{
    Graphics_drawStringCentered(&g_sContext, (int8_t*) "Now watching:", AUTO_STRING_LENGTH, 84, 20, OPAQUE_TEXT);

    sprintf(string[0], "Time: %3d/%3d", time, timeMax);

    if(mute)
    {
        sprintf(string[1], "Volume:  Muted ");
    }
    else
    {
        sprintf(string[1], "Volume: %3d/100", volume);
    }

    sprintf(string[2], title);

    Graphics_drawStringCentered(&g_sContext, (int8_t*) string[0], 15, 64, 70, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, (int8_t*) string[1], 15, 64, 80, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, (int8_t*) string[2], AUTO_STRING_LENGTH, titlePos, 45, FONT_FMT_UNCOMPRESSED);

    showProgressBar();

    Graphics_drawImage(&g_sContext, &YTLogo, 5, 5);

    if(!playing)
    {
        Graphics_drawImage(&g_sContext, &playIMG, 7, 110);
    }
    else
    {
        Graphics_drawImage(&g_sContext, &pauseIMG, 7, 110);
    }
}

void _titleGraphics()
{
    tRectangle whiteBar = {40, 0, 50, 130};
    GrContextForegroundSet(&g_sContext, 0xffffff);
    GrRectFill(&g_sContext, &whiteBars);
    Graphics_drawStringCentered(&g_sContext, (int8_t*) title, AUTO_STRING_LENGTH, titlePos, 45, FONT_FMT_UNCOMPRESSED);
}

void _menuGraphics(uint8_t menuS)
{
    Graphics_clearDisplay(&g_sContext);

    Graphics_drawStringCentered(&g_sContext, (int8_t*) "Menu:", AUTO_STRING_LENGTH, 84, 20, OPAQUE_TEXT);

    Graphics_drawImage(&g_sContext, &YTLogo, 5, 5);

    Graphics_Rectangle select={9, 45+20*menuS, 119, 55+20*menuS};

    Graphics_fillRectangle(&g_sContext, &select);

    Graphics_drawRectangle(&g_sContext, &select);

    if(menuS==0)
    {
        Graphics_setForegroundColor(&g_sContext, 0xFFFFFF);
        Graphics_setBackgroundColor(&g_sContext, 0xFF0000);
        Graphics_drawStringCentered(&g_sContext, (int8_t*) "Previous video",AUTO_STRING_LENGTH,64, 50, OPAQUE_TEXT);
        Graphics_setForegroundColor(&g_sContext, 0xFF0000);
        Graphics_setBackgroundColor(&g_sContext, 0xFFFFFF);
    }
    else
    {
        Graphics_drawStringCentered(&g_sContext, (int8_t*) "Previous video",AUTO_STRING_LENGTH,64, 50, OPAQUE_TEXT);
    }

    if(menuS==1)
    {
        Graphics_setForegroundColor(&g_sContext, 0xFFFFFF);
        Graphics_setBackgroundColor(&g_sContext, 0xFF0000);
        Graphics_drawStringCentered(&g_sContext, (int8_t*) "Next video",AUTO_STRING_LENGTH,64, 70, OPAQUE_TEXT);
        Graphics_setForegroundColor(&g_sContext, 0xFF0000);
        Graphics_setBackgroundColor(&g_sContext, 0xFFFFFF);
    }
    else
    {
        Graphics_drawStringCentered(&g_sContext, (int8_t*) "Next Video",AUTO_STRING_LENGTH,64, 70, OPAQUE_TEXT);
    }

    if(menuS==2)
    {
        Graphics_setForegroundColor(&g_sContext, 0xFFFFFF);
        Graphics_setBackgroundColor(&g_sContext, 0xFF0000);
        Graphics_drawStringCentered(&g_sContext, (int8_t*) "Lower speed",AUTO_STRING_LENGTH,64, 90, OPAQUE_TEXT);
        Graphics_setForegroundColor(&g_sContext, 0xFF0000);
        Graphics_setBackgroundColor(&g_sContext, 0xFFFFFF);
    }
    else
    {
        Graphics_drawStringCentered(&g_sContext, (int8_t*) "Lower speed",AUTO_STRING_LENGTH,64, 90, OPAQUE_TEXT);
    }

    if(menuS==3)
    {
        Graphics_setForegroundColor(&g_sContext, 0xFFFFFF);
        Graphics_setBackgroundColor(&g_sContext, 0xFF0000);
        Graphics_drawStringCentered(&g_sContext, (int8_t*) "Higher speed",AUTO_STRING_LENGTH,64, 110, OPAQUE_TEXT);
        Graphics_setForegroundColor(&g_sContext, 0xFF0000);
        Graphics_setBackgroundColor(&g_sContext, 0xFFFFFF);
    }
    else
    {
        Graphics_drawStringCentered(&g_sContext, (int8_t*) "Higher speed",AUTO_STRING_LENGTH,64, 110, OPAQUE_TEXT);
    }
}

void _connectionGraphics()
{
    Graphics_drawStringCentered(&g_sContext, (int8_t*) "   Connecting...   ", AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
    Graphics_drawImage(&g_sContext, &YTLogo, 64-16, 15);
}

void _failedGraphics()
{
    Graphics_drawStringCentered(&g_sContext, (int8_t*) "Connection failed", AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
}

void showProgressBar()
{
    tRectangle antiProgressBar = {posBarraX, posBarraY, posBarraX+dimBarraX, posBarraY+dimBarraY};
    tRectangle progressBar = {posBarraX, posBarraY, posBarraX+(int)(((float)time/(float)timeMax)*dimBarraX), posBarraY+dimBarraY};
    GrContextForegroundSet(&g_sContext, 0xffffff);
    GrRectFill(&g_sContext, &antiProgressBar);
    // Initializes progress bar borders
    GrContextForegroundSet(&g_sContext, 0xff0000);
    GrLineDrawH(&g_sContext, posBarraX, posBarraX+dimBarraX, posBarraY);
    GrLineDrawH(&g_sContext, posBarraX, posBarraX+dimBarraX, posBarraY+dimBarraY);
    GrLineDrawV(&g_sContext, posBarraX, posBarraY, posBarraY+dimBarraY);
    GrLineDrawV(&g_sContext, posBarraX+dimBarraX, posBarraY, posBarraY+dimBarraY);
    GrRectFill(&g_sContext, &progressBar);
    GrFlush(&g_sContext);
}
