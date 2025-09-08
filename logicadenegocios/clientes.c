#include "../headers/clientes.h"
#include "../headers/pedidos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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
}

void eliminarClientePorCedula(char* cedula) {
    for (int i = 0; i < cantidadClientes; i++) {
        if (strcmp(clientes[i].cedula, cedula) == 0) {
            eliminarCliente(i);
            return;
        }
    }
    printf("\033[0;31mNo se encontró un cliente con esa cédula\033[0m\n");
}


void mostrarTodosLosClientes() {
    printf("\n=== LISTA COMPLETA DE CLIENTES ===\n");
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
	if (archivo == NULL) {
		cantidadClientes = 0;
		return;
	}
	char buffer[100];
	char nombre[100];
	char cedula[100];
	char telefono[100];
	fgets(buffer, sizeof(buffer),archivo);
	while (fgets(buffer,sizeof(buffer),archivo)) {

		// Para el nombre
		if (strstr(buffer, "\"nombre\":")) {

			char* inicio = strchr(buffer, '\"');
			inicio = strchr(inicio + 1, '\"') + 1;
			char* fin = strchr(inicio, '\"');

			int largo = fin - inicio;
			strncpy(nombre, inicio, largo);
			nombre[largo] = '\0';
		}
		// Para la cedula
		if (strstr(buffer, "\"cedula\":")) {

			char* inicio = strchr(buffer, '\"');
			inicio = strchr(inicio + 1, '\"') + 1;
			char* fin = strchr(inicio, '\"');

			int largo = fin - inicio;
			strncpy(cedula, inicio, largo);
			cedula[largo] = '\0';
		}
		if (strstr(buffer, "\"telefono\":")) {

			char* inicio = strchr(buffer, '\"');
			inicio = strchr(inicio + 1, '\"') + 1;
			char* fin = strchr(inicio, '\"');

			int largo = fin - inicio;
			strncpy(telefono, inicio, largo);
			telefono[largo] = '\0';

			clientes = realloc(clientes, (cantidadClientes + 1) * sizeof(Cliente));

			//Para el nombre
			clientes[cantidadClientes].nombre = malloc(strlen(nombre) + 1);
			strcpy(clientes[cantidadClientes].nombre, nombre);

			//Para la cedula
			clientes[cantidadClientes].cedula = malloc(strlen(cedula) + 1);
			strcpy(clientes[cantidadClientes].cedula, cedula);

			//Para el telefono
			clientes[cantidadClientes].telefono = malloc(strlen(telefono) + 1);
			strcpy(clientes[cantidadClientes].telefono, telefono);
			cantidadClientes++;
		}
	}
	fclose(archivo);
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
	fprintf(archivo, "[\n");
	for (int i = 0; i < cantidadClientes; i++) {
	    fprintf(archivo, "    {\n");
	    fprintf(archivo, "        \"nombre\": \"%s\",\n", clientes[i].nombre);
		fprintf(archivo, "        \"cedula\": \"%s\",\n", clientes[i].cedula);
		fprintf(archivo, "        \"telefono\": \"%s\"\n", clientes[i].telefono);
	    if ( i < cantidadClientes -1) {
            fprintf(archivo, "    },\n");
        } else {
            fprintf(archivo, "    }\n");
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
	guardarClientes();
}











