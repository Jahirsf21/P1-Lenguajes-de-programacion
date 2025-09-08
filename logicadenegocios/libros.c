#include "../headers/libros.h"
#include "../headers/pedidos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

Libro* libros = NULL;
int stockLibros = 0;

void liberarLibro(Libro *libro) {
    if (libro != NULL) {
        if (libro->titulo != NULL) {
            free(libro->titulo);
            libro->titulo = NULL;
        }
        if (libro->autor != NULL) {
            free(libro->autor);
            libro->autor = NULL;
        }
        if (libro->codigo!= NULL) {
            free(libro->codigo);
            libro->codigo = NULL;
        }
    }
}

void liberarTodosLosLibros() {
    if (libros != NULL) {
    for (int i = 0; i < stockLibros; i++) {
        liberarLibro(&libros[i]);
    }
    free(libros);
    libros = NULL;
    stockLibros = 0;
    }
}

void mostrarTodosLosLibros() {
    printf("\n=== LISTA  DE LIBROS ===\n");
    if (stockLibros == 0) {
        printf("No hay libros en el stock.\n");
    } else {
        for (int i = 0; i < stockLibros; i++) {
            char* mostrarLibros = libroToString(&libros[i]);
            if (mostrarLibros!= NULL) {
                printf("%d. %s\n", i+1, mostrarLibros);
                free(mostrarLibros);
            }
        }
    }
    printf("Total de libros: %d\n", stockLibros);
    printf("==================================\n");
}

char* libroToString(Libro *libro) {
    char stock[100];
    char precio[100];
    int largo = strlen(libro->titulo) + strlen(libro->autor) + strlen(libro->codigo) + strlen(precio) +strlen(stock)+ 50;
    char* resultado = malloc(largo);
    if (resultado == NULL) {
        return NULL;
    }
    snprintf(resultado, largo,  "Titulo: %s | Autor: %s | Codigo: %s | Precio: %.1f | Cantidad en stock: %d",libro->titulo,libro->autor,libro->codigo,libro->precio,libro->stock);
    return resultado;
}

void cargarLibros() {
    FILE* archivo = fopen("store/libros.json", "r");
    if (archivo == NULL) {
        stockLibros = 0;
        return;
    }
    char buffer[100];
    char titulo[100];
    char autor[100];
    char codigo[100];
    float precio;
    int stock;
    fgets(buffer,sizeof(buffer), archivo);
    while (fgets(buffer,sizeof(buffer),archivo)) {

        //Para el titulo
        if (strstr(buffer, "\"titulo\":")) {

            char* inicio = strchr(buffer, '\"');
            inicio = strchr(inicio + 1, '\"') + 1;
            char* fin = strchr(inicio, '\"');

            int largo = fin - inicio;
            strncpy(titulo, inicio, largo);
            titulo[largo] = '\0';
        }

        //Para la autor
        if (strstr(buffer, "\"autor\":")) {

            char* inicio = strchr(buffer, '\"');
            inicio = strchr(inicio + 1, '\"') + 1;
            char* fin = strchr(inicio, '\"');

            int largo = fin - inicio;
            strncpy(autor, inicio, largo);
            autor[largo] = '\0';
        }

        //Para el codigo
        if (strstr(buffer, "\"codigo\":")) {

            char* inicio = strchr(buffer, '\"');
            inicio = strchr(inicio + 1, '\"') + 1;
            char* fin = strchr(inicio, '\"');

            int largo = fin - inicio;
            strncpy(codigo, inicio, largo);
            codigo[largo] = '\0';
        }

        //Para el precio
        if (strstr(buffer, "\"precio\":")) {
            char* inicio = strchr(buffer, ':') + 1;
            while (*inicio == ' ') inicio++;  
            precio = atof(inicio); 
        }

        //Para el stock
        if (strstr(buffer, "\"stock\":")) {
            char* inicio = strchr(buffer, ':') + 1;
            while (*inicio == ' ') inicio++;  // Saltar espacios
            stock = atoi(inicio);  // atoi() convierte string a int

            libros = realloc(libros, (stockLibros +1) * sizeof(Libro));

            //Titulo del libro
            libros[stockLibros].titulo = malloc(strlen(titulo) +1);
            strcpy(libros[stockLibros].titulo,titulo);

            //Autor del libro
            libros[stockLibros].autor = malloc(strlen(autor) +1);
            strcpy(libros[stockLibros].autor,autor);

            //Codigo del libro
            libros[stockLibros].codigo = malloc(strlen(codigo) +1);
            strcpy(libros[stockLibros].codigo, codigo);

            //Precio del libro
            libros[stockLibros].precio = precio;

            //Cantidad en stock
            libros[stockLibros].stock = stock;
            stockLibros++;
        }
    }
    fclose(archivo);
}

void guardarLibros() {
    FILE* archivo = fopen("store/libros.json", "w");
    fprintf(archivo, "[\n");
    for (int i = 0; i < stockLibros; i++) {
        fprintf(archivo, "    {\n");
        fprintf(archivo, "        \"titulo\": \"%s\",\n", libros[i].titulo);
        fprintf(archivo, "        \"autor\": \"%s\",\n", libros[i].autor);
        fprintf(archivo, "        \"codigo\": \"%s\",\n", libros[i].codigo);
        fprintf(archivo, "        \"precio\": %.2f,\n", libros[i].precio);
        fprintf(archivo, "        \"cantidad en stock\": %d\n", libros[i].stock); 
        if (i < stockLibros - 1) {
            fprintf(archivo, "    },\n");
        } else {
            fprintf(archivo, "    }\n");
        }
    }
    fprintf(archivo, "]\n");
    fclose(archivo);
}


bool libroAsociadoPedido(char* codigo) {
    for (int i = 0; i < cantidadPedidos; i++) {
        for (int j = 0; j < pedidos[i].cantidadLibros; j++) {
            if (strcmp(pedidos[i].libros[j].codigo, codigo) == 0) {
                return true;
            }
        }
    }
    return false;
}

bool validarStock(int cantidad) {
    if (cantidad < 0) {
        printf("\033[0;31mLa cantidad debe ser un numero y no puede ser menor a 0\033[0m\n");
        return false;
    }
    return true;
}

bool validarPrecio(float precio) {
    if (isnan(precio) || precio <= 0) {
        printf("\033[0;31mEl precio debe ser un flotante y no puede ser menor a 0\033[0m\n");
        return false;
    }
    return true;
}

bool validarCodigo(char* codigo) {
    if(codigo == NULL || strlen(codigo) == 0) {
        printf("\033[0;31mEl codigo del libro no puede ser vacio\033[0m\n");
        return false;
    }
    if (strlen(codigo) != 3) {
        printf("\033[0;31mEl codigo solo puede ser de 3 digitos\033[0m\n");
        return false;
    }
    for (int i = 0; codigo[i] != '\0'; i++) { 
        if (!isdigit(codigo[i])) {
            printf("\033[0;31mEl codigo solo debe tener numeros\033[0m\n");
            return false; 
        }
    }
    for (int i = 0; i < stockLibros; i++) {
        if(strcmp(libros[i].codigo, codigo) == 0) {
            printf("\033[0;31mEl codigo ya ha sido asociado a otro libro\033[0m\n");
            return false;
        }
    }
    
    return true;
}

bool validarAutor(char* autor) {
    if (autor == NULL || strlen(autor) == 0) {
        printf("\033[0;31mEl nombre del autor del libro no puede estar vacio\033[0m\n");
        return false;
    }
    for (int i = 0; autor[i] != '\0'; i++) {
        if (!isalpha(autor[i]) && autor[i] != ' ') {
            printf("\033[0;31mEl nombre del autor solo debe contener letras\033[0m\n");
            return false;
        }
    }
    return true;
}

bool validarTitulo(char* titulo) {
    if (titulo == NULL || strlen(titulo) == 0) {
        printf("\033[0;31mEl titulo del libro no puede estar vacío\033[0m\n");
        return false;
    }
    for (int i = 0; titulo[i] != '\0'; i++) {
        if (!isalpha(titulo[i]) && titulo[i] != ' ') {
            printf("\033[0;31mEl titulo debe contener solo debe contener letras\033[0m\n");
            return false;
        }

    }
    return true;
}

void eliminarStockLibro(char* codigo, int cantidad) {
    for (int i = 0; i < stockLibros; i++) {
        if (strcmp(libros[i].codigo, codigo) == 0) {
            if (libros[i].stock >= cantidad) {
                libros[i].stock -= cantidad;
                guardarLibros(); 
                printf("Stock del libro reducido exitosamente\n");
            } else {
                printf("No hay stock suficiente del libro \n");
            }
            return; 
        }
    }
    printf("No se encontró ningún libro con el código %s.\n", codigo);
}

void agregarStockLibro(char* codigo, int cantidad) {
    for (int i = 0; i < stockLibros; i++) {
        if (strcmp(libros[i].codigo, codigo) == 0) {
            libros[i].stock += cantidad;
            guardarLibros(); 
            printf("Stock del libro aumentado exitosamente\n");
            return; 
        }
    }
    printf("No se encontró ningún libro con el código %s.\n", codigo);
}

void eliminarLibro(char* codigo) {
    for (int i = 0; i < stockLibros; i++) {
        if(strcmp(libros[i].codigo, codigo) == 0) {
            free(libros[i].titulo);
            free(libros[i].autor);
            free(libros[i].codigo);
            for (int j = i; j < stockLibros - 1; j++) {
                libros[j] = libros[j + 1];
            }
            stockLibros--;
            libros = realloc(libros, stockLibros * sizeof(Libro));
            guardarLibros();
            printf("Libro eliminado exitosamente.\n");
            return;  
        }
    }
}

void registrarLibro(char* titulo, char* autor, char* codigo, float precio, int stock) {

    libros = realloc(libros, (stockLibros +1) * sizeof(Libro));

    //Titulo del libro
    libros[stockLibros].titulo = malloc(strlen(titulo) +1);
    strcpy(libros[stockLibros].titulo,titulo);

    //Autor del libro
    libros[stockLibros].autor = malloc(strlen(autor) +1);
    strcpy(libros[stockLibros].autor,autor);

    //Codigo del libro
    libros[stockLibros].codigo = malloc(strlen(codigo) +1);
    strcpy(libros[stockLibros].codigo, codigo);

    //Precio del libro
    libros[stockLibros].precio = precio;

    //Cantidad en stock
    libros[stockLibros].stock = stock;
    stockLibros++;
    system("clear");
    printf("El libro ha sido registrado!\n");
    guardarLibros();

}
