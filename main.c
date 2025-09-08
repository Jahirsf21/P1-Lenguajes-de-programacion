#include "headers/libros.h"
#include "headers/pedidos.h"
#include "headers/clientes.h"
#include "headers/locales.h"
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

int menuLibros(){
    printf("Menú de Gestión de Libros:\n");
    printf("1- Registrar Libro\n");
    printf("2- Eliminar Libro\n");
    printf("3- Regresar\n");
    printf("Seleccione una opcion: ");

    char input[10];
        if(fgets(input, sizeof(input), stdin)) {  
            input[strcspn(input, "\n")] = 0;
            
            if(strlen(input) == 0) {
                CLEAR;
            }
            char respuesta = input[0];             
                      
            if(respuesta == '1') {
                CLEAR;
                menuRegistrarLibro();
            } else if(respuesta == '2') {
                if (stockLibros > 0) {
                    CLEAR;
                    menuEliminarLibro();
                } else {
                    CLEAR;
                    printf("\033[0;31mNo hay libros registrados para eliminar.\033[0m\n");
                    menuLibros();
                }
            } else if(respuesta == '3') {
                return 0;
            } else {
                CLEAR;
                printf("\033[0;31mOpción no válida. Por favor ingrese una de las opciones.\033[0m\n");
                menuLibros();
            }
        } else {
            limpiarBuffer();
            CLEAR;
            printf("\033[0;31mEntrada incorrecta. Por favor ingresa una opción válida\033[0m\n");
            menuLibros();
        }
    }

void menuEliminarLibro() {
    mostrarTodosLosLibros();
    char codigo[100]; 
    printf("=== ELIMINAR LIBRO ===\n");  
    while (true) {
        printf("Ingrese el codigo del libro que desea eliminar ( r ) para regresar: ");
        fgets(codigo, sizeof(codigo), stdin);
        if ((strlen(codigo) > 0) && (codigo[strlen(codigo)-1] == '\n')) {
            codigo[strlen(codigo)-1] = '\0';
        }

        if (strlen(codigo) == 1 && tolower(codigo[0]) == 'r') {
            CLEAR;
            printf("Regresando al menú anterior...\n");
            return;
        }

        if (!existeLibro(codigo)) {
            continue;  
        }
        if (libroAsociadoPedido(codigo)) {
            printf("\033[0;33mEl libro está asociado a un pedido y no puede ser eliminado.\033[0m\n");
            return;  
        }
        char confirmacion[10];
        while (true) {
            printf("¿Estás seguro que deseas eliminar este libro? (s/n): ");
            fgets(confirmacion, sizeof(confirmacion), stdin);

            if (strlen(confirmacion) > 0 && confirmacion[strlen(confirmacion)-1] == '\n') {
                confirmacion[strlen(confirmacion)-1] = '\0';
            }

            if (strlen(confirmacion) != 1 || (tolower(confirmacion[0]) != 's' && tolower(confirmacion[0]) != 'n')) {
                printf("\033[0;31mEntrada inválida. Por favor ingresa 's' o 'n'.\033[0m\n");
                continue;
            }

            if (tolower(confirmacion[0]) == 'n') {
                printf("Eliminación cancelada.\n");
                return;
            }
            break;
        }
        eliminarLibro(codigo);
        break; 
    }
}


int menuClientes(){
    printf("Menu de Gestión de Clientes:\n");
    printf("1- Registrar Cliente\n");
    printf("2- Eliminar Cliente\n");
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
                menuRegistrarCliente();
            } else if(respuesta == '2') {
                if (cantidadClientes > 0) {
                    CLEAR;
                    menuEliminarCliente();
                } else {
                    CLEAR;
                    printf("\033[0;31mNo hay clientes registrados para eliminar.\033[0m\n");
                    menuClientes();
                }
            } else if(respuesta == '3') {
                return 0;
            } else {
                CLEAR;
                printf("\033[0;31mOpción no válida. Por favor ingrese una de las opciones.\033[0m\n");
                menuClientes();
            }
        } else {
            limpiarBuffer();
            CLEAR;
            printf("\033[0;31mEntrada incorrecta. Por favor ingresa una opción válida\033[0m\n");
            menuClientes();
        }

}




void menuRegistrarCliente() {
    
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




void menuEliminarCliente() {
    mostrarTodosLosClientes();
    char cedula[100]; 
    printf("=== ELIMINAR CLIENTE ===\n");  
    while (true) {
        printf("Ingrese la cedula del cliente que desea eliminar ( r ) para regresar: ");
        fgets(cedula, sizeof(cedula), stdin);
        if ((strlen(cedula) > 0) && (cedula[strlen(cedula)-1] == '\n')) {
            cedula[strlen(cedula)-1] = '\0';
        }

        if (strlen(cedula) == 1 && tolower(cedula[0]) == 'r') {
            CLEAR;
            printf("Regresando al menú anterior...\n");
            return;
        }

        if (!existeCliente(cedula)) {
            continue;  
        }
        if (clienteTienePedidos(cedula)) {
            printf("\033[0;33mEl cliente está asociado a un pedido y no puede ser eliminado.\033[0m\n");
            return;  
        }
        char confirmacion[10];
        while (true) {
            printf("¿Estás seguro que deseas eliminar este cliente? (s/n): ");
            fgets(confirmacion, sizeof(confirmacion), stdin);

            if (strlen(confirmacion) > 0 && confirmacion[strlen(confirmacion)-1] == '\n') {
                confirmacion[strlen(confirmacion)-1] = '\0';
            }

            if (strlen(confirmacion) != 1 || (tolower(confirmacion[0]) != 's' && tolower(confirmacion[0]) != 'n')) {
                printf("\033[0;31mEntrada inválida. Por favor ingresa 's' o 'n'.\033[0m\n");
                continue;
            }

            if (tolower(confirmacion[0]) == 'n') {
                printf("Eliminación cancelada.\n");
                return;
            }
            break;
        }
        eliminarClientePorCedula(cedula);
        break; 
    }
}


















int menuPedidos(){
    printf("Menu de Gestión de Pedidos:\n");
    printf("1- Crear Pedido\n");
    printf("2- Eliminar Pedido\n");
    printf("3- Modificar Pedido\n");
    printf("4- Regresar");
    printf("Seleccione una opción: ");
    char input[10];
        if(fgets(input, sizeof(input), stdin)) {  
            input[strcspn(input, "\n")] = 0;
            
            if(strlen(input) == 0) {
                CLEAR;
            }
            char respuesta = input[0];             
            switch (respuesta)
            {
            case '1':
                CLEAR;
                printf("Crear pedido");
                break;
            case '2':
                CLEAR;
                printf("eliminar Pedido");
                break;
            case '3':
                CLEAR;
                printf("Modificar Pedido");
                break;
            default:
                CLEAR;
                printf("\033[0;31mOpción no válida. Por favor ingrese una de las opciones.\033[0m\n");
                menuPedidos();
                break;
            }    
        } else {
            limpiarBuffer();
            CLEAR;
            printf("\033[0;31mEntrada incorrecta. Por favor ingresa una opción válida\033[0m\n");
            menuPedidos();
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
            printf("total ventas");
            break;
        case '2':
            CLEAR;
            printf("clientes mas pedidos\n");
            break;
        case '3':
            CLEAR;
            printf("libros mas vendidos");
            break;
        case '4':
            CLEAR;
            printf("autores mas ventas");
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
            printf("carga de inventario\n");
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
}


void menuRegistrarLibro() {
    char titulo[100];    
    char autor[100];    
    char codigo[100];   
    float precio;
    int stock;
    bool datosValidos = false;
    
    printf("=== REGISTRAR NUEVO LIBRO ===\n");  
    
    while (!datosValidos) {
        // Solicitar título
        do {
            printf("Ingrese el titulo del libro: ");
            fgets(titulo, sizeof(titulo), stdin);
            if ((strlen(titulo) > 0) && (titulo[strlen(titulo)-1] == '\n')) {
                titulo[strlen(titulo)-1] = '\0';
            }
        } while(!validarTitulo(titulo));
        
        // Solicitar autor
        do {
            printf("Ingrese el autor del libro: "); 
            fgets(autor, sizeof(autor), stdin);
            if ((strlen(autor) > 0) && (autor[strlen(autor)-1] == '\n')) {
                autor[strlen(autor)-1] = '\0';
            }
        } while(!validarAutor(autor));
        
        // Solicitar código
        do {
            printf("Ingrese el codigo del libro: ");  
            fgets(codigo, sizeof(codigo), stdin);
            if ((strlen(codigo) > 0) && (codigo[strlen(codigo)-1] == '\n')) {
                codigo[strlen(codigo)-1] = '\0';
            }
        } while(!validarCodigo(codigo));

        do {
            printf("Ingrese el precio del libro: ");
            if (scanf("%f", &precio) != 1) {
                printf("\033[0;31mPor favor ingrese un precio válido\033[0m\n");
                limpiarBuffer();
                precio = -1; 
            } else {
                limpiarBuffer();
            }
        } while(!validarPrecio(precio));
        
        do {
            printf("Ingrese la cantidad en stock: ");
            if (scanf("%d", &stock) <= 0) {
                printf("\033[0;31mPor favor ingrese una cantidad válida (Mayor a 0)\033[0m\n");
                limpiarBuffer();
                stock = -1; 
            } else {
                limpiarBuffer();
            }
        } while(!validarStock(stock));
        
        datosValidos = true;
    }
    registrarLibro(titulo, autor, codigo, precio, stock);
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

int main() {
    
    system("clear");
    cargarClientes();
    cargarLibros();
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
