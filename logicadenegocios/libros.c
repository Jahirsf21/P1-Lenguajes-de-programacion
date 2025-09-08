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

char* libroToString(Libro *libro) {
    int largo = strlen(libro->titulo) + strlen(libro->autor) + strlen(libro->codigo) + 100;
    char* resultado = malloc(largo);
    snprintf(resultado, largo, "Titulo: %s | Autor: %s | Codigo: %s | Precio: %.2f | Cantidad en stock: %d",libro->titulo, libro->autor, libro->codigo, libro->precio, libro->stock); 
    return resultado;
}

void mostrarTodosLosLibros() {
    printf("=== LISTA DE LIBROS ===\n");
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
    printf("Total de titulos: %d\n", stockLibros);
    printf("==================================\n");
}
void cargarLibros() {
    FILE* archivo = fopen("store/libros.json", "r");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        if (strstr(buffer, "{")) {
            char titulo[100] = "";
            char autor[100] = "";
            char codigo[100] = "";
            float precio = 0.0;
            int stock = 0;

            // Leer Título
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"titulo\":")) {
                char* inicio = strchr(buffer, ':') + 3;
                char* fin = strrchr(buffer, '\"');
                if (inicio && fin && fin > inicio) {
                    strncpy(titulo, inicio, fin - inicio);
                    titulo[fin - inicio] = '\0';
                }
            }
            
            // Leer Autor
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"autor\":")) {
                char* inicio = strchr(buffer, ':') + 3;
                char* fin = strrchr(buffer, '\"');
                if (inicio && fin && fin > inicio) {
                    strncpy(autor, inicio, fin - inicio);
                    autor[fin - inicio] = '\0';
                }
            }
            
            // Leer Código
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"codigo\":")) {
                char* inicio = strchr(buffer, ':') + 3;
                char* fin = strrchr(buffer, '\"');
                if (inicio && fin && fin > inicio) {
                    strncpy(codigo, inicio, fin - inicio);
                    codigo[fin - inicio] = '\0';
                }
            }

            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"precio\":")) {
                char* inicio = strchr(buffer, ':') + 1;
                precio = atof(inicio);
            }

            // Leer Stock 
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"stock\":")) {
                char* inicio = strchr(buffer, ':') + 1;
                stock = atoi(inicio);
            }

            if (strlen(titulo) > 0 && strlen(autor) > 0 && strlen(codigo) > 0) {
                libros = realloc(libros, (stockLibros + 1) * sizeof(Libro));

                libros[stockLibros].titulo = malloc(strlen(titulo) + 1);
                strcpy(libros[stockLibros].titulo, titulo);

                libros[stockLibros].autor = malloc(strlen(autor) + 1);
                strcpy(libros[stockLibros].autor, autor);

                libros[stockLibros].codigo = malloc(strlen(codigo) + 1);
                strcpy(libros[stockLibros].codigo, codigo);

                libros[stockLibros].precio = precio;
                libros[stockLibros].stock = stock;
                
                stockLibros++;
            }
        }
    }
    fclose(archivo);
}

void guardarLibros() {
    FILE* archivo = fopen("store/libros.json", "w");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo para guardar los libros.\n");
        return;
    }
    fprintf(archivo, "[\n");
    for (int i = 0; i < stockLibros; i++) {
        fprintf(archivo, "    {\n");
        fprintf(archivo, "        \"titulo\": \"%s\",\n", libros[i].titulo);
        fprintf(archivo, "        \"autor\": \"%s\",\n", libros[i].autor);
        fprintf(archivo, "        \"codigo\": \"%s\",\n", libros[i].codigo);
        fprintf(archivo, "        \"precio\": %.2f,\n", libros[i].precio);
        fprintf(archivo, "        \"stock\": %d\n", libros[i].stock);
        fprintf(archivo, "    }");
        if (i < stockLibros - 1) {
            fprintf(archivo, ",\n");
        } else {
            fprintf(archivo, "\n");  
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
        printf("\033[0;31mEl precio debe ser un numero y no puede ser menor a 0\033[0m\n");
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
            printf("\033[0;31mEl titulo solo debe contener letras\033[0m\n");
            return false;
        }

    }
    return true;
}

bool existeLibro(char* codigo) {
    if (codigo == NULL || strlen(codigo) == 0) {
        printf("\033[0;31mDebes ingresar el codigo del libro a buscar\033[0m\n");
        return false;
    }
    for (int i = 0; i < stockLibros; i++) {
        if (strcmp(libros[i].codigo, codigo) == 0) {
            return true; 
        }
    }
    printf("\033[0;31mNo existe un libro con ese codigo\033[0m\n");
    return false; 
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
    int indice = -1;
    for (int i = 0; i < stockLibros; i++) {
        if(strcmp(libros[i].codigo, codigo) == 0) {
            indice = i;
            break;
        }
    }
    
    if (indice != -1) {
        liberarLibro(&libros[indice]);
        for (int i = indice; i < stockLibros - 1; i++) {
            libros[i] = libros[i + 1];
        }
        stockLibros--;
        if (stockLibros > 0) {
            libros = realloc(libros, stockLibros * sizeof(Libro));
        } else {
            free(libros);
            libros = NULL;
        }
        guardarLibros();
        printf("Libro eliminado exitosamente.\n");
    }
}

void registrarLibro(char* titulo, char* autor, char* codigo, float precio, int stock) {
    libros = realloc(libros, (stockLibros + 1) * sizeof(Libro));

    libros[stockLibros].titulo = malloc(strlen(titulo) + 1);
    strcpy(libros[stockLibros].titulo, titulo);

    libros[stockLibros].autor = malloc(strlen(autor) + 1);
    strcpy(libros[stockLibros].autor, autor);

    libros[stockLibros].codigo = malloc(strlen(codigo) + 1);
    strcpy(libros[stockLibros].codigo, codigo);

    libros[stockLibros].precio = precio;
    libros[stockLibros].stock = stock;
    
    stockLibros++;
    system("clear");
    printf("El libro ha sido registrado!\n");
    guardarLibros();
}