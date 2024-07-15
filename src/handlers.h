#ifndef __HANDLERS_H__
#define __HANDLERS_H__

void sendUART(char* str);
void ADC14_IRQHandler();
void PORT4_IRQHandler();
void PORT5_IRQHandler();
void PORT3_IRQHandler();
//uint16_t myAtoi();
void EUSCIA2_IRQHandler();
void TA0_N_IRQHandler();
void TA1_N_IRQHandler();
void TA2_N_IRQHandler();

#endif
