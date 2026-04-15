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
    
    int zmiennaStan = 0;
    int rd6prev = 0;
    int rd13prev = 0;
    unsigned char licznik = 0;
    
    
    while(1){
        int buttonrd6 = PORTDbits.RD6;
        int buttonrd13 = PORTDbits.RD13;
        //zmiana stanu 
        if(buttonrd6 == 1 && rd6prev ==0){
            zmiennaStan++;
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
                __delay32(100000);
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
        }
    }
    
    return 0;
    
    
}