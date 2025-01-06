/*
 * File:   IOs.h
 * Author: Aadi Chauhan, Phoenix Bouma, Saim Khalid
 *
 * Created on: 2024-10-20
 */

#ifndef IOS_H
#define IOS_H


// Function Prototypes
void IOinit(void);
void debounce(void);
void IO_check(void);
void delay(uint16_t milliseconds);
void set_PWM_duty_cycle(uint16_t duty);
void timerSetup(uint32_t period, uint32_t clkVal);
void sendDataOverUART(uint16_t adcValue);

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);


// Global Variables
extern int pb1_state;
extern int pb2_state;
extern int pb3_state;




#endif /* IOS_H */
