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

#define LICZBA_PROGRAMOW   2
#define OPOZNIENIE_DEBOUNCE 40000UL

static const unsigned long TABELA_PREDKOSCI[5] = {
    2000000UL,
    6000000UL,
    12000000UL,
    24000000UL,
    40000000UL
};

static unsigned char licznik   = 0;
static int           pozycja   = 0;
static int           kierunek  = 1;

void inicjalizacja(void) {
    AD1PCFG = 0xFFFF;
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    TRISA  = 0x0000;
    TRISB |= (1 << 3) | (1 << 4);
    LATA   = 0x0000;
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

void licznik_binarny_gora(unsigned long opoznienie) {
    LATA = licznik;
    licznik++;
    __delay32(opoznienie);
}

void wezyk(unsigned long opoznienie) {
    LATA = (unsigned int)(0b111 << pozycja);
    pozycja += kierunek;
    if (pozycja == 5 || pozycja == 0)
        kierunek *= -1;
    __delay32(opoznienie);
}

unsigned char wykryjWcisniecie(unsigned char aktualny, unsigned char *poprzedni) {
    unsigned char wcisniety = 0;
    if (*poprzedni == 1 && aktualny == 0) {
        __delay32(OPOZNIENIE_DEBOUNCE);
        wcisniety = 1;
    }
    *poprzedni = aktualny;
    return wcisniety;
}

int main(void) {
    inicjalizacja();

    unsigned char aktywny_program = 0;
    unsigned char poprzedni_rb3 = 1;
    unsigned char poprzedni_rb4 = 1;
    unsigned long opoznienie;

    while (1) {
        opoznienie = pobierzOpoznienie();

        if (wykryjWcisniecie(PORTBbits.RB3, &poprzedni_rb3)) {
            aktywny_program = (aktywny_program + 1) % LICZBA_PROGRAMOW;
            resetujStan();
        }

        if (wykryjWcisniecie(PORTBbits.RB4, &poprzedni_rb4)) {
            aktywny_program = (aktywny_program + LICZBA_PROGRAMOW - 1) % LICZBA_PROGRAMOW;
            resetujStan();
        }

        switch (aktywny_program) {
            case 0:
                licznik_binarny_gora(opoznienie);
                break;
            case 1:
                wezyk(opoznienie);
                break;
            default:
                aktywny_program = 0;
                break;
        }
    }

    return 0;
}
