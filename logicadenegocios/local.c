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
    int leyendoUsuario = 0;  

    local = malloc(sizeof(Local));
    local->usuarios = NULL;       
    local->cantUsuarios = 0;      

    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        if (strstr(buffer, "\"nombre\":")) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(nombre, inicioStr, finStr - inicioStr);
            nombre[finStr - inicioStr] = '\0';
            local->nombre = strdup(nombre);
        }
        else if (strstr(buffer, "\"telefono\":")) {
            char* inicioStr = strchr(buffer, ':') + 1;
            while (*inicioStr == ' ' || *inicioStr == '\"') inicioStr++;
            char* finStr = strpbrk(inicioStr, "\",\n");
            strncpy(telefono, inicioStr, finStr - inicioStr);
            telefono[finStr - inicioStr] = '\0';
            local->telefono = strdup(telefono);
        }
        else if (strstr(buffer, "\"cedula\":")) {
            char* inicioStr = strchr(buffer, ':') + 1;
            while (*inicioStr == ' ' || *inicioStr == '\"') inicioStr++;
            char* finStr = strpbrk(inicioStr, "\",\n");
            strncpy(cedula, inicioStr, finStr - inicioStr);
            cedula[finStr - inicioStr] = '\0';
            local->cedulaJuridica = strdup(cedula);
        }
        else if (strstr(buffer, "\"horario\"")) {
            
        }
        else if (strstr(buffer, "\"inicio\":")) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(inicio, inicioStr, finStr - inicioStr);
            inicio[finStr - inicioStr] = '\0';
            local->horarioInicio = strdup(inicio);
        }
        else if (strstr(buffer, "\"final\":")) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(fin, inicioStr, finStr - inicioStr);
            fin[finStr - inicioStr] = '\0';
            local->horarioFin = strdup(fin);
        }

        else if (strstr(buffer, "\"siguiente\":")) {
            char* inicioStr = strchr(buffer, ':') + 1;
            siguiente = atoi(inicioStr);
            local->sigPedido = siguiente;
        }
        else if (strstr(buffer, "\"usuario\":")) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(usuario, inicioStr, finStr - inicioStr);
            usuario[finStr - inicioStr] = '\0';
            leyendoUsuario = 1;
        }
        else if (strstr(buffer, "\"password\":") && leyendoUsuario) {
            char* inicioStr = strchr(buffer, ':') + 3;
            char* finStr = strrchr(buffer, '\"');
            strncpy(password, inicioStr, finStr - inicioStr);
            password[finStr - inicioStr] = '\0';

            
            local->usuarios = realloc(local->usuarios, (local->cantUsuarios + 1) * sizeof(Usuario));
            local->usuarios[local->cantUsuarios].nombre = strdup(usuario);
            local->usuarios[local->cantUsuarios].password = strdup(password);
            local->cantUsuarios++;

            leyendoUsuario = 0;
        }
    }

    fclose(archivo);
}

void toStringLocal(){
    printf("Nombre del local: %s\n", local->nombre);
    printf("Número de teléfono: %s\n", local->telefono);
    printf("Cédula Jurídica: %s\n", local->cedulaJuridica);
    printf("Horario: De %s a %s\n", local->horarioInicio, local->horarioFin);
    printf("Usuarios:\n");
    verTodosLosUsuarios();
}

void toStringUsuario(Usuario* usuario){
    printf("Nombre de Usuario: %s\n", usuario->nombre);
    printf("Contraseña: %s\n", usuario->password);
}

void verTodosLosUsuarios(){
    for(int i =0; i<local->cantUsuarios; i++){
        printf("Usuario %d\n", i+1);
        toStringUsuario(&local->usuarios[i]);
    }
}


bool comprobarUsuario(char* usuario, char* password){
    for(int i =0; i<local->cantUsuarios; i++){
        
        if(strcmp(local->usuarios[i].nombre, usuario) == 0){
            if(strcmp(local->usuarios[i].password, password) == 0) return true;
        }
    }
    return false;
}



void totalPedidos(){
    char* meses[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    int primero = obtenerPrimerAnio();
    int ultimo = obtenerUltimoAnio();
    for(int i = primero; i<=ultimo; i++){
        printf("Año : %d\n", i);
        printf("Desglose por mes: \n");
        for(int j =1; j<=12; j++){
            printf("%s : %f \n", meses[j-1], obtenerTotalMesAnio(j, i));
        }
        printf("Total Año: %f", obtenerTotalAnio(i));
    }
    prinf("Total General: %f", obtenerTotal());
}