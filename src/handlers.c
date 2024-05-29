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

#define RED_LED_PIN GPIO_PIN0
#define GREEN_LED_PIN GPIO_PIN1
#define BLUE_LED_PIN GPIO_PIN2

void sendUART(char *str)
{
    while (*str != '\0')
    {
        UART_transmitData(EUSCI_A2_BASE, *str);
        str++;
    }
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

void PORT4_IRQHandler()
{
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);
    Interrupt_disableInterrupt(INT_PORT4);

    if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1) == GPIO_INPUT_PIN_LOW)
    {
        if (!menuOpen)
        {
            if (playing)
            {
                Interrupt_disableInterrupt(INT_TA0_N);
                playing = 0;
                char str[7] = "pause.";
                sendUART(str);
                _graphics();
            }
            else
            {
                Interrupt_enableInterrupt(INT_TA0_N);
                playing = 1;
                char str[6] = "play.";
                sendUART(str);
                _graphics();
            }
        }
        else
        {
            _menuFunc(menuSelect);
            menuOpen = 0;
            menuSelect = 0;
            Graphics_clearDisplay(&g_sContext);
            _graphics();
        }
    }
}

void PORT5_IRQHandler()
{
    GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
    Interrupt_disableInterrupt(INT_PORT5);

    if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN1) == GPIO_INPUT_PIN_LOW)
    {
        if (mute)
        {
            volume = volumeMute;
            mute = 0;
            char str[8] = "unmute.";
            sendUART(str);
        }
        else
        {
            volumeMute = volume;
            volume = 0;
            mute = 1;
            char str[6] = "mute.";
            sendUART(str);
        }

        if (!menuOpen)
        {
            _graphics();
        }
    }
}

void PORT3_IRQHandler()
{
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
    Interrupt_disableInterrupt(INT_PORT3);

    if (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)
    {
        if (menuOpen)
        {
            menuOpen = 0;
            Graphics_clearDisplay(&g_sContext);
            _graphics();
        }
        else
        {
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
            }
            count = 0; // Reset count for next input
        }
        // Turn on blue LED for debug
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, RED_LED_PIN);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GREEN_LED_PIN);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, BLUE_LED_PIN);

        Interrupt_disableSleepOnIsrExit();
    }
}

void TA0_N_IRQHandler()
{
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    if (time != timeMax && playing == 1)
    {
        time += 1;

        if (!menuOpen)
        {
            showProgressBar();
            sprintf(string[0], "Time: %3d/%3d", time, timeMax);
            Graphics_drawStringCentered(&g_sContext, (int8_t*) string[0], 15, 64, 70, OPAQUE_TEXT);
        }

        if (time == timeMax)
        {
            playing = 0;
            Interrupt_disableInterrupt(INT_TA0_N);
            _graphics();
        }
    }
}

void TA1_N_IRQHandler()
{
    Timer_A_clearInterruptFlag(TIMER_A1_BASE);
    volumeDelay = 1;
    timeDelay = 1;
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT3);
}
