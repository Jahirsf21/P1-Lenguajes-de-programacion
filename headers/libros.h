#ifndef LIBROS_H
#define LIBROS_H
#include <stdbool.h>

typedef struct libro {
    char* titulo;
    char* autor;
    char* codigo;
    float precio;
    int stock;
} Libro;

extern Libro* libros;
extern int stockLibros;

char* libroToString(Libro *libro);
void mostrarTodosLosLibros();
void mostrarLibrosPorAutor(char* autor);

//Funciones para el registro, cargado y guardado de los libros
void menuRegistrarLibro();
void menuAgregarStockLibro();


void menuEliminarLibro();
void menuCargaInventario();

void registrarLibro(char* titulo, char* autor, char* codigo, float precio, int stock);
void cargarLibros();
void guardarLibros();
void eliminarStockLibro(char *codigo, int cantidad);
void eliminarLibro(char *codigo);
void agregarStockLibro(char* codigo, int cantidad);
void cargaInventario(const char* archivoCarga);



//Funciones de busqueda
bool existeLibro(char* codigo);

//Validaciones en los tipos de datos para el libro
bool validarTitulo(char* titulo);
bool validarAutor(char* autor);
bool validarCodigo(char* codigo);
bool validarPrecio(float precio);
bool validarStock(int cantidad);
bool libroAsociadoPedido(char* codigo);

//Funciones para liberar memoria
void liberarLibro(Libro *libro);
void liberarTodosLosLibros();


#endif //LIBROS_H