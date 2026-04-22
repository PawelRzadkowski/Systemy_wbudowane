/*
 * File:   newmainXC16.c
 * Author: local
 *
 * Created on April 1, 2026, 12:08 PM
 */
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include"libpic30.h" //biblioteka do opoznienia


int main(void) {
    TRISA = 0x0000;   
    TRISD = 0xFFFF;
    
    unsigned char zmiennaStan = 0;
    int rd6prev = 0;
    int rd13prev = 0;
    unsigned char licznik = 0;
    
    
    while(1){
        int buttonrd6 = PORTDbits.RD6;
        int buttonrd13 = PORTDbits.RD13;
        //zmiana stanu 
        if(buttonrd6 == 1 && rd6prev ==0){
            zmiennaStan++;
            licznik = 0;
            if(zmiennaStan > 8){
                zmiennaStan = 0;
            }
        }
        
        if(buttonrd13 == 1 && rd13prev == 0){
            if(zmiennaStan == 0){
                zmiennaStan = 8;
            }
            else{
                zmiennaStan--;
            }
            
        }
        
        rd6prev = buttonrd6;
        rd13prev = buttonrd13;
        
        switch(zmiennaStan){
            case 0:
                LATA = licznik;
                licznik++;
                __delay32(1000000);
                break;
            case 1:
                LATA = licznik;
                licznik--;
                __delay32(1000000);
                break;
            case 2:
                LATA = licznik ^ (licznik >> 1);
                licznik++;
                __delay32(1000000);
                break;
            case 3:
                LATA = licznik ^ (licznik >> 1);
                licznik--;
                __delay32(1000000);
                break;
            case 4:
                unsigned char dziesiatki = licznik / 10;
                unsigned char jednosci = licznik % 10;
                LATA = (dziesiatki << 4) | jednosci;
                licznik++;
                if(licznik > 99) licznik = 0;
                __delay32(1000000);
                break;
            case 5:
                unsigned char dziesiatki = licznik / 10;
                unsigned char jednosci = licznik % 10;
                LATA = (dziesiatki << 4) | jednosci;
                if(licznik == 0) licznik = 99;
                else licznik--;
                __delay32(1000000);
                break;
            case 6:
                static int pos = 0;
                static int kierunek = 1;
                LATA = (1 << pos);
                pos += kierunek;
                if(pos == 7 || pos == 0) kierunek *= -1;
                __delay32(1000000);
                break;
            case 7:
                static unsigned char kolejka = 0x01;
                LATA = kolejka;
                kolejka <<= 1;
                if(kolejka == 0) kolejka = 0x01;
                __delay32(1000000);
                break;
            case 8:
                static unsigned char lfsr = 0b111001;
                unsigned char bit = ((lfsr >> 5) ^ (lfsr >> 4)) & 1;
                lfsr = (lfsr << 1) | bit;
                LATA = lfsr;
                __delay32(1000000);
                break;
        }
    }
    
    return 0;
    
    
}