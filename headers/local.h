#ifndef LOCAL_H
#define LOCAL_H

typedef struct usuario
{
    char* nombre;
    char* password;
} Usuario;

typedef struct local {
    char* nombre;
    char* telefono;
    char* cedulaJuridica;
    char* horarioInicio;
    char* horarioFin;
    int sigPedido;
    Usuario* usuario;
    int cantUsuarios;
} Local;

extern Local *local;

void cargarLocal();



#endif