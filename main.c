#include "headers/libros.h"
#include "headers/pedidos.h"
#include "headers/clientes.h"
#include "headers/locales.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define CLEAR system("clear")

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int admin(){
    printf("Menú de opciones administrativas:\n");
    printf("1- Registrar Libros\n");
    printf("2- Carga de inventario\n");
    printf("3- Registrar clientes\n");
    printf("4- Crear Pedido\n");
    printf("5- Estadisticas\n");
    printf("6- Regresar\n");
    printf("Seleccione una opción: ");
    int respuesta;
    if(scanf("%d", &respuesta)==1){
        limpiarBuffer();
        switch (respuesta)
        {
        case 1:
            CLEAR;
            registrarCliente();
            break;
        case 2:
            CLEAR;
            printf("carga de inventario\n");
            break;
        case 3:
            CLEAR;
            registrarCliente();
            break;
        case 4:
            CLEAR;
            printf("Crear pedido");
            break;
        case 5:
            CLEAR;
            printf("Estadisticas");
            break;
        case 6:
            CLEAR;
            return 0;
            break;
        default:
            CLEAR;
            printf("\033[0;31mOpción no válida. Por favor ingrese una de las opciones.\033[0m\n");
            admin();
            break;
        }
    } else{
        system("clear");
        limpiarBuffer();
        printf("\033[0;31mEntrada incorrecta. Por favor, ingresa un número\033[0m\n");
        admin();
    }
}

void registrarCliente() {
    
    char nombre[100];
    char cedula[100];
    char telefono[100];
    bool datosValidos = false;
    
    printf("=== REGISTRAR NUEVO CLIENTE ===\n");

    while (!datosValidos) {

        do {
            printf("Ingrese el nombre del cliente: ");
            fgets(nombre, sizeof(nombre), stdin);
            if ((strlen(nombre) > 0) && (nombre[strlen(nombre) - 1] == '\n')) {
                nombre[strlen(nombre) - 1] = '\0';
            }
        } while (!validarNombre(nombre));
        
   
        do {
            printf("Ingrese el numero de cedula (9 digitos): ");
            fgets(cedula, sizeof(cedula), stdin);
            if ((strlen(cedula) > 0) && (cedula[strlen(cedula) - 1] == '\n')) {
                cedula[strlen(cedula) - 1] = '\0';
            }
        } while (!validarCedula(cedula));
        

        do {
            printf("Ingrese el numero de telefono (8 digitos): ");
            fgets(telefono, sizeof(telefono), stdin);
            if ((strlen(telefono) > 0) && (telefono[strlen(telefono) - 1] == '\n')) {
                telefono[strlen(telefono) - 1] = '\0';
            }
        } while (!validarTelefono(telefono));
        
        datosValidos = true;
    }
    
    registrarClientes(nombre, cedula, telefono);
}


int general(){
    printf("Menú de opciones generales:\n");
    printf("1- Consulta de catálogo\n");
    printf("2- Consulta de cliente\n");
    printf("3- Regresar\n");
    printf("Seleccione una opción: ");

        char input[10];
        if(fgets(input, sizeof(input), stdin)) {  
            input[strcspn(input, "\n")] = 0;
            
            if(strlen(input) == 0) {
                CLEAR;
            }
            char respuesta = input[0];             
                                   

            if(respuesta == '1') {
                CLEAR;
                printf("Catalogo");
            } else if(respuesta == '2') {
                CLEAR;
                printf("cliente");
            } else if(respuesta == '3') {
                CLEAR;
                return 0;
            } else {
                CLEAR;
                printf("\033[0;31mOpción no válida. Por favor ingrese una de las opciones.\033[0m\n");
            }
        } else {
            limpiarBuffer();
            CLEAR;
            printf("\033[0;31mEntrada incorrecta. Por favor ingresa una opción válida\033[0m\n");
        }
    }



int main() {
    system("clear");
    while(true){
        printf("Bienvenido al sistema de Librería\n");
        printf("Menu principal:\n");
        printf("1- Opciones Administrativas\n");
        printf("2- Opciones Generales\n");
        printf("3- Salir\n");
        printf("Seleccione una opción: ");

        char input[10];
        if(fgets(input, sizeof(input), stdin)) {  
            input[strcspn(input, "\n")] = 0;
            
            if(strlen(input) == 0) {
                CLEAR;
                continue;
            }
            char respuesta = input[0];             
                      

            if(respuesta == '1') {
                CLEAR;
                admin();
            } else if(respuesta == '2') {
                CLEAR;
                general();
            } else if(respuesta == '3') {
                printf("¿Estás seguro que deseas salir (Y/N): ");
                char confirm[10];
                if(fgets(confirm, sizeof(confirm), stdin)) {
                    char res = confirm[0];
                    if(res=='n' || res=='N') continue;
                    else exit(0);
                }
            } else {
                CLEAR;
                printf("\033[0;31mOpción no válida. Por favor ingrese una de las opciones.\033[0m\n");
            }
        } else {
            limpiarBuffer();
            CLEAR;
            printf("\033[0;31mEntrada incorrecta. Por favor ingresa una opción válida\033[0m\n");
        }
    }

    return 0;
}
