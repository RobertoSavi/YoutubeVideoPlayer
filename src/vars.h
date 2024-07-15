#ifndef __VARS_H__
#define __VARS_H__

#define RED_LED_PIN GPIO_PIN0
#define GREEN_LED_PIN GPIO_PIN1
#define BLUE_LED_PIN GPIO_PIN2

int time;
int timeMax;
uint16_t volume;
uint8_t volumeDelay;
uint16_t volumeMute;
uint8_t mute;
uint8_t timeDelay;
uint8_t playing;
uint8_t menuOpen;
uint8_t menuSelect;
uint8_t titlePos;

uint8_t TXData;
uint8_t RXData;
char receiveData[50];
uint8_t duration[10];
uint8_t title[100];
uint8_t count;
uint8_t ack;
bool durationReceived;
bool gotInfo;

const uint8_t posBarraX;
const uint8_t posBarraY;
const uint8_t dimBarraX;
const uint8_t dimBarraY;

const uint32_t palette[];
const uint8_t YTLogoBitmap[];
const Graphics_Image YTLogo;

const uint8_t pauseBitmap[];
const Graphics_Image pauseIMG;

const uint8_t playBitmap[];
const Graphics_Image playIMG;

char string[10][100];

Graphics_Context g_sContext;

uint16_t resultsBuffer[2];

const Timer_A_ContinuousModeConfig contFast;

const Timer_A_ContinuousModeConfig contSecond;

const eUSCI_UART_ConfigV1 uartConfig;

#endif
