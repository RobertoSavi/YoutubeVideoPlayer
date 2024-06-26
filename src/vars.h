#ifndef __VARS_H__
#define __VARS_H__

#define RED_LED_PIN GPIO_PIN0
#define GREEN_LED_PIN GPIO_PIN1
#define BLUE_LED_PIN GPIO_PIN2

extern volatile int time;
extern volatile int timeMax;
extern volatile uint16_t volume;
extern volatile uint8_t volumeDelay;
extern volatile uint16_t volumeMute;
extern volatile uint8_t mute;
extern volatile uint8_t timeDelay;
extern volatile uint8_t playing;
extern volatile uint8_t menuOpen;
extern volatile uint8_t menuSelect;

extern volatile uint8_t TXData;
extern volatile uint8_t RXData;
extern volatile char receiveData[50];
extern uint8_t duration[10];
extern uint8_t title[100];
extern volatile uint8_t count;
extern volatile uint8_t ack;
extern volatile bool durationReceived;
extern volatile bool gotInfo;

extern const uint8_t posBarraX;
extern const uint8_t posBarraY;
extern const uint8_t dimBarraX;
extern const uint8_t dimBarraY;

extern const uint32_t palette[];
extern const uint8_t YTLogoBitmap[];
extern const Graphics_Image YTLogo;

extern const uint8_t pauseBitmap[];
extern const Graphics_Image pauseIMG;

extern const uint8_t playBitmap[];
extern const Graphics_Image playIMG;

extern char string[10][30];

extern Graphics_Context g_sContext;

extern uint16_t resultsBuffer[2];

extern const Timer_A_ContinuousModeConfig contFast;

extern const Timer_A_ContinuousModeConfig contSecond;

extern const eUSCI_UART_ConfigV1 uartConfig;

#endif
