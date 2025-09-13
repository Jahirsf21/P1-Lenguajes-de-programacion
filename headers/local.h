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
    Usuario* usuarios;
    int cantUsuarios;
} Local;

extern Local *local;

void cargarLocal();
bool comprobarUsuario(char* usuario, char* password);
void verTodosLosUsuarios();
void toStringUsuario(Usuario *usuario);
void toStringLocal();
void totalPedidos();
int ClientesConMasPedidos();
int LibrosMasVendidos();


#endif