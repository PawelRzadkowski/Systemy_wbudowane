/*
 * File:   Zad2.c
 * Author: local
 *
 * Created on April 15, 2026, 11:48 AM
 */


#include "xc.h"
#include "libpic30.h"
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


int main(void) {
    
    unsigned portValue = 0x0001;
    char current6 = 0, prev6 =0, current7 =0, prev7=0;
    int value = 1;
    
    TRISA = 0x0000;
    TRISD =0xFFFF;
    
    while(1){
        switch(value){
            case 1:
                LATA = portValue;
                break;
            case 2:
                LATA = portValue +1;
                break;
            case 3:
                LATA = portValue +3;
                break;
            default:
                LATA = value;
        }
        prev6 = PORTDbits.RD6;
        prev7 = PORTDbits.RD7;
        __delay32(150000);
        current6 = PORTDbits.RD6;
        current7 = PORTDbits.RD7;
        
        if(current6 - prev6 ==1){ //przycisk w gore
            value --;
            portValue = 1;
            if(value <=0){
                value = 3;
            }
        }
        
        if (current7 - prev7 == 1){
            value++;
            portValue=1;
            if(value>=4){
                value = 1;
        }
    }
}
    
    return 0;
}
