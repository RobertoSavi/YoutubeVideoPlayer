#ifndef __VARS_H__
#define __VARS_H__

#define RED_LED_PIN GPIO_PIN0
#define GREEN_LED_PIN GPIO_PIN1
#define BLUE_LED_PIN GPIO_PIN2

extern int time;
extern int timeMax;
extern uint16_t volume;
extern uint8_t volumeDelay;
extern uint16_t volumeMute;
extern uint8_t mute;
extern uint8_t timeDelay;
extern uint8_t playing;
extern uint8_t menuOpen;
extern uint8_t menuSelect;
extern uint8_t titlePos;

extern uint8_t TXData;
extern uint8_t RXData;
extern char receiveData[50];
extern uint8_t duration[10];
extern uint8_t title[100];
extern uint8_t count;
extern uint8_t ack;
extern bool durationReceived;
extern bool gotInfo;
extern uint8_t speed;

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

extern char string[10][100];

extern Graphics_Context g_sContext;

extern uint16_t resultsBuffer[2];

extern const Timer_A_ContinuousModeConfig contFast;

extern const Timer_A_ContinuousModeConfig contSecond;

extern const eUSCI_UART_ConfigV1 uartConfig;

#endif
