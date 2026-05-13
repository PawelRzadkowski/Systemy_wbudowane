/*
 * File:   Zad3.c
 * Author: local
 *
 * Created on May 13, 2026, 12:02 PM
 */


// PIC24FJ128GA010 Configuration Bit Settings

// CONFIG2
#pragma config POSCMOD = XT
#pragma config OSCIOFNC = ON
#pragma config FCKSM = CSDCMD
#pragma config FNOSC = PRI
#pragma config IESO = ON

// CONFIG1
#pragma config WDTPS = PS32768
#pragma config FWPSA = PR128
#pragma config WINDIS = ON
#pragma config FWDTEN = ON
#pragma config ICS = PGx2
#pragma config GWRP = OFF
#pragma config GCP = OFF
#pragma config JTAGEN = OFF

#include "xc.h"
#include "libpic30.h"
#include "stdio.h"
#include "adc.h"
#include "stdbool.h"

void init(void)
{
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);

    TRISA = 0x0000;
    LATA = 0x0000;

    TRISD |= (1 << 6);

    CNPU2bits.CN16PUE = 1;
}

int main(void)
{
    init();

    unsigned long wartosc_adc;
    unsigned long procent_adc;
    unsigned int stan_alarmu = 0;
    unsigned int licznik_migania = 0;

    bool blokada_alarmu = false;

    while(1)
    {
        wartosc_adc = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);

        if(wartosc_adc == 0xFFFF)
        {
            continue;
        }

        procent_adc = (wartosc_adc * 100UL) / 1023UL;

        unsigned char przycisk = PORTDbits.RD6;
        if(procent_adc < 50)
        {
            stan_alarmu = 0;
            licznik_migania = 0;
            blokada_alarmu = false;

            LATA = 0x0000;
        }
        else if(przycisk == 0)
        {
            stan_alarmu = 0;
            blokada_alarmu = true;

            LATA = 0x0000;
        }
        else if(procent_adc >= 50 &&
                stan_alarmu == 0 &&
                !blokada_alarmu)
        {
            stan_alarmu = 1;
            licznik_migania = 0;
        }
        if(stan_alarmu == 1)
        {
            LATA ^= 0x0001;

            __delay32(1000000);

            licznik_migania++;

            if(licznik_migania >= 20)
            {
                stan_alarmu = 2;
            }
        }
        else if(stan_alarmu == 2)
        {
            LATA = 0x00FF;
        }
    }

    return 0;
}