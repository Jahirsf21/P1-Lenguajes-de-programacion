#include "headers/libros.h"
#include "headers/pedidos.h"
#include "headers/clientes.h"
#include "headers/local.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define CLEAR system("clear")

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menuLibros() {
    while (true) {
        CLEAR;
        printf("Menú de Gestión de Libros:\n");
        printf("1- Registrar Libro\n");
        printf("2- Eliminar Libro\n");
        printf("3- Regresar\n");
        printf("Seleccione una opcion: ");

        char input[10];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;

        switch (input[0]) {
            case '1':
                menuRegistrarLibro();
                break;
            case '2':
                if (stockLibros > 0) {
                    CLEAR;
                    menuEliminarLibro();
                } else {
                    printf("\033[0;31mNo hay libros registrados para eliminar.\033[0m\n");
                    getchar();
                }
                break;
            case '3':
                CLEAR;
                return;
            default:
                printf("\033[0;31mOpción no válida.\033[0m\n");
                getchar();
                break;
        }
    }
}

void menuClientes() {
    while (true) {
        CLEAR;
        printf("Menu de Gestión de Clientes:\n");
        printf("1- Registrar Cliente\n");
        printf("2- Eliminar Cliente\n");
        printf("3- Regresar\n");
        printf("Seleccione una opción: ");

        char input[10];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;

        switch (input[0]) {
            case '1':
                menuRegistrarCliente();
                break;
            case '2':
                if (cantidadClientes > 0) {
                    menuEliminarCliente();
                } else {
                    printf("\033[0;31mNo hay clientes registrados para eliminar.\033[0m\n");
                    getchar();
                }
                break;
            case '3':
                return;
            default:
                printf("\033[0;31mOpción no válida.\033[0m\n");
                getchar();
                break;
        }
    }
}

void menuPedidos() {
    while (true) { 
        CLEAR;
        printf("Menu de Gestión de Pedidos:\n");
        printf("1- Crear Pedido\n");
        printf("2- Eliminar Pedido\n");
        printf("3- Modificar Pedido\n");
        printf("4- Mostrar todos los pedidos\n");
        printf("5- Buscar pedidos por fecha\n");
        printf("6- Regresar\n");
        printf("Seleccione una opción: ");

        char input[10];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;

        switch (input[0]) {
            case '1':
                CLEAR;
                menuCrearPedido(); 
                break;
            case '2':
                CLEAR;
                menuEliminarPedido(); 
                break;
            case '3':
                CLEAR;
                menuModificarPedido();
                break;
            case '4':
                CLEAR;
                mostrarTodosLosPedidos();
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
            case '5': {
                CLEAR;
                mostrarTodosLosPedidos();
                char fecha[20];
                printf("Ingrese la fecha a buscar (dd/mm/aaaa) (o r para regresar): ");
                fgets(fecha, sizeof(fecha), stdin);
                fecha[strcspn(fecha, "\n")] = 0;
                if (strcmp(fecha, "r") == 0 || strcmp(fecha, "R") == 0 ){
                    menuPedidos();
                } else {
                    if (validarFecha(fecha)) {
                        mostrarPedidoPorFecha(fecha);
                    } else {
                        printf("\033[0;31mFormato de fecha no válido.\033[0m\n");
                    }
                    printf("\nPresione Enter para continuar...");
                    getchar();
                    break;
                }
            }
            case '6':
                return; 
            default:
                printf("\033[0;31mOpción no válida.\033[0m\n");
                getchar();
                break;
        }
    }
}

int menuEstadisticas(){
    printf("Menú de estadísticas:\n");
    printf("1- Total de ventas\n");
    printf("2- Clientes con más pedidos\n");
    printf("3- Libros más vendidos\n");
    printf("4- Autores con mas ventas por año\n");
    printf("5- Regresar\n");
    printf("Seleccione una opción: ");
    char input[10];
    int respuesta;
    if(fgets(input, sizeof(input), stdin)){
        input[strcspn(input, "\n")] = 0;
            
        if(strlen(input) == 0) {
            CLEAR;
        }
        char respuesta = input[0];
        switch (respuesta)
        {
        case '1':
            CLEAR;
            
            totalPedidos();
            break;
        case '2':
            CLEAR;
            ClientesConMasPedidos();
            break;
        case '3':
            CLEAR;
            LibrosMasVendidosMenu();
            break;
        case '4':
            CLEAR;
            autorConMasVentasAnio();
            break;
        case '5':
            CLEAR;
            return 0;
            break;
        default:
            CLEAR;
            printf("\033[0;31mOpción no válida. Por favor ingrese una de las opciones.\033[0m\n");
            menuEstadisticas();
            break;
        }
    } else{
        system("clear");
        limpiarBuffer();
        printf("\033[0;31mEntrada incorrecta. Por favor, ingresa un número\033[0m\n");
        menuEstadisticas();
    }

}

int admin(){
    while(true){
    printf("Menú de opciones administrativas:\n");
    printf("1- Gestión de Libros\n");
    printf("2- Carga de inventario\n");
    printf("3- Gestión de Clientes\n");
    printf("4- Gestión de Pedidos\n");
    printf("5- Estadisticas\n");
    printf("6- Regresar\n");
    printf("Seleccione una opción: ");
    char input[10];
    int respuesta;
    if(fgets(input, sizeof(input), stdin)){
        input[strcspn(input, "\n")] = 0;
            
        if(strlen(input) == 0) {
            CLEAR;
        }
        char respuesta = input[0];
        switch (respuesta)
        {
        case '1':
            CLEAR;
            menuLibros();
            break;
        case '2': 
            CLEAR;
            menuCargaInventario();
            break;
        case '3':
            CLEAR;
            menuClientes();
            break;
        case '4':
            CLEAR;
            menuPedidos();
            break;
        case '5':
            CLEAR;
            menuEstadisticas();
            break;
        case '6':
            CLEAR;
            return 0;
            break;
        default:
            CLEAR;
            printf("\033[0;31mOpción no válida. Por favor ingrese una de las opciones.\033[0m\n");
            continue;
            break;
        }
    } else{
        system("clear");
        limpiarBuffer();
        printf("\033[0;31mEntrada incorrecta. Por favor, ingresa un número\033[0m\n");
        continue;
    }
}
}




int general(){
    while(true){
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
                printf("\033[0;31mEntrada incorrecta. Por favor ingresa una opción válida\033[0m\n");
                continue;
            }
            char respuesta = input[0];             
                                   
            if(respuesta == '1') {
                CLEAR;
                mostrarTodosLosLibros();
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
}

int login() {
    CLEAR;
    char usuario[100];
    char password[100];

    while (true) {
        printf("Ingrese el nombre de usuario (r para regresar): ");
        if (!fgets(usuario, sizeof(usuario), stdin)) continue;
        usuario[strcspn(usuario, "\n")] = 0;

        if (strcmp(usuario, "r") == 0 || strcmp(usuario, "R") == 0) {
            CLEAR;
            return 0; 
        }
        if (strlen(usuario) == 0) {
            CLEAR;
            printf("\033[0;31mEntrada incorrecta. Por favor ingresa un usuario válido\033[0m\n");
            continue;
        }

        printf("Ingrese la contraseña: ");
        if (!fgets(password, sizeof(password), stdin)) continue;
        password[strcspn(password, "\n")] = 0;

        if (strlen(password) == 0) {
            CLEAR;
            printf("\033[0;31mEntrada incorrecta. Por favor ingresa una contraseña válida\033[0m\n");
            continue;
        }

        if (comprobarUsuario(usuario, password)) {
            return 1; 
        } else {
            CLEAR;
            printf("\033[0;31mNombre de usuario o contraseña incorrecta.\033[0m\n");
        }
    }
}


int main() {
    cargarClientes();
    cargarLibros();
    cargarPedidos();
    cargarLocal();
    while(true){
        CLEAR;
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
                printf("\033[0;31mEntrada incorrecta. Por favor ingresa una opción válida\033[0m\n");
                continue;
            }
            char respuesta = input[0];             
                      
            if(respuesta == '1') {
                CLEAR;
                if(login()) CLEAR; admin();
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
