#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "src/LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "menu.h"
#include "vars.h"
#include "handlers.h"
#include "init.h"

void _buttonsInit()
{
    /* clean voltage in button */
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);

    //Set the buttons as pullup resistors and enable their interrupts
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5,GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5,GPIO_PIN1);
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P4,GPIO_PIN1);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3,GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P3,GPIO_PIN5);
    GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);

    /* registering to NVIC */
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT3);

    /* clear interrupt flags */
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);
    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    //Set the led as an output pin
    GPIO_setAsOutputPin(GPIO_PORT_P2, RED_LED_PIN | GREEN_LED_PIN | BLUE_LED_PIN);

    /* activate interrupt notification */
    Interrupt_enableMaster();
}

void _UARTInit()
{
    // Selecting P3.2 and P3.3 in UART mode and P1.0 as output (LED)
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48); //MODIFIED TO 48MHz
    /* Configuring UART Module */
    UART_initModule(EUSCI_A2_BASE, &uartConfig);
    /* Enable UART module */
    UART_enableModule(EUSCI_A2_BASE);
    /* Enabling interrupts */
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA2);
    Interrupt_enableSleepOnIsrExit();
}

void _graphicsInit()
{
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, 0xFF0000);
    Graphics_setBackgroundColor(&g_sContext, 0xFFFFFF);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    //Clear the display and call connection graphics
    Graphics_clearDisplay(&g_sContext);
    _connectionGraphics();
}

void _adcInit()
{
    /* Configures Pin 6.0 and 4.4 as ADC input */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADCOSC/64/8) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
     * with internal 2.5v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
     *  is complete and enabling conversions */
    ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence convert.*/
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}

void _clocksInit()
{
    /* Starting and enabling ACLK (32kHz) */
    /* Configuring Continuous Mode */
    Timer_A_configureContinuousMode(TIMER_A0_BASE, &contSecond);
    Timer_A_configureContinuousMode(TIMER_A1_BASE, &contFast);
    Timer_A_configureContinuousMode(TIMER_A2_BASE, &contSecond);

    /* Enabling interrupts and going to sleep */
    Interrupt_enableSleepOnIsrExit();
    Interrupt_enableInterrupt(INT_TA1_N);
    Interrupt_enableInterrupt(INT_TA2_N);

    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();

    /* Starting the timers in continuous mode */
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);
}

int _connectionInit()
{
    //sendUART("ready.");

    while (0 /* receiving data */)
        while (!gotInfo)
        {
            /* receive data */
        }

    playing = 1;
    Interrupt_enableInterrupt(INT_TA0_N);

    Graphics_clearDisplay(&g_sContext);
    _graphics();
    //Graphics_clearDisplay(&g_sContext);
    //_graphics();

    // Turn off red and blue LEDs
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, RED_LED_PIN);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, BLUE_LED_PIN);
    //GPIO_setOutputLowOnPin(GPIO_PORT_P2, RED_LED_PIN);
    //GPIO_setOutputLowOnPin(GPIO_PORT_P2, BLUE_LED_PIN);

    // Turn on green LED
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GREEN_LED_PIN);
    //GPIO_setOutputHighOnPin(GPIO_PORT_P2, GREEN_LED_PIN);

    return 0;
}

void _hwInit()
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Turn on red LED
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GREEN_LED_PIN);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, BLUE_LED_PIN);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, RED_LED_PIN);

    //Call the other inits
    _graphicsInit();
    _buttonsInit();
    _adcInit();
    _UARTInit();
    _clocksInit();
    _connectionInit();
}
