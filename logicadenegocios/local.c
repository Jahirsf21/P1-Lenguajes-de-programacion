#include "../headers/clientes.h"
#include "../headers/pedidos.h"
#include "../headers/local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define CLEAR system("clear")

Local* local = NULL;

void cargarLocal() {
    FILE* archivo = fopen("store/local.json", "r");
    if (archivo == NULL) {
        printf("Error: no se pudo abrir store/local.json\n");
        return;
    }

    char buffer[256];
    char nombre[100] = "";
    char telefono[100] = "";
    char cedula[100] = "";
    char inicio[20] = "";
    char fin[20] = "";
    int siguiente = 0;
    char usuario[100] = "";
    char password[100] = "";

    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        if (strstr(buffer, "\"nombre\":")) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(nombre, inicioStr, finStr - inicioStr);
            nombre[finStr - inicioStr] = '\0';
        }
        else if (strstr(buffer, "\"telefono\":")) {
            char* inicioStr = strchr(buffer, ':') + 1;
            while (*inicioStr == ' ' || *inicioStr == '\"') inicioStr++;
            char* finStr = strpbrk(inicioStr, "\",\n");
            strncpy(telefono, inicioStr, finStr - inicioStr);
            telefono[finStr - inicioStr] = '\0';
        }
        else if (strstr(buffer, "\"cedula\":")) {
            char* inicioStr = strchr(buffer, ':') + 1;
            while (*inicioStr == ' ' || *inicioStr == '\"') inicioStr++;
            char* finStr = strpbrk(inicioStr, "\",\n");
            strncpy(cedula, inicioStr, finStr - inicioStr);
            cedula[finStr - inicioStr] = '\0';
        }
        else if (strstr(buffer, "\"inicio\":")) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(inicio, inicioStr, finStr - inicioStr);
            inicio[finStr - inicioStr] = '\0';
        }
        else if (strstr(buffer, "\"final\":")) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(fin, inicioStr, finStr - inicioStr);
            fin[finStr - inicioStr] = '\0';
        }
        else if (strstr(buffer, "\"siguiente\":")) {
            char* inicioStr = strchr(buffer, ':') + 1;
            siguiente = atoi(inicioStr);
        }
        else if (strstr(buffer, "\"usuario\":")) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(usuario, inicioStr, finStr - inicioStr);
            usuario[finStr - inicioStr] = '\0';
        }
        else if (strstr(buffer, "\"password\":")) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(password, inicioStr, finStr - inicioStr);
            password[finStr - inicioStr] = '\0';
        }
    }

    fclose(archivo);

    local = malloc(sizeof(Local));
    local->nombre = strdup(nombre);
    local->telefono = strdup(telefono);
    local->cedulaJuridica = strdup(cedula);
    local->horarioInicio = strdup(inicio);
    local->horarioFin = strdup(fin);
    local->sigPedido = siguiente;

    
    local->usuario = malloc(sizeof(Usuario));
    local->usuario->nombre = strdup(usuario);
    local->usuario->password = strdup(password);
}