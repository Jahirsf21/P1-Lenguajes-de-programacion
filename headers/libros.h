#ifndef LIBROS_H
#define LIBROS_H

#define MAX_TITULO 100
#define MAX_AUTOR 100
#define MAX_CODIGO 20 
#define MAX_LINEA 256
#define ARCHIVO_LIBROS "store/libros.json"


//Struct para representar un libro
typedef struct libro {
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    char codigo[MAX_CODIGO];
    float precio;
    int stock;
} Libro;

//Struct para representar el inventario de libros
typedef struct inventario {
    Libro *libros;
    int stockLibros;
    int capacidad;
} InventarioLibros;


//Funciones de manejo de inventario
InventarioLibros* iniciarInventario();
void liberarInventario(InventarioLibros *inventario);

//Funciones principales de manejo de libros
int registrarLibro(InventarioLibros *inventario);
int eliminarLibro(InventarioLibros *inventario, const char *codigo);
Libro* buscarLibroPorCodigo(InventarioLibros *inventario, const char *codigo);

//Funciones de inventario
int cargarInventario(InventarioLibros *inventario, const char *nombreArchivo);
int validarStockDisponible(InventarioLibros *inventario, const char *codigo, int stockSolicitado);
int actualizarStockLibro(InventarioLibros *inventario, const char *codigo, int cambioStock);

//Funciones de persistencia
int cargarLibrosDesdeArchivo(InventarioLibros *inventario);
int guardarLibrosEnArchivo(InventarioLibros *inventario);

//Funciones auxiliares
int compararCadenas(const char *cadena1, const char *cadena2);
void copiarCadena(char *destino, const char *origen);
int largoCadena(const char *cadena);

//Funciones de utilidad
void limpiarBuffer();
int validarCodigoUnico(InventarioLibros *inventario, const char *codigo);
void mostrarLibro(const Libro *libro);
void redimensionarInventario(InventarioLibros *inventario);

#endif