
 /*
 * File:   IOs.c
 * Author: Aadi Chauhan, Phoenix Bouma, Saim Khalid
 *
 * Created on: 2024-09-16
 */

#include <xc.h>
#include "IOs.h"
#include "clkChange.h"
#include "UART2.h"
#include "ADC.h"

int isOnMode = 0;
int isBlinking = 0;

int pb1_state;
int pb2_state = 0;
int pb3_state;


void IOinit(){
    AD1PCFG = 0xFFFF; /* keep this line as it sets I/O pins that can also be analog to be digital */
    newClk(8);
    /* Let's set up some I/O */
    TRISBbits.TRISB8 = 0;
    LATBbits.LATB8 = 0;
    // Potentiometer
    TRISAbits.TRISA3 = 1;
    // PB3
    TRISAbits.TRISA4 = 1;
    CNPU1bits.CN0PUE = 1;
    CNEN1bits.CN0IE = 1;
    // PB2
    TRISBbits.TRISB4 = 1;
    CNPU1bits.CN1PUE = 1;
    CNEN1bits.CN1IE = 1;
    // PB1
    TRISAbits.TRISA2 = 1;
    CNPU2bits.CN30PUE = 1;
    CNEN2bits.CN30IE = 1;
    
    IPC4bits.CNIP = 6;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    
    InitUART2();
}


void debounce(void){
        delay(80);
}


void IO_check(void) {

    /* IDLE CHECK */
    if (isOnMode == 0 && pb2_state == 0 && pb3_state == 0) {
        LATBbits.LATB8 = 0; // Turn LED OFF
        Idle();
        return;
    }

    /* OFF MODE*/
    else if (isOnMode == 0 && pb2_state == 1){
        /* TO DO : IMPLEMENT ADC VARIABLE BLINKING */
        LATBbits.LATB8 = 1; // 100% intensity ON
        delay(500); // 500 ms delay
        /* SHOULD I SEND UART DATA? */
        if (pb3_state == 1){
            uint16_t adcValue = do_ADC();
            sendDataOverUART(adcValue);
        }
        LATBbits.LATB8 = 0; // OFF
        delay(500); // 500 ms delay
        /* SHOULD I SEND UART DATA? */
        if (pb3_state == 1){
            uint16_t adcValue = do_ADC();
            sendDataOverUART(adcValue);
        }
    }
    
    /* ON MODE */
    if (isOnMode){
        /* NO BLINKING */
        if (pb2_state == 1 && isBlinking == 1) {
            // Normal ON mode without blinking
            for (int i = 0; i < 6; i++){ // the 6 was chosen by 500 / 80 (total delay in the regular set_PWM_duty_cycle function)
                uint16_t potentiometerValue = do_ADC();
                set_PWM_duty_cycle(potentiometerValue);            
            }
            isBlinking = 0;
            
            
            
        } 
        
        else if (pb2_state == 1 && isBlinking == 0){
            // Blinking in ON mode
            LATBbits.LATB8 = 0; // LED OFF
            delay(500);
            isBlinking = 1;
        }
        
        else if (pb2_state == 0){
            uint16_t potentiometerValue = do_ADC();
            set_PWM_duty_cycle(potentiometerValue);
        }
        
        /* SHOULD I SEND UART DATA? */
        if (pb3_state == 1){
            uint16_t adcValue = do_ADC();
            sendDataOverUART(adcValue);
        }
        
    }
    


}


void set_PWM_duty_cycle(uint16_t adcValue) {
    
//    uint16_t period = ((adcValue * 1000) / 1023); // Map to 10 ms - 1000 ms this shit was so stupid of me my brain is so cooked
    
    float dutyCycle = (float) adcValue / 0x3FF;

    float upTime = 5 * dutyCycle;
    float downTime = 5 - upTime;


    if (adcValue != 0 && adcValue != 1023 && upTime > 0 && downTime < 1000){
        // Turn LED ON
        LATBbits.LATB8 = 1;
        delay((unsigned int) upTime);
        /* SHOULD I SEND UART DATA? */
        if (pb3_state == 1) {
            uint16_t adcValue = do_ADC();
            sendDataOverUART(adcValue);
        }
        // Turn LED OFF
        LATBbits.LATB8 = 0;
        delay((unsigned int) downTime);
        /* SHOULD I SEND UART DATA? */
        if (pb3_state == 1) {
            uint16_t adcValue = do_ADC();
            sendDataOverUART(adcValue);
        }
    }

    else if (adcValue == 0){
        // PERMA OFF
        LATBbits.LATB8 = 0;
        delay(5);
        /* SHOULD I SEND UART DATA? */
        if (pb3_state == 1) {
            uint16_t adcValue = do_ADC();
            sendDataOverUART(adcValue);
        }
    }

    else if (adcValue == 1023){
        // PERMA ON
        LATBbits.LATB8 = 1;
        delay(5);
        /* SHOULD I SEND UART DATA? */
        if (pb3_state == 1) {
            uint16_t adcValue = do_ADC();
            sendDataOverUART(adcValue);
        }
    }
    
}


void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
//    Don't forget to clear the CN interrupt flag!
       // Check which buttons caused the interrupt
    if (PORTAbits.RA2 == 0){  // PB1 is pressed
        if (isOnMode == 0){
            isOnMode = 1;
        }
        else{
            isOnMode = 0;
        }
    }
        
    if (PORTBbits.RB4 == 0){  // PB2 is pressed
        if (pb2_state == 0){
            pb2_state = 1;
        }
        else{
            pb2_state = 0;
        }
    }
    
    if (PORTAbits.RA4 == 0){  // PB3 is pressed
        if (pb3_state == 0){
            pb3_state = 1;
        }
        else{
            pb3_state = 0;
        }
    }
    IFS1bits.CNIF = 0;
    
    Nop(); 
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    // Clear the Timer 2 interrupt flag
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0;
    TMR2 = 0;
    return;
}


void delay(uint16_t milliseconds) {
    if (milliseconds == 0) {
        return; // Return immediately if delay is 0 ms
    }

    uint32_t timerPeriod = milliseconds * 16; // Calculate timer period

    timerSetup(timerPeriod, 32);
}


void timerSetup(uint32_t period, uint32_t clkVal) {
    newClk(clkVal);
    T2CONbits.TON = 1;
    T2CONbits.TSIDL = 0;
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0;
    IPC1bits.T2IP = 7;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
    PR2 = period;
    Idle();
    newClk(8);
}


void sendDataOverUART(uint16_t adcValue){
    char buffer[50]; // Buffer to hold formatted string
    // Calculate intensity percentage
    uint16_t intensity = adcValue * 100 / 1023;

    if (LATBbits.LATB8 == 0){
        intensity = 0;
    }
    
    // Format data as "ADC,Intensity\n"
    sprintf(buffer, "%u,%u\n", adcValue, intensity);

    // Transmit the formatted string
    Disp2String(buffer);
    newClk(8);
    
}
