#ifndef CLIENTES_H
#define CLIENTES_H
#include <stdbool.h> 

typedef struct cliente {
	char* nombre;
	char* cedula;
	char* telefono;
} Cliente;

extern Cliente* clientes;
extern int cantidadClientes;

//Funciones encargadas de mostrar la informacion del cliente y los clientes.
void mostrarTodosLosClientes();
char* clienteToString(Cliente *cliente);

//Funciones encargadas del registro de clientes y guardado, cargado de datos
void cargarClientes();
void guardarClientes();
void menuRegistrarCliente();
void menuEliminarCliente();
void registrarClientes(char* nombre, char* cedula, char* telefono);

//Validaciones para los tipos de datos que tiene el cliente
bool validarTelefono(char* telefono);
bool validarCedula(char*  cedula);
bool validarNombre(char* nombre);
bool clienteTienePedidos(char* cedula);
bool existeCliente(char* cedula);


//Funciones encargadas de la liberacion de memoria
void liberarCliente(Cliente* cliente);
void liberarTodosLosClientes();
void eliminarCliente(int indice);

void eliminarClientePorCedula(char* cedula);


#endif