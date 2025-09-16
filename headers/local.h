#ifndef LOCAL_H
#define LOCAL_H
#include "../headers/clientes.h"
#include "../headers/pedidos.h"
#include "../headers/local.h"
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

//metodos toString
void verTodosLosUsuarios();
void toStringUsuario(Usuario *usuario);
void toStringLocal();

//metodo para cargar la información del local
void cargarLocal();

//metodo para comprobar si las credenciales son correctas
bool comprobarUsuario(char* usuario, char* password);

//metodos para las estadisticas
int totalPedidos();
int ClientesConMasPedidos();
int LibrosMasVendidos();
int autorConMasVentasAnio();
int LibrosMasVendidosMenu();

#endif