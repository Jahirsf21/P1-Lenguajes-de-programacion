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

//Funciones de utilidad
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void mostrarLibro(const Libro *libro){
    printf("Código: %s\n", libro->codigo);
    printf("Título: %s\n", libro->titulo);
    printf("Autor: %s\n", libro->autor);
    printf("Precio: %.2f\n", libro->precio);
    printf("Stock: %d\n", libro->stock);
    printf("------------------------\n");
}

void redimensionarInventario(InventarioLibros *inventario){
    if(inventario->stockLibros >= inventario->capacidad){
        inventario->capacidad *= 2;
        Libro *temp = realloc(inventario->libros, inventario->capacidad * sizeof(Libro));
        if(temp == NULL){
            printf("Error al redimensionar el inventario.\n");
            return;
        }
        inventario->libros = temp;
    }
}

int validarCodigoUnico(InventarioLibros *inventario, const char *codigo) {
    for(int i = 0; i < inventario->stockLibros; i++) {
        if(strcmp(inventario->libros[i].codigo, codigo) == 0) {
            return 0; //codigo ya existe
        }
    }
    return 1; //codigo unico
}

//Funciones de manejo de inventario
InventarioLibros* iniciarInventario() {
    InventarioLibros *inventario = malloc(sizeof(InventarioLibros));
    if(inventario == NULL) {
        printf("Error al asignar memoria para el inventario.\n");
        return NULL;
    }

    inventario->capacidad = 10; //capacidad inicial
    inventario->stockLibros = 0;
    inventario->libros = malloc(inventario->capacidad * sizeof(Libro));
    if(inventario->libros == NULL){
        printf("Error al asignar memoria para los libros.\n");
        free(inventario);
        return NULL;
    }
    return inventario;
}

void liberarInventario(InventarioLibros *inventario){
    if(inventario != NULL){
        if(inventario->libros != NULL){
            free(inventario->libros);
        }
        free(inventario);
    }

}