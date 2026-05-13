/*
 * File:   Zad4.c
 * Author: local
 *
 * Created on May 13, 2026, 12:27 PM
 */

// PIC24FJ128GA010 Configuration Bit Settings

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
#include "buttons.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"

void init_system(void)
{
    LCD_Initialize();

    BUTTON_Enable(BUTTON_S3);
    BUTTON_Enable(BUTTON_S4);
    BUTTON_Enable(BUTTON_S5);
    BUTTON_Enable(BUTTON_S6);
}

void pokaz_ustawienia(const char* poziomMocy,
                      char* minStr,
                      char* secStr)
{
    LCD_ClearScreen();

    LCD_PutString("Moc: ", 5);
    LCD_PutString(poziomMocy,
                  strlen(poziomMocy));

    LCD_PutChar('\n');

    LCD_PutString("Timer: ", 7);

    LCD_PutString(minStr,
                  strlen(minStr));

    LCD_PutString(":", 1);

    LCD_PutString(secStr,
                  strlen(secStr));
}

void ekran_startowy(void)
{
    LCD_ClearScreen();

    LCD_PutString("MIKROFALOWKA", 12);

    LCD_PutChar('\n');

    LCD_PutString("GOTOWA", 6);
}

void menu_kuchenki(void)
{
    const char* poziomyMocy[] =
    {
        "800W",
        "600W",
        "350W",
        "200W"
    };

    int pozostalyCzas = 0;

    int min;
    int sec;

    char txtMin[5];
    char txtSec[5];

    int aktualnaMoc = 0;

    bool uruchomiona = false;

    while(1)
    {
        min = pozostalyCzas / 60;
        sec = pozostalyCzas % 60;

        sprintf(txtMin, "%02d", min);
        sprintf(txtSec, "%02d", sec);

        if(!uruchomiona)
        {
            pokaz_ustawienia(poziomyMocy[aktualnaMoc],
                             txtMin,
                             txtSec);
        }

        if(BUTTON_IsPressed(BUTTON_S6))
        {
            aktualnaMoc++;

            if(aktualnaMoc > 3)
            {
                aktualnaMoc = 0;
            }

            while(BUTTON_IsPressed(BUTTON_S6));

            __delay32(300000);
        }

        if(BUTTON_IsPressed(BUTTON_S5))
        {
            pozostalyCzas += 60;

            if(pozostalyCzas > 5999)
            {
                pozostalyCzas = 5999;
            }

            while(BUTTON_IsPressed(BUTTON_S5));

            __delay32(300000);
        }
        if(BUTTON_IsPressed(BUTTON_S4))
        {
            pozostalyCzas += 10;

            if(pozostalyCzas > 5999)
            {
                pozostalyCzas = 5999;
            }

            while(BUTTON_IsPressed(BUTTON_S4));

            __delay32(300000);
        }
        if(BUTTON_IsPressed(BUTTON_S3))
        {
            uruchomiona = !uruchomiona;

            while(BUTTON_IsPressed(BUTTON_S3));

            __delay32(300000);
        }

        if(uruchomiona && pozostalyCzas > 0)
        {
            min = pozostalyCzas / 60;
            sec = pozostalyCzas % 60;

            sprintf(txtMin, "%02d", min);
            sprintf(txtSec, "%02d", sec);

            LCD_ClearScreen();

            LCD_PutString("Pracuje...", 9);

            LCD_PutChar('\n');

            LCD_PutString(txtMin,
                          strlen(txtMin));

            LCD_PutString(":", 1);

            LCD_PutString(txtSec,
                          strlen(txtSec));

            __delay32(2000000);

            pozostalyCzas--;

            if(pozostalyCzas <= 0)
            {
                uruchomiona = false;
                pozostalyCzas = 0;
            }
        }
    }
}

void glowne_menu(void)
{
    ekran_startowy();

    while(1)
    {
        if(BUTTON_IsPressed(BUTTON_S5))
        {
            while(BUTTON_IsPressed(BUTTON_S5));

            menu_kuchenki();

            return;
        }

        if(BUTTON_IsPressed(BUTTON_S4))
        {
            while(BUTTON_IsPressed(BUTTON_S4));

            menu_kuchenki();

            return;
        }
    }
}

int main(void)
{
    init_system();

    glowne_menu();

    while(1)
    {

    }

    return 0;
}