#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "vars.h"

volatile uint16_t time=0;
volatile uint16_t timeMax=0;
volatile uint16_t volume=100;
volatile uint8_t volumeDelay=0;
volatile uint16_t volumeMute;
volatile uint8_t mute=0;
volatile uint8_t timeDelay=0;
volatile uint8_t playing=0;
volatile uint8_t menuOpen=0;
volatile uint8_t menuSelect=0;
volatile char* title="Waiting for connection...";
volatile uint8_t TXData=1;
volatile uint8_t RXData=0;
volatile char receiveData[50];

const uint8_t posBarraX=29;
const uint8_t posBarraY=113;
const uint8_t dimBarraX=90;
const uint8_t dimBarraY=9;

const uint32_t palette[]={0xFFFFFF, 0xFF0000};
const uint8_t YTLogoBitmap[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
                                0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                                0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                                0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                                0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                                0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                                0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const Graphics_Image YTLogo={GRAPHICS_IMAGE_FMT_8BPP_UNCOMP, 32, 32, 2, palette, YTLogoBitmap};

const uint8_t pauseBitmap[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,
                               0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const Graphics_Image pauseIMG={GRAPHICS_IMAGE_FMT_8BPP_UNCOMP, 16, 16, 2, palette, pauseBitmap};

const uint8_t playBitmap[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
                              0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
                              0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
                              0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
                              0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                              0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                              0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                              0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                              0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
                              0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
                              0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
                              0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
                              0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const Graphics_Image playIMG={GRAPHICS_IMAGE_FMT_8BPP_UNCOMP, 16, 16, 2, palette, playBitmap};

char string[10][30];

/* Graphic library context */
Graphics_Context g_sContext;

/* ADC results buffer */
uint16_t resultsBuffer[2];

const Timer_A_ContinuousModeConfig contFast={
        TIMER_A_CLOCKSOURCE_ACLK,           // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,      // ACLK/1 = 32.768khz
        TIMER_A_TAIE_INTERRUPT_ENABLE,      // Enable Overflow ISR
        TIMER_A_DO_CLEAR};                  // Clear Counter

const Timer_A_ContinuousModeConfig contSecond={
        TIMER_A_CLOCKSOURCE_ACLK,           // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_2,      // ACLK/1 = 32.768khz
        TIMER_A_TAIE_INTERRUPT_ENABLE,      // Enable Overflow ISR
        TIMER_A_DO_CLEAR};                  // Clear Counter

const eUSCI_UART_ConfigV1 uartConfig={
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source (3MHz)
        26,                                      // prescaler for timer
        0,                                       // first modulation stage select
        111,                                     // second modulation stage select
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling enable
        EUSCI_A_UART_8_BIT_LEN                   // 8 bit data length
};

