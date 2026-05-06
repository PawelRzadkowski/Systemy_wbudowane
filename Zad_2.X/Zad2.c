/*
 * File:   Zad2.c
 * Author: local
 *
 * Created on May 6, 2026, 12:07 PM
 */


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
#include "adc.h"

#define LICZBA_PROGRAMOW 2

static const unsigned long TABELA_PREDKOSCI[5] = {
    2000000UL,
    6000000UL,
    12000000UL,
    24000000UL,
    40000000UL
};

static unsigned char licznik  = 0;
static int           pozycja  = 0;
static int           kierunek = 1;

void inicjalizacja(void) {
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);

    AD1PCFG = 0xFFFF;

    TRISA  = 0x0000;
    TRISB |= (1 << 3) | (1 << 4);

    LATA   = 0x0000;

    // Pull-upy dla przycisków (WA?NE)
    CNPU1bits.CN6PUE = 1; // RB3
    CNPU1bits.CN7PUE = 1; // RB4
}

unsigned long pobierzOpoznienie(void) {
    unsigned long odczyt = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);

    if (odczyt == 0xFFFF) return TABELA_PREDKOSCI[2];

    unsigned char poziom = (unsigned char)(odczyt / 205);
    if (poziom > 4) poziom = 4;

    return TABELA_PREDKOSCI[poziom];
}

void resetujStan(void) {
    licznik  = 0;
    pozycja  = 0;
    kierunek = 1;
}

int main(void) {
    inicjalizacja();

    unsigned char aktywny_program = 0;

    char prev_rb3 = 1, curr_rb3 = 1;
    char prev_rb4 = 1, curr_rb4 = 1;

    unsigned long opoznienie;

    while (1) {

        // --- efekty LED ---
        switch (aktywny_program) {
            case 0:
                LATA = licznik;
                licznik++;
                break;

            case 1:
                LATA = (unsigned int)(0b111 << pozycja);
                pozycja += kierunek;

                if (pozycja == 5 || pozycja == 0)
                    kierunek *= -1;
                break;

            default:
                aktywny_program = 0;
                break;
        }

        opoznienie = pobierzOpoznienie();

        // --- dzielony delay + obs?uga przycisków ---
        for (unsigned long i = 0; i < opoznienie; i += 2000) {

            __delay32(2000);

            curr_rb3 = PORTBbits.RB3;
            curr_rb4 = PORTBbits.RB4;

            // RB3 ? nast?pny program
            if (curr_rb3 == 0 && prev_rb3 == 1) { // przycisk wci?ni?ty (aktywny LOW)
                __delay32(20000); // debounce ~1ms
                if (PORTBbits.RB3 == 0) {
                    aktywny_program = (aktywny_program + 1) % LICZBA_PROGRAMOW;
                    resetujStan();
                }
            }

            // RB4 ? poprzedni program
            if (curr_rb4 == 0 && prev_rb4 == 1) {
                __delay32(20000);
                if (PORTBbits.RB4 == 0) {
                    aktywny_program = (aktywny_program + LICZBA_PROGRAMOW - 1) % LICZBA_PROGRAMOW;
                    resetujStan();
                }
            }

            prev_rb3 = curr_rb3;
            prev_rb4 = curr_rb4;
        }
    }

    return 0;
}