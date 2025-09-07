#include "../headers/libros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Funciones auxiliares
int compararCadenas(const char *cadena1, const char *cadena2) {
    while(*cadena1 && (cadena1 == cadena2)) {
        cadena1++;
        cadena2++;
    }
    return *(const unsigned char*)cadena1 - *(const unsigned char*)cadena2;
}

void copiarCadena(char *destino, const char *origen){
    while(*origen) {
        *destino++ = *origen++;
    }
    *destino = '\0';
}

int largoCadena(const char *cadena){
    int largo = 0;
    while (*cadena++) largo++;
    return largo;
}
