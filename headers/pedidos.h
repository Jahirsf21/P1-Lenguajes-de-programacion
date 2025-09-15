#ifndef PEDIDOS_H
#define PEDIDOS_H

#include "clientes.h"
#include "libros.h"
#include <stdbool.h>

typedef struct detallePedido { 
    Libro* libro;
    int cantidad;
} DetallePedido;

typedef struct pedido {
    char* codigo;
    char* fecha;
    Cliente* cliente;
    DetallePedido* detalles;
    int cantidadDetalles;
    float subtotal;
    float impuesto;
    float total;
} Pedido;

extern Pedido* pedidos;
extern int cantidadPedidos;

extern Libro* librosTemporales;
extern bool stockTemporal;


// Menus
void menuCrearPedido();
void menuModificarPedido();
void menuEliminarPedido();

// Funciones para la creacion, modificacion, eliminacion y actualizacion de datos;
void generarPedido(DetallePedido* carrito, int cantidadEnCarrito, float subtotal, Cliente* cliente, char* fecha);
void modificarPedido(Pedido* pedidoAModificar);
void liberarTodosLosPedidos();
void eliminarPedido(char* codigo);
void actualizarMontoPedido(Pedido* pedido);

//Funciones para mostrar la informacion del pedido
char* pedidoToString(Pedido* pedido);
void mostrarTodosLosPedidos();
void mostrarPedidoPorFecha(char* fecha);

// Funciones de guardado y cargado
void guardarPedidos();
void cargarPedidos();

//Validaciones
bool validarFecha(char* fecha);

//Auxiliares para el pedido
// Agregar estas declaraciones

void crearCopiaTemporalLibros();
void liberarCopiaTemporalLibros();
Libro* buscarLibroPorCodigoTemporal(char* codigo);
void actualizarStockTemporal(char* codigo, int cambio);

//Funciones para obtener información para las estadisticas 
int obtenerUltimoAnio();
int obtenerPrimerAnio();
float obtenerTotalMesAnio(int mes, int anio);
float obtenerTotalAnio(int anio);
float obtenerTotalGeneral();
int obtenerCantPedidosCliente(Cliente *cliente);
int cantVentasLibro(Libro* libro);
int cantVentasLibroAnio(Libro* libro, int anio);
char** todosLosAutores(int anio, int * cantidadAutores);
int ventasAutorAnio(char* autor, int anio);
#endif