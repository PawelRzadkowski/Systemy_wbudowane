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

#include <xc.h>
#include "libpic30.h"


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
    unsigned char lfsr = 0b111001;

    int fixed_leds = 0;
    int moving_pos = 0;
    int target_pos = 7;

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
                LATA = (0b111 << pos);
                pos += kierunek;
                if (pos == 5 || pos == 0)
                    kierunek *= -1;
                break;
            case 7:
                LATA = fixed_leds | (1 << moving_pos);
                moving_pos++;
                if (moving_pos > target_pos) {
                    fixed_leds |= (1 << target_pos);
                    target_pos--;
                    moving_pos = 0;
                    if (target_pos < 0) {
                        fixed_leds = 0;
                        target_pos = 7;
                    }
                }
                break;
            case 8: {
                unsigned char bit = ((lfsr >> 5) ^ (lfsr >> 4)) & 1;
                lfsr = (lfsr << 1) | bit;
                LATA = lfsr;
                break;
            }
        }

        prev6 = PORTDbits.RD6;
        prev7 = PORTDbits.RD7;

        __delay32(500000);

        current6 = PORTDbits.RD6;
        current7 = PORTDbits.RD7;

        if (current6 - prev6 == 1) {
            zmiennaStan++;
            if (zmiennaStan > 8) zmiennaStan = 0;
            licznik = 0;
            pos = 0; kierunek = 1;
            fixed_leds = 0; moving_pos = 0; target_pos = 7;
            lfsr = 0b111001;
        }

        if (current7 - prev7 == 1) {
            if (zmiennaStan == 0) zmiennaStan = 8;
            else zmiennaStan--;
            licznik = 0;
            pos = 0; kierunek = 1;
            fixed_leds = 0; moving_pos = 0; target_pos = 7;
            lfsr = 0b111001;
        }
    }

    return 0;
}
