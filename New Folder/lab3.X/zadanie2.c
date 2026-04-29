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

#define NUM_PROGRAMS   2
#define DEBOUNCE_DELAY 40000UL

static const unsigned long SPEED_TABLE[5] = {
    2000000UL,
    6000000UL,
    12000000UL,
    24000000UL,
    40000000UL
};

void init(void) {
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    TRISA = 0x0000;
    TRISB |= (1 << 3) | (1 << 4);
    LATA  = 0x0000;
}

unsigned long getDelay(void) {
    unsigned long adc = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
    if (adc == 0xFFFF) return SPEED_TABLE[2];

    unsigned char level = (unsigned char)(adc / 205);
    if (level > 4) level = 4;

    return SPEED_TABLE[level];
}

void program_binary_counter(unsigned long opoznienie) {
    static unsigned char count = 0;

    LATA = count;
    count++;
    __delay32(opoznienie);
}

void program_knight_rider(unsigned long opoznienie) {
    static unsigned char pos       = 0;
    static          char direction = 1;

    LATA = (unsigned int)(1u << pos);
    __delay32(opoznienie);

    if (direction == 1) {
        if (pos >= 7) { direction = -1; pos--; }
        else          { pos++; }
    } else {
        if (pos == 0) { direction =  1; pos++; }
        else          { pos--; }
    }
}

unsigned char buttonPressed(unsigned char current, unsigned char *previous) {
    unsigned char pressed = 0;
    if (*previous == 1 && current == 0) {
        __delay32(DEBOUNCE_DELAY);
        pressed = 1;
    }
    *previous = current;
    return pressed;
}

int main(void) {
    init();

    unsigned char current_program = 0;
    unsigned char prev_rb3 = 1;
    unsigned char prev_rb4 = 1;
    unsigned long opoznienie;

    while (1) {
        opoznienie = getDelay();

        if (buttonPressed(PORTBbits.RB3, &prev_rb3)) {
            current_program = (current_program + 1) % NUM_PROGRAMS;
        }

        if (buttonPressed(PORTBbits.RB4, &prev_rb4)) {
            current_program = (current_program + NUM_PROGRAMS - 1) % NUM_PROGRAMS;
        }

        switch (current_program) {
            case 0:
                program_binary_counter(opoznienie);
                break;
            case 1:
                program_knight_rider(opoznienie);
                break;
            default:
                current_program = 0;
                break;
        }
    }

    return 0;
}
