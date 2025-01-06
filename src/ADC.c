/*
 * File:   ADC.c
 * Author: Aadi
 *
 * Created on November 21, 2024, 10:00 PM
 */

#include "xc.h"
#include "ADC.h"
#include "clkChange.h"
#include "UART2.h"

void DispADC(void)
{
        newClk(8);
        uint16_t adcbuf;
        uint8_t MarkerCnt;
        uint8_t MarkerCntOld; 
        
        adcbuf = do_ADC();
        MarkerCnt = adcbuf/20 + 1;
        
        if (MarkerCntOld != MarkerCnt)
        {
            XmitUART2('\r', 1);
            XmitUART2(' ', MarkerCntOld + 15);
            XmitUART2('\r', 1);
            XmitUART2('*', MarkerCnt);
            XmitUART2(' ', 1);
            Disp2Hex(adcbuf);
            MarkerCntOld = MarkerCnt;
        }
        newClk(32);
    return;
}

uint16_t do_ADC(void) {
    AD1PCFG = 0xFFFF;
    uint16_t ADCvalue;

    AD1CON1bits.ADON = 1;

    AD1CON1bits.FORM = 0b00;
    AD1CON1bits.SSRC = 0b111;
    AD1CON1bits.ASAM = 0;

    AD1CON2bits.VCFG = 0b000;

    AD1CON2bits.OFFCAL = 0;
    AD1CON2bits.CSCNA = 0;

    AD1CON2bits.BUFM = 0;
    AD1CON2bits.ALTS = 0;

    AD1CON3bits.ADRC = 0;

    AD1CON3bits.SAMC = 0b11111;

    AD1CHSbits.CH0NA = 0;
    AD1CHSbits.CH0SA = 0b0101;
    AD1PCFGbits.PCFG5 = 0;

    AD1CON1bits.SAMP = 1;
    while (AD1CON1bits.DONE == 0) {
    }
    ADCvalue = ADC1BUF0;
    AD1CON1bits.SAMP = 0;
    AD1CON1bits.ADON = 0;
    return (ADCvalue);
}

