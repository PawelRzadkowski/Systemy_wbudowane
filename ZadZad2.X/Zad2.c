/*
 * File:   Zad2.c
 * Author: local
 *
 * Created on May 13, 2026, 11:45 AM
 */


// PIC24FJ128GA010 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include "xc.h"
#include "libpic30.h"
#include "adc.h"

#define LICZBA_PROGRAMOW 2

static const unsigned long TABELA_PREDKOSCI[5] = {
    100000UL,
    400000UL,
    1000000UL,
    3000000UL,
    7000000UL
};

static unsigned char licznik = 0;

static int pozycja = 0;
static int kierunek = 1;

void inicjalizacja(void)
{
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    TRISA = 0x0000;
    TRISD |= (1 << 6);
    TRISD |= (1 << 13);
    LATA = 0x0000;
}

unsigned long pobierzOpoznienie(void)
{
    unsigned long odczyt;
    unsigned char poziom;

    odczyt = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);

    if (odczyt == 0xFFFF)
        return TABELA_PREDKOSCI[2];

    poziom = (unsigned char)(odczyt / 205);

    if (poziom > 4)
        poziom = 4;

    return TABELA_PREDKOSCI[poziom];
}

void resetujStan(void)
{
    licznik = 0;

    pozycja = 0;
    kierunek = 1;
}

void program1(void)
{
    LATA = licznik;
    licznik++;
}

void program2(void)
{
    LATA = (0b111 << pozycja);

    pozycja += kierunek;

    if (pozycja >= 5) {
        pozycja = 5;
        kierunek = -1;
    }

    if (pozycja <= 0) {
        pozycja = 0;
        kierunek = 1;
    }
}

int main(void)
{
    inicjalizacja();

    unsigned char aktywny_program = 0;

    char prev6 = 0;
    char curr6 = 0;

    char prev13 = 0;
    char curr13 = 0;

    unsigned long opoznienie;

    while (1) {

        opoznienie = pobierzOpoznienie();

        switch (aktywny_program) {

            case 0:
                program1();
                break;

            case 1:
                program2();
                break;

            default:
                aktywny_program = 0;
                break;
        }

        prev6 = PORTDbits.RD6;
        prev13 = PORTDbits.RD13;

        __delay32(opoznienie);

        curr6 = PORTDbits.RD6;
        curr13 = PORTDbits.RD13;

        if (prev6 == 1 && curr6 == 0) {

            aktywny_program++;

            if (aktywny_program >= LICZBA_PROGRAMOW)
                aktywny_program = 0;

            resetujStan();
        }

        if (prev13 == 1 && curr13 == 0) {

            if (aktywny_program == 0)
                aktywny_program = LICZBA_PROGRAMOW - 1;
            else
                aktywny_program--;

            resetujStan();
        }
    }

    return 0;
}