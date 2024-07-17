#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
//#include <cstdlib>
#include "handlers.h"
#include "vars.h"
#include "graphics.h"
#include "menu.h"
#include <stdlib.h>  // For strtoul
#include <stdint.h>  // For uint16_t
#include <errno.h>   // For errno

void sendUART(char *str)
{
    while (*str != '\0')
    {
        UART_transmitData(EUSCI_A2_BASE, *str);
        str++;
    }
}

void updateTimerForPlaybackSpeed(uint8_t speed)
{
    uint16_t divider;

    // Determine the clock divider based on the speed
    switch ((int) (speed))
    {
    case 25:  // 0.25x speed
        divider = TIMER_A_CLOCKSOURCE_DIVIDER_8;
        break;
    case 50:  // 0.50x speed
        divider = TIMER_A_CLOCKSOURCE_DIVIDER_4;
        break;
    case 75:  // 0.75x speed
        divider = TIMER_A_CLOCKSOURCE_DIVIDER_3;
        break;
    case 100: // 1.00x speed
        divider = TIMER_A_CLOCKSOURCE_DIVIDER_2;
        break;
    case 125: // 1.25x speed
        divider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
        break;
    case 150: // 1.50x speed
        divider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
        break;
    case 175: // 1.75x speed
        divider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
        break;
    case 200: // 2.00x speed
        divider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
        break;
    default:
        divider = TIMER_A_CLOCKSOURCE_DIVIDER_2; // Default to normal speed
    }

    // Update timer configuration with the new divider
    const Timer_A_ContinuousModeConfig contConfig = {
    TIMER_A_CLOCKSOURCE_ACLK,           // ACLK Clock Source
            divider,                            // Use the calculated divider
            TIMER_A_TAIE_INTERRUPT_ENABLE,      // Enable Overflow ISR
            TIMER_A_DO_CLEAR };

    // Reconfigure Continuous Mode for all timers
    Timer_A_configureContinuousMode(TIMER_A0_BASE, &contConfig);

    // Restart the timers
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);
}

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler()
{
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if (status & ADC_INT1)
    {
        /* Store ADC14 conversion results */
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        /* compare the conversion results with given values to
         * enable the interrupt */
        if (resultsBuffer[0] > 15000)
        {
            if (timeDelay && menuOpen == 0)
            {
                if (time <= timeMax - 10)
                {
                    time += 10;
                }
                else
                {
                    time = timeMax;
                    playing = 0;
                    Interrupt_disableInterrupt(INT_TA0_N);
                    _graphics();
                }

                timeDelay = 0;
                char str[10] = "timePlus.";
                sendUART(str);
                _graphics();
            }
        }
        else if (resultsBuffer[0] < 1000)
        {
            if (timeDelay && menuOpen == 0)
            {
                if (time >= 10)
                {
                    time -= 10;
                }
                else
                {
                    time = 0;
                }

                timeDelay = 0;
                char str[11] = "timeMinus.";
                sendUART(str);
                _graphics();
            }
        }

        if (resultsBuffer[1] > 15000)
        {
            if (volumeDelay && menuOpen == 0)
            {
                if (mute)
                {
                    mute = 0;
                    volume = volumeMute;
                }

                if (volume <= 95)
                {
                    volume += 5;
                }
                else
                {
                    volume = 100;
                }

                volumeDelay = 0;
                char str[7] = "volUp.";
                sendUART(str);
                _graphics();
            }
            else if (menuOpen && menuSelect > 0 && volumeDelay)
            {
                menuSelect--;
                --volumeDelay;
                _menuGraphics(menuSelect);
            }
        }
        else if (resultsBuffer[1] < 1000)
        {
            if (volumeDelay && menuOpen == 0)
            {
                if (mute)
                {
                    mute = 0;
                    volume = volumeMute;
                }

                if (volume >= 5)
                {
                    volume -= 5;
                }
                else
                {
                    volume = 0;
                }

                volumeDelay = 0;
                char str[9] = "volDown.";
                sendUART(str);
                _graphics();
            }
            else if (menuOpen && menuSelect < 3 && volumeDelay)
            {
                menuSelect++;
                volumeDelay = 0;
                _menuGraphics(menuSelect);
            }
        }
    }
}

//Handler for the joystick button
void PORT4_IRQHandler()
{
    //Clear of the interrupt flag and disable the interrupt
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);
    Interrupt_disableInterrupt(INT_PORT4);

    if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1) == GPIO_INPUT_PIN_LOW)
    {
        if (!menuOpen)
        {
            //If the menu is closed handle the playing of the video
            if (playing)
            {
                //If the video is playing pause it, disable the timer that increases the video time every second
                //and update the graphics
                Interrupt_disableInterrupt(INT_TA0_N);
                playing = 0;
                char str[7] = "pause.";
                sendUART(str);
                _graphics();
            }
            else
            {
                //If the video is paused play it, enable the timer that increases the video time every second
                //and update the graphics
                Interrupt_enableInterrupt(INT_TA0_N);
                playing = 1;
                char str[6] = "play.";
                sendUART(str);
                _graphics();
            }
        }
        else
        {
            //If the menu is open call the function that handles the menu option that is currently selected
            //then close the menu and update the graphics
            _menuFunc(menuSelect);
            menuOpen = 0;
            menuSelect = 0;
            Graphics_clearDisplay(&g_sContext);
            _graphics();
        }
    }
}

//Handler for the mute button
void PORT5_IRQHandler()
{
    //Clear of the interrupt flag and disable the interrupt
    GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
    Interrupt_disableInterrupt(INT_PORT5);

    if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN1) == GPIO_INPUT_PIN_LOW)
    {
        if (mute)
        {
            //If the video is muted load the saved volume and unmute it
            volume = volumeMute;
            mute = 0;
            char str[8] = "unmute.";
            sendUART(str);
        }
        else
        {
            //If the video is unmuted save the current volume and set it as 0
            //then unmute the video
            volumeMute = volume;
            volume = 0;
            mute = 1;
            char str[6] = "mute.";
            sendUART(str);
        }

        if (!menuOpen)
        {
            //If the menu is not open reload the graphics
            _graphics();
        }
    }
}

//Handler for the menu button
void PORT3_IRQHandler()
{
    //Clear of the interrupt flag and disable the interrupt
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
    Interrupt_disableInterrupt(INT_PORT3);

    if (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)
    {
        if (menuOpen)
        {
            //If the menu is open close it and load the general graphics
            menuOpen = 0;
            Graphics_clearDisplay(&g_sContext);
            _graphics();
        }
        else
        {
            //If the menu is closed open it and load the menu graphics
            menuOpen = 1;
            _menuGraphics(menuSelect);
        }
    }
}

/*uint16_t myAtoi(const char* str) {
 int result = 0;
 int i = 0;

 // Process digits
 while (str[i] >= '0' && str[i] <= '9') {
 result = result * 10 + (str[i] - '0');
 i++;
 }

 return result;
 }*/

void EUSCIA2_IRQHandler(void)
{
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {

        RXData = UART_receiveData(EUSCI_A2_BASE);
        if (RXData != '#')
        {

            if (!durationReceived)
            {
                duration[count] = RXData;
            }
            else
            {
                title[count] = RXData;
            }
            count++;
            /*ack--;
             if (ack == 0)
             {
             UART_transmitData(EUSCI_A2_BASE, '%');
             ack = 1;
             }*/
        }
        else
        {
            if (!durationReceived)
            {
                duration[count] = '\0';  // Null-terminate the string
                //sendUART(duration);
                timeMax = atoi(duration); // Convert string to integer
                durationReceived = true;
            }
            else
            {
                title[count] = '\0'; // Null-terminate the string
                gotInfo = true;
                durationReceived = false;
                Graphics_clearDisplay(&g_sContext);
                _graphics();
                playing = 1;
                Interrupt_enableInterrupt(INT_TA0_N);

                // Turn on blue LED for debug
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, RED_LED_PIN);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, BLUE_LED_PIN);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GREEN_LED_PIN);
            }
            count = 0; // Reset count for next input
        }

        Interrupt_disableSleepOnIsrExit();
    }
}

//Handler for the timer 0 (called every second)
void TA0_N_IRQHandler()
{
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    if (time != timeMax && playing == 1)
    {
        //If the video is playing and has not reached the maximum time increase the time by 1;
        time += 1;

        if (!menuOpen)
        {
            //If the menu is not open update the progress bar and the time of the video
            showProgressBar();
            int timeMinutes = time / 60;
            int timeSeconds = time % 60;
            int timeMaxMinutes = timeMax / 60;
            int timeMaxSeconds = timeMax % 60;
            tRectangle whiteBar = { 0, 65, 130, 75 };
            GrContextForegroundSet(&g_sContext, 0xffffff);
            GrRectFill(&g_sContext, &whiteBar);
            GrContextForegroundSet(&g_sContext, 0xff0000);

            sprintf(string[0], "Time: %02d:%02d/%02d:%02d", timeMinutes,
                    timeSeconds, timeMaxMinutes, timeMaxSeconds);
            Graphics_drawStringCentered(&g_sContext, (int8_t*) string[0], 30,
                                        64, 70, OPAQUE_TEXT);
        }

        if (time == timeMax)
        {
            //If the video reached the end pause it, disable the timer and update the graphics
            playing = 0;
            Interrupt_disableInterrupt(INT_TA0_N);
            _graphics();
        }
    }
}

//Handler for the timer 1 (called in less than a second)
void TA1_N_IRQHandler()
{
    //Reset the delay for volume and time change and enable the buttons
    Timer_A_clearInterruptFlag(TIMER_A1_BASE);
    volumeDelay = 1;
    timeDelay = 1;
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT3);
}

//Handler for the timer 2 (called every second)
void TA2_N_IRQHandler()
{
    Timer_A_clearInterruptFlag(TIMER_A2_BASE);

    //Modify the position of the title
    if (titlePos > 0)
    {
        titlePos -= 10;
    }
    else
    {
        titlePos = 134;
    }

    //If the menu is closed update the title
    if (!menuOpen)
    {
        _titleGraphics();
    }
}
