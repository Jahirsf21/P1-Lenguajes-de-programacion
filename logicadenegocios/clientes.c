#include "../headers/clientes.h"
#include "../headers/pedidos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define CLEAR system("clear")

Cliente* clientes = NULL;
int cantidadClientes = 0;

void liberarCliente(Cliente* cliente) {
    if (cliente != NULL) {
        if (cliente->nombre != NULL) {
            free(cliente->nombre);
            cliente->nombre = NULL;
        }
        if (cliente->cedula != NULL) {
            free(cliente->cedula);
            cliente->cedula = NULL;
        }
        if (cliente->telefono != NULL) {
            free(cliente->telefono);
            cliente->telefono = NULL;
        }
    }
}

void liberarTodosLosClientes() {
    if (clientes != NULL) {
        for (int i = 0; i < cantidadClientes; i++) {
            liberarCliente(&clientes[i]);
        }
        free(clientes);
        clientes = NULL;
        cantidadClientes = 0;
    }
}
void eliminarCliente(int indice) {
    if (indice < 0 || indice >= cantidadClientes) {
        printf("\033[0;31mÍndice de cliente inválido\033[0m\n");
        return;
    }
    liberarCliente(&clientes[indice]);
    for (int i = indice; i < cantidadClientes - 1; i++) {
        clientes[i] = clientes[i + 1];
    }
    cantidadClientes--;
    if (cantidadClientes > 0) {
        clientes = realloc(clientes, cantidadClientes * sizeof(Cliente));
    } else {
        free(clientes);
        clientes = NULL;
    }
    
    printf("\033[0;32mCliente eliminado exitosamente\033[0m\n");
    guardarClientes();
    printf("Presiona enter para continuar...");
    getchar();
}

void eliminarClientePorCedula(char* cedula) {
    for (int i = 0; i < cantidadClientes; i++) {
        if (strcmp(clientes[i].cedula, cedula) == 0) {
            eliminarCliente(i);
            return;
        }
    }
}


void mostrarTodosLosClientes() {
    printf("=== LISTA COMPLETA DE CLIENTES ===\n");
    if (cantidadClientes == 0) {
        printf("No hay clientes registrados.\n");
    } else {
        for (int i = 0; i < cantidadClientes; i++) {
            char* mostrarClientes = clienteToString(&clientes[i]);
            if (mostrarClientes!= NULL) {
                printf("%d. %s\n", i+1, mostrarClientes);
                free(mostrarClientes);
            }
        }
    }
    printf("Total de clientes: %d\n", cantidadClientes);
    printf("==================================\n");
}


char* clienteToString(Cliente *cliente) {
	int largo = strlen(cliente->nombre) + strlen(cliente->cedula) + strlen(cliente->telefono) + 50;
	char* resultado = malloc(largo);
	if (resultado == NULL) {
		return NULL;
	}
	snprintf(resultado, largo,  "Nombre: %s | Cédula: %s | Teléfono: %s",cliente->nombre, cliente->cedula, cliente->telefono);
	return resultado;
}

void cargarClientes() {
    FILE* archivo = fopen("store/clientes.json", "r");

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        if (strstr(buffer, "{")) {
            char nombre[100] = "";
            char cedula[100] = "";
            char telefono[100] = "";
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"nombre\":")) {
                char* inicio = strchr(buffer, ':') + 3; 
                char* fin = strrchr(buffer, '\"'); 
                if (inicio && fin && fin > inicio) {
                    strncpy(nombre, inicio, fin - inicio);
                    nombre[fin - inicio] = '\0';
                }
            }
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"cedula\":")) {
                char* inicio = strchr(buffer, ':') + 3;
                char* fin = strrchr(buffer, '\"');
                if (inicio && fin && fin > inicio) {
                    strncpy(cedula, inicio, fin - inicio);
                    cedula[fin - inicio] = '\0';
                }
            }
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"telefono\":")) {
                char* inicio = strchr(buffer, ':') + 3;
                char* fin = strrchr(buffer, '\"');
                if (inicio && fin && fin > inicio) {
                    strncpy(telefono, inicio, fin - inicio);
                    telefono[fin - inicio] = '\0';
                }
            }
            
            if (strlen(nombre) > 0 && strlen(cedula) > 0 && strlen(telefono) > 0) {
                clientes = realloc(clientes, (cantidadClientes + 1) * sizeof(Cliente));

                clientes[cantidadClientes].nombre = malloc(strlen(nombre) + 1);
                strcpy(clientes[cantidadClientes].nombre, nombre);

                clientes[cantidadClientes].cedula = malloc(strlen(cedula) + 1);
                strcpy(clientes[cantidadClientes].cedula, cedula);

                clientes[cantidadClientes].telefono = malloc(strlen(telefono) + 1);
                strcpy(clientes[cantidadClientes].telefono, telefono);
                
                cantidadClientes++;
            }
        }
    }
    fclose(archivo);
}

bool existeCliente(char* cedula) {
    if (cedula == NULL || strlen(cedula) == 0) {
        printf("\033[0;31mDebes ingresar la cedula del cliente a buscar\033[0m\n");
        return false;
    }
    for (int i = 0; i < cantidadClientes; i++) {
        if (strcmp(clientes[i].cedula, cedula) == 0) {
            return true; 
        }
    }
    printf("\033[0;31mNo existe un cliente con esa cedula\033[0m\n");
    return false; 
}


bool clienteTienePedidos(char* cedula) {
    for (int i = 0; i < cantidadPedidos; i++) {
        if (strcmp(pedidos[i].cliente->cedula, cedula) == 0) {
            return true;
        }
    }
    return false;
}

bool validarTelefono(char* telefono) {
    if (telefono == NULL || strlen(telefono) == 0) {
        printf("\033[0;31mEl numero de telefono no puede estar vacio\033[0m\n");
        return false;
    }
    if (strlen(telefono) != 8) {
        printf("\033[0;31mEl numero de telefono solo puede tener 8 digitos\033[0m\n");
        return false;
    }
    for (int i = 0; telefono[i] != '\0'; i++) {
        if (!isdigit(telefono[i])) {
            printf("\033[0;31mEl numero de telefono solo debe contener numeros\033[0m\n");
            return false;
        }
    }
    return true;
}

bool validarCedula(char* cedula) {
    if (cedula == NULL || strlen(cedula) == 0) {
        printf("\033[0;31mEl numero de cedula no puede estar vacio\033[0m\n");
        return false;
    }
    if (strlen(cedula) != 9) {
        printf("\033[0;31mLa cedula solo puede tener 9 digitos\033[0m\n");
        return false;
    }
    for (int i = 0; cedula[i] != '\0'; i++) {
        if (!isdigit(cedula[i])) {
            printf("\033[0;31mLa cedula solo debe contener numeros\033[0m\n");
            return false;
        }
    }
    for (int  i = 0; i < cantidadClientes; i++) {
    	if (strcmp(clientes[i].cedula, cedula) == 0) {
   			printf("\033[0;31mLa cedula ya ha sido asociada a otro cliente\033[0m\n");
    		return false;
    	}
    }
    return true;
}

bool validarNombre(char* nombre) {
    if (nombre == NULL || strlen(nombre) == 0) {
        printf("\033[0;31mEl nombre no puede estar vacío\033[0m\n");
        return false;
    }
    for (int i = 0; nombre[i] != '\0'; i++) {
        if (!isalpha(nombre[i]) && nombre[i] != ' ') {
            printf("\033[0;31mEl nombre solo puede contener letras y espacios\033[0m\n");
            return false;
        }
    }
    return true;
}

void guardarClientes() {
    FILE* archivo = fopen("store/clientes.json", "w");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo para guardar clientes.\n");
        return;
    }
    fprintf(archivo, "[\n");
    for (int i = 0; i < cantidadClientes; i++) {
        fprintf(archivo, "    {\n");
        fprintf(archivo, "        \"nombre\": \"%s\",\n", clientes[i].nombre);
        fprintf(archivo, "        \"cedula\": \"%s\",\n", clientes[i].cedula);
        fprintf(archivo, "        \"telefono\": \"%s\"\n", clientes[i].telefono);
        
        fprintf(archivo, "    }");
        if (i < cantidadClientes - 1) {
            fprintf(archivo, ",\n"); 
        } else {
            fprintf(archivo, "\n");
        }
    }
    fprintf(archivo, "]\n");
    fclose(archivo);
}

void registrarClientes(char* nombre, char* cedula, char* telefono) {
	clientes = realloc(clientes, (cantidadClientes + 1) * sizeof(Cliente));

	clientes[cantidadClientes].nombre = malloc(strlen(nombre) + 1);
	strcpy(clientes[cantidadClientes].nombre, nombre);

	clientes[cantidadClientes].cedula = malloc(strlen(cedula) + 1);
	strcpy(clientes[cantidadClientes].cedula, cedula);

	clientes[cantidadClientes].telefono = malloc(strlen(telefono) + 1);
	strcpy(clientes[cantidadClientes].telefono, telefono);

	cantidadClientes++;
	printf("El cliente ha sido registrado!\n");
    char* cliente = clienteToString(&clientes[cantidadClientes-1]);
    printf("%s\n", cliente);
	guardarClientes();
    printf("Presiona enter para continuar...\n");
    getchar();
}


void menuRegistrarCliente() {
    CLEAR;
    char nombre[100];
    char cedula[100];
    char telefono[100];
    bool datosValidos = false;
    printf("=== REGISTRAR NUEVO CLIENTE ===\n");
    mostrarTodosLosClientes();
    while (!datosValidos) {

        do {
            printf("Ingrese el nombre del cliente (c para cancelar): ");
            fgets(nombre, sizeof(nombre), stdin);
            if ((strlen(nombre) > 0) && (nombre[strlen(nombre) - 1] == '\n')) {
                nombre[strlen(nombre) - 1] = '\0';
            }
            if (strcmp(nombre,"c")==0 || strcmp(nombre,"C") == 0) {
                return;
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
    CLEAR;
    registrarClientes(nombre, cedula, telefono);
}


void menuEliminarCliente() {
    CLEAR;
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
        CLEAR;
        eliminarClientePorCedula(cedula);
        break; 
    }
}



