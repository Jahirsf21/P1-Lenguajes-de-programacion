#ifndef PEDIDOS_H
#define PEDIDOS_H

#include "clientes.h"
#include "libros.h"
#include <stdbool.h>

typedef struct pedido {
	char* codigo;
	Cliente* cliente;
	Libro* libros; 
	int cantidadLibros;
} Pedido;


extern Pedido* pedidos;
extern int cantidadPedidos;


void menuCrearPedido();
void menuEditarPedido();


void generarPedido(char* cedulaCliente, char* fecha);
void modificarPedido(char* codigoPedido);
void eliminarPedido(char* codigoPedido);


void filtrarFecha(char* fecha);
void filtrarAutor(char* nombreAutor);
void mostrarPedido(char* codigoPedido);



#endif