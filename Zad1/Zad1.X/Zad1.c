/*
 * File:   Zad1.c
 * Author: local
 *
 * Created on April 22, 2026, 12:27 PM
 */

#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI  //            // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON   //           // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF       
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include"libpic30.h" 


int main(void) {
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    TRISD = 0xFFFF;
 
    unsigned char zmiennaStan = 0;
    unsigned char licznik = 0;
 
    char prev6 = 0, current6 = 0;
    char prev7 = 0, current7 = 0;
 
    int pos = 0;
    int kierunek = 1;
    unsigned char kolejka = 0x01;
    unsigned char lfsr = 0b111001;
 
    while (1) {
 
        switch (zmiennaStan) {
            case 0:
                LATA = licznik;
                licznik++;
                break;
            case 1:
                LATA = licznik;
                licznik--;
                break;
            case 2:
                LATA = licznik ^ (licznik >> 1);
                licznik++;
                break;
            case 3:
                LATA = licznik ^ (licznik >> 1);
                licznik--;
                break;
            case 4: {
                unsigned char dziesiatki = licznik / 10;
                unsigned char jednosci = licznik % 10;
                LATA = (dziesiatki << 4) | jednosci;
                licznik++;
                if (licznik > 99) licznik = 0;
                break;
            }
            case 5: {
                unsigned char dziesiatki = licznik / 10;
                unsigned char jednosci = licznik % 10;
                LATA = (dziesiatki << 4) | jednosci;
                if (licznik == 0) licznik = 99;
                else licznik--;
                break;
            }
            case 6:
                LATA = (1 << pos);
                pos += kierunek;
                if (pos == 7 || pos == 0) kierunek *= -1;
                break;
            case 7:
                LATA = kolejka;
                kolejka <<= 1;
                if (kolejka == 0) kolejka = 0x01;
                break;
            case 8: {
                unsigned char bit = ((lfsr >> 5) ^ (lfsr >> 4)) & 1;
                lfsr = (lfsr << 1) | bit;
                LATA = lfsr;
                break;
            }
        }
 
        prev6  = PORTDbits.RD6;
        prev7 = PORTDbits.RD7;
 
        __delay32(100000);
 
        current6 = PORTDbits.RD6;
        current7 = PORTDbits.RD7;
 
        if (current6 - prev6 == 1) {
            zmiennaStan++;
            if (zmiennaStan > 8) zmiennaStan = 0;
            licznik = 0;
            pos = 0; kierunek = 1;
            kolejka = 0x01;
            lfsr = 0b111001;
        }
 
        if (current7 - prev7 == 1) {
            if (zmiennaStan == 0) zmiennaStan = 8;
            else zmiennaStan--;
            licznik = 0;
            pos = 0; kierunek = 1;
            kolejka = 0x01;
            lfsr = 0b111001;
        }
    }
 
    return 0;
}