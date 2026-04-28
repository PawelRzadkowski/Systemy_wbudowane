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


void ADC_Init(void)
{
    AD1PCFGL  = 0xFFFE;
    AD1CON1   = 0x0000;
    AD1CON2   = 0x0000;
    AD1CON3   = 0x0202;
    AD1CHS0   = 0x0000;
    AD1CON1bits.ADON = 1;
}
 
unsigned int ADC_Read(void)
{
    AD1CON1bits.SAMP = 1;
    __delay32(2000);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}
 
#define THRESHOLD         512u
#define ALL_LEDS          0x00FF
#define ONE_LED           0x0001
#define LEDS_OFF          0x0000
#define BLINK_HALF_DELAY  750000UL
#define BLINK_HALF_TICKS  14u
 
typedef enum {
    ST_IDLE,
    ST_BLINK,
    ST_ALL_ON
} AlarmState;
 
int main(void)
{
    TRISA = 0x0000;
    LATA  = LEDS_OFF;
 
    TRISBbits.TRISB3 = 1;
 
    ADC_Init();
 
    AlarmState state      = ST_IDLE;
    unsigned int blinkCnt = 0;
    unsigned char ledVal  = 0;
    char prevRB3          = 0;
    char currRB3          = 0;
 
    while (1)
    {
        unsigned int adcVal = ADC_Read();
 
        currRB3 = PORTBbits.RB3;
        int rb3Pressed = (currRB3 == 1 && prevRB3 == 0);
        prevRB3 = currRB3;
 
        switch (state)
        {
            case ST_IDLE:
                LATA = LEDS_OFF;
                if (adcVal > THRESHOLD)
                {
                    state    = ST_BLINK;
                    blinkCnt = 0;
                    ledVal   = 0;
                }
                break;
 
            case ST_BLINK:
                if (rb3Pressed || adcVal <= THRESHOLD)
                {
                    state = ST_IDLE;
                    LATA  = LEDS_OFF;
                    break;
                }
 
                ledVal = !ledVal;
                LATA   = ledVal ? ONE_LED : LEDS_OFF;
                __delay32(BLINK_HALF_DELAY);
 
                blinkCnt++;
                if (blinkCnt >= BLINK_HALF_TICKS)
                {
                    state = ST_ALL_ON;
                    LATA  = ALL_LEDS;
                }
                break;
 
            case ST_ALL_ON:
                if (rb3Pressed || adcVal <= THRESHOLD)
                {
                    state = ST_IDLE;
                    LATA  = LEDS_OFF;
                }
                else
                {
                    LATA = ALL_LEDS;
                }
                break;
 
            default:
                state = ST_IDLE;
                break;
        }
 
        __delay32(20000);
    }
 
    return 0;
}