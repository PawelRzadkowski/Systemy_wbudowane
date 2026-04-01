/*
 * File:   zad1.c
 * Author: local
 *
 * Created on April 1, 2026, 12:37 PM
 */


#include "xc.h"


int main(void) {
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
