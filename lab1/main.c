#include <stdio.h>
#include <stdlib.h>

float sumowanie(float a, float b){
    return a+b;
}

int mnozenie (int a , int b , int c){
    return a*b*c;
}

void czydodatnia(int liczba){
    if(liczba >0){
        printf("Liczba jest dodatnia");
    }
}

int wyswietlanietablicy(int tab[],int size){
    printf("\n Tablica: ");
    for(int i=0;i<size;i++){
        printf("%d", tab[i]);
    }
    return 0;
}


int silnia(int a){
    int wynik=1;
    int i=1;
    if(a == 0){
        return 0;
    }
    while(i<=a){
        wynik = wynik*i;
        i++;
    }
    return wynik;

}

int main()
{
   printf("silnia z %d to %d",6,silnia(6));

    return 0;
}
