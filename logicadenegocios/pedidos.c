#include "../headers/pedidos.h"
#include "../headers/clientes.h"
#include "../headers/libros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define CLEAR system("clear")           
#define IMPUESTO_VENTA 0.13        

Pedido* pedidos = NULL;
int cantidadPedidos = 0;

Libro* librosTemporales = NULL;
bool stockTemporal = false;

void liberarCopiaTemporalLibros() {
    if (librosTemporales != NULL) {
        for (int i = 0; i < stockLibros; i++) {
            free(librosTemporales[i].codigo);
            free(librosTemporales[i].titulo);
            free(librosTemporales[i].autor);
        }
        free(librosTemporales);
        librosTemporales = NULL;
    }
    stockTemporal = false;
}

void crearCopiaTemporalLibros() {
    if (librosTemporales != NULL) {
        free(librosTemporales);
    }
    librosTemporales = malloc(stockLibros * sizeof(Libro));
    for (int i = 0; i < stockLibros; i++) {
        librosTemporales[i] = libros[i];
        librosTemporales[i].codigo = malloc(strlen(libros[i].codigo) + 1);
        strcpy(librosTemporales[i].codigo, libros[i].codigo);
        librosTemporales[i].titulo = malloc(strlen(libros[i].titulo) + 1);
        strcpy(librosTemporales[i].titulo, libros[i].titulo);
        librosTemporales[i].autor = malloc(strlen(libros[i].autor) + 1);
        strcpy(librosTemporales[i].autor, libros[i].autor);
    }
    stockTemporal = true;
}


Libro* buscarLibroPorCodigoTemporal(char* codigo) {
    Libro* arrayAUsar = stockTemporal ? librosTemporales : libros;
    for (int i = 0; i < stockLibros; i++) {
        if (strcmp(arrayAUsar[i].codigo, codigo) == 0) {
            return &arrayAUsar[i];
        }
    }
    return NULL;
}


void actualizarStockTemporal(char* codigo, int cambio) {
    if (!stockTemporal) return;
    for (int i = 0; i < stockLibros; i++) {
        if (strcmp(librosTemporales[i].codigo, codigo) == 0) {
            librosTemporales[i].stock += cambio;
            break;
        }
    }
}


char* pedidoToString(Pedido* pedido) {
    char buffer[2048];
    char* cursor = buffer;
    cursor += sprintf(cursor, "Cod: %s | Fecha: %s | Cliente: %s (%s)\n",
                      pedido->codigo, pedido->fecha, pedido->cliente->nombre, pedido->cliente->cedula);
    cursor += sprintf(cursor, "  Detalles:\n");
    for (int i = 0; i < pedido->cantidadDetalles; i++) {
        cursor += sprintf(cursor, "    - (%d x) %s (Cod: %s)\n",
                          pedido->detalles[i].cantidad,
                          pedido->detalles[i].libro->titulo,
                          pedido->detalles[i].libro->codigo);
    }
    sprintf(cursor, "  Total: %.2f (Subtotal: %.2f, Impuesto: %.2f)",
            pedido->total, pedido->subtotal, pedido->impuesto);
    char* resultado = malloc(strlen(buffer) + 1);
    strcpy(resultado, buffer);
    return resultado;
}


void mostrarTodosLosPedidos() {
    CLEAR;
    printf("=== LISTA COMPLETA DE PEDIDOS ===\n");
    if (cantidadPedidos == 0) {
        printf("No hay pedidos registrados.\n");
    } else {
        for (int i = 0; i < cantidadPedidos; i++) {
            char* mostrarPedido = pedidoToString(&pedidos[i]);
            printf("------------------------------------\n");
            printf("%s\n", mostrarPedido);
            free(mostrarPedido);
        }
        printf("------------------------------------\n");
    }
    printf("Total de pedidos: %d\n", cantidadPedidos);
}


void mostrarPedidoPorFecha(char* fecha) {
    CLEAR;
    printf("=== PEDIDOS PARA LA FECHA: %s ===\n", fecha);
    int encontrados = 0;
    for (int i = 0; i < cantidadPedidos; i++) {
        if (strcmp(pedidos[i].fecha, fecha) == 0) {
            char* mostrarPedido = pedidoToString(&pedidos[i]);
            printf("------------------------------------\n");
            printf("%s\n", mostrarPedido);
            free(mostrarPedido);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("No se encontraron pedidos para esa fecha.\n");
    }
}


bool validarFecha(char* fecha) {
    if (strlen(fecha) != 10) {
        return false;
    }
    if (fecha[2] != '/' || fecha[5] != '/') {
        return false;
    }
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            continue;
        }
        if (!isdigit(fecha[i])) return false;
    }
    return true;
}

Libro* buscarLibroPorCodigo(char* codigo) {
    for (int i = 0; i < stockLibros; i++) {
        if (strcmp(libros[i].codigo, codigo) == 0) {
            return &libros[i];
        }
    }
    return NULL;
}

Cliente* buscarClientePorCedula(char* cedula) {
    for (int i = 0; i < cantidadClientes; i++) {
        if (strcmp(clientes[i].cedula, cedula) == 0) {
            return &clientes[i];
        }
    }
    return NULL;
}

void guardarPedidos() {
    FILE* archivo = fopen("store/pedidos.json", "w");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo para guardar los pedidos.\n");
        return;
    }

    fprintf(archivo, "[\n");
    for (int i = 0; i < cantidadPedidos; i++) {
        fprintf(archivo, "    {\n");
        fprintf(archivo, "        \"codigo\": \"%s\",\n", pedidos[i].codigo);
        fprintf(archivo, "        \"fecha\": \"%s\",\n", pedidos[i].fecha);
        fprintf(archivo, "        \"nombre del cliente\": \"%s\",\n", pedidos[i].cliente->nombre);
        fprintf(archivo, "        \"telefono del cliente\": \"%s\",\n", pedidos[i].cliente->telefono);
        fprintf(archivo, "        \"cedula del cliente\": \"%s\",\n", pedidos[i].cliente->cedula);
        fprintf(archivo, "        \"subtotal\": %.2f,\n", pedidos[i].subtotal);
        fprintf(archivo, "        \"impuesto\": %.2f,\n", pedidos[i].impuesto);
        fprintf(archivo, "        \"total\": %.2f,\n", pedidos[i].total);
        
        fprintf(archivo, "        \"detalles\": [\n");
        for (int j = 0; j < pedidos[i].cantidadDetalles; j++) {
            fprintf(archivo, "            {\n");
            fprintf(archivo, "                \"titulo del libro\": \"%s\",\n", pedidos[i].detalles[j].libro->titulo);
            fprintf(archivo, "                \"codigo del libro\": \"%s\",\n", pedidos[i].detalles[j].libro->codigo);
            fprintf(archivo, "                \"cantidad\": %d\n", pedidos[i].detalles[j].cantidad);
            fprintf(archivo, "            }");
            if (j < pedidos[i].cantidadDetalles - 1) {
                fprintf(archivo, ",\n");
            } else {
                fprintf(archivo, "\n");
            }
        }
        fprintf(archivo, "        ]\n");

        fprintf(archivo, "    }");
        if (i < cantidadPedidos - 1) {
            fprintf(archivo, ",\n");
        } else {
            fprintf(archivo, "\n");
        }
    }
    fprintf(archivo, "]\n");
    fclose(archivo);
}

void cargarPedidos() {
    FILE* archivo = fopen("store/pedidos.json", "r");
    if (archivo == NULL) {
        cantidadPedidos = 0;
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        if (strstr(buffer, "{") && !strstr(buffer, "            {")) {
            char codigo[100] = "";
            char fecha[20] ="";
            char cedulaCliente[100] = "";
            float subtotal = 0, impuesto = 0, total = 0;
            DetallePedido* detalles = NULL;
            int cantidadDetalles = 0;

            // Codigo del pedido
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"codigo\":")) {
                char* inicio = strchr(buffer, ':') + 3;
                char* fin = strrchr(buffer, '\"');
                if (inicio && fin && fin > inicio) {
                    strncpy(codigo, inicio, fin - inicio);
                    codigo[fin - inicio] = '\0';
                }
            }

            // Fecha del pedido
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"fecha\":")) { 
                char* inicio = strchr(buffer, ':') + 3;
                char* fin = strrchr(buffer, '\"');
                if (inicio && fin && fin > inicio) {
                    strncpy(fecha, inicio, fin - inicio);
                    fecha[fin - inicio] = '\0';
                }
            }
            
            // Nombre y telefono
            fgets(buffer, sizeof(buffer), archivo); 
            fgets(buffer, sizeof(buffer), archivo); 
            
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"cedula del cliente\":")) {
                char* inicio = strchr(buffer, ':') + 3;
                char* fin = strrchr(buffer, '\"');
                if (inicio && fin && fin > inicio) {
                    strncpy(cedulaCliente, inicio, fin - inicio);
                    cedulaCliente[fin - inicio] = '\0';
                }
            }
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"subtotal\":")) { subtotal = atof(strchr(buffer, ':') + 1); }
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"impuesto\":")) { impuesto = atof(strchr(buffer, ':') + 1); }
            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"total\":")) { total = atof(strchr(buffer, ':') + 1); }

            if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"detalles\":")) {
                while (fgets(buffer, sizeof(buffer), archivo) && !strstr(buffer, "]")) {
                    if (strstr(buffer, "{")) {
                        char codigoLibro[100] = "";
                        int cantidad = 0;
                        fgets(buffer, sizeof(buffer), archivo); 
                        if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"codigo del libro\":")) {
                            char* inicio = strchr(buffer, ':') + 3;
                            char* fin = strrchr(buffer, '\"');
                            if (inicio && fin && fin > inicio) {
                                strncpy(codigoLibro, inicio, fin - inicio);
                                codigoLibro[fin - inicio] = '\0';
                            }
                        }
                        if (fgets(buffer, sizeof(buffer), archivo) && strstr(buffer, "\"cantidad\":")) {
                            cantidad = atoi(strchr(buffer, ':') + 1);
                        }

                        Libro* libroEncontrado = buscarLibroPorCodigo(codigoLibro);
                        if (libroEncontrado != NULL) {
                            cantidadDetalles++;
                            detalles = realloc(detalles, cantidadDetalles * sizeof(DetallePedido));
                            detalles[cantidadDetalles - 1].libro = libroEncontrado;
                            detalles[cantidadDetalles - 1].cantidad = cantidad;
                        }
                        fgets(buffer, sizeof(buffer), archivo);
                    }
                }
            }

            Cliente* clienteEncontrado = buscarClientePorCedula(cedulaCliente);
            if (clienteEncontrado != NULL && cantidadDetalles > 0) {
                cantidadPedidos++;
                pedidos = realloc(pedidos, cantidadPedidos * sizeof(Pedido));
                Pedido* nuevoPedido = &pedidos[cantidadPedidos - 1];
                
                nuevoPedido->codigo = malloc(strlen(codigo) + 1);
                strcpy(nuevoPedido->codigo, codigo);

                nuevoPedido->fecha = malloc(strlen(fecha) + 1); 
                strcpy(nuevoPedido->fecha, fecha); 

                nuevoPedido->cliente = clienteEncontrado;
                nuevoPedido->detalles = detalles;
                nuevoPedido->cantidadDetalles = cantidadDetalles;
                nuevoPedido->subtotal = subtotal;
                nuevoPedido->impuesto = impuesto;
                nuevoPedido->total = total;
            }
        }
    }
    fclose(archivo);
}


void liberarTodosLosPedidos() {
    for (int i = 0; i < cantidadPedidos; i++) {
        free(pedidos[i].codigo);
        free(pedidos[i].fecha);
        free(pedidos[i].detalles); 
    }
    if (pedidos != NULL) {
        free(pedidos);
        pedidos = NULL; 
    }
    cantidadPedidos = 0; 
    //printf("\033[0;32mTodos los pedidos han sido liberados de la memoria.\033[0m\n");
}


void eliminarPedido(char* codigo) {
    int indice = -1;
    for (int i = 0; i < cantidadPedidos; i++) {
        if (strcmp(pedidos[i].codigo, codigo) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\033[0;31mNo se encontró un pedido con el código '%s'.\033[0m\n", codigo);
        return;
    }
    for (int i = 0; i < pedidos[indice].cantidadDetalles; i++) {
        agregarStockLibro(pedidos[indice].detalles[i].libro->codigo, pedidos[indice].detalles[i].cantidad);
    }
    guardarLibros(); 
    free(pedidos[indice].codigo);
    free(pedidos[indice].fecha);
    free(pedidos[indice].detalles);
    for (int i = indice; i < cantidadPedidos - 1; i++) {
        pedidos[i] = pedidos[i + 1];
    }
    cantidadPedidos--;
    if (cantidadPedidos > 0) {
        pedidos = realloc(pedidos, cantidadPedidos * sizeof(Pedido));
    } else {
        free(pedidos);
        pedidos = NULL;
    }
    guardarPedidos();
    printf("\033[0;32mPedido '%s' eliminado exitosamente. El stock ha sido restaurado.\033[0m\n", codigo);
}

void generarPedido(DetallePedido* carrito, int cantidadEnCarrito, float subtotal, Cliente* cliente, char* fecha) {
    for (int i = 0; i < cantidadEnCarrito; i++) {
        eliminarStockLibro(carrito[i].libro->codigo, carrito[i].cantidad);
    }
    guardarLibros();

    cantidadPedidos++;
    pedidos = realloc(pedidos, cantidadPedidos * sizeof(Pedido));
    Pedido* nuevoPedido = &pedidos[cantidadPedidos - 1];
    
    nuevoPedido->codigo = malloc(20);
    sprintf(nuevoPedido->codigo, "PED-%03d", cantidadPedidos);

    nuevoPedido->fecha = malloc(strlen(fecha) + 1);
    strcpy(nuevoPedido->fecha, fecha);

    nuevoPedido->cliente = cliente;
    nuevoPedido->detalles = carrito;
    nuevoPedido->cantidadDetalles = cantidadEnCarrito;
    nuevoPedido->subtotal = subtotal;
    nuevoPedido->impuesto = subtotal * IMPUESTO_VENTA;
    nuevoPedido->total = subtotal + nuevoPedido->impuesto;
    
    guardarPedidos(); 
    CLEAR;
    printf("=== PEDIDO GENERADO EXITOSAMENTE ===\n");
}


void actualizarMontoPedido(Pedido* pedido) {
    float subtotal = 0;
    for (int i = 0; i < pedido->cantidadDetalles; i++) {
        subtotal += pedido->detalles[i].libro->precio * pedido->detalles[i].cantidad;
    }
    pedido->subtotal = subtotal;
    pedido->impuesto = subtotal * IMPUESTO_VENTA;
    pedido->total = subtotal + pedido->impuesto;
}

void menuEliminarPedido() {
    if (cantidadPedidos == 0) {
        printf("\033[0;31mNo hay pedidos registrados para eliminar.\033[0m\n");
        printf("Presione enter para regresar...");
        getchar();
        return;
    }
    
    mostrarTodosLosPedidos();
    
    char codigoPedido[100];
    printf("\nIngrese el código del pedido que desea eliminar (o c para cancelar): ");
    fgets(codigoPedido, sizeof(codigoPedido), stdin);
    codigoPedido[strcspn(codigoPedido, "\n")] = 0;

    if (strcmp(codigoPedido, "c") == 0) {
        printf("Eliminación cancelada.\n");
        return;
    }

    eliminarPedido(codigoPedido);
}

void menuCrearPedido() {
    if (stockLibros == 0) {
        printf("\033[0;31mNo hay libros en el catálogo. No se puede crear el pedido.\033[0m\n");
        printf("Presione enter para regresar...");
        getchar();
        return; 
    }
    if (cantidadClientes == 0) {
        printf("\033[0;31mNo hay clientes. No se puede crear el pedido.\033[0m\n");
        printf("Presione enter para regresar...");
        getchar();
        return; 
    }
    crearCopiaTemporalLibros();

    DetallePedido* carrito = NULL;
    int cantidadEnCarrito = 0;
    char opcion[10];
    char inputBuffer[100]; 

    while (true) {
        CLEAR;
        printf("=== CREANDO NUEVO PEDIDO ===\n");
        float subtotalActual = 0;
        if (cantidadEnCarrito == 0) {
            printf("El carrito está vacío.\n");
        } else {
            printf("--- Carrito de Compras ---\n");
            for (int i = 0; i < cantidadEnCarrito; i++) {
                printf("  %d. COD: %s, Titulo: %s, Cant: %d, Precio U: %.2f, Subtotal: %.2f\n",
                       i + 1,
                       carrito[i].libro->codigo,
                       carrito[i].libro->titulo,
                       carrito[i].cantidad,
                       carrito[i].libro->precio,
                       carrito[i].libro->precio * carrito[i].cantidad);
                subtotalActual += carrito[i].libro->precio * carrito[i].cantidad;
            }
            float impuestoActual = subtotalActual * IMPUESTO_VENTA;
            float totalActual = subtotalActual + impuestoActual;
            
            printf("---------------------------\n");
            printf("Subtotal:          %.2f\n", subtotalActual);
            printf("Impuesto (%.0f%%):    %.2f\n", IMPUESTO_VENTA * 100, impuestoActual);
            printf("Total a pagar:     %.2f\n", totalActual);
        }
        printf("\n");
        printf("Seleccione una opción:\n");
        printf("1. Agregar libro al pedido\n");
        printf("2. Eliminar/reducir libro del pedido\n");
        printf("3. Generar pedido\n");
        printf("4. Cancelar y salir\n");
        printf("Opción: ");
        fgets(opcion, sizeof(opcion), stdin);
        opcion[strcspn(opcion, "\n")] = 0;
        if (strlen(opcion) != 1) {
            continue;
        }
        switch (opcion[0]) {
            case '1': {
                CLEAR;
                printf("=== CATÁLOGO DE LIBROS ===\n");
                if (stockLibros == 0) {
                    printf("No hay libros registrados.\n");
                } else {
                    for (int i = 0; i < stockLibros; i++) {
                        Libro* libroMostrar = &librosTemporales[i];
                        printf("------------------------------------\n");
                        printf("Código: %s\n", libroMostrar->codigo);
                        printf("Título: %s\n", libroMostrar->titulo);
                        printf("Autor: %s\n", libroMostrar->autor);
                        printf("Precio: %.2f\n", libroMostrar->precio);
                        printf("Stock: %d\n", libroMostrar->stock);
                    }
                    printf("------------------------------------\n");
                }
                printf("Total de libros: %d\n", stockLibros);
                
                char filtroOpcion[10];
                do {
                    printf("\n¿Desea filtrar el catálogo por autor? (s/n): ");
                    fgets(filtroOpcion, sizeof(filtroOpcion), stdin);
                    filtroOpcion[strcspn(filtroOpcion, "\n")] = 0;
                    
                    if (strlen(filtroOpcion) == 1 && (tolower(filtroOpcion[0]) == 's' || tolower(filtroOpcion[0]) == 'n')) {
                        break;
                    }
                    printf("\033[0;31mError: Ingrese 's' para sí o 'n' para no.\033[0m\n");
                } while (true);
                
                if (tolower(filtroOpcion[0]) == 's') {
                    printf("Ingrese el nombre del autor a buscar: ");
                    char nombreAutor[100];
                    fgets(nombreAutor, sizeof(nombreAutor), stdin);
                    nombreAutor[strcspn(nombreAutor, "\n")] = 0;
                    
                    mostrarLibrosPorAutor(nombreAutor);
                } else {
                    
                }

                Libro* libroEncontrado = NULL;
                char codigoLibro[10];
                
                do {
                    printf("Ingrese el código del libro a agregar (o 'c' para cancelar): ");
                    fgets(codigoLibro, sizeof(codigoLibro), stdin);
                    codigoLibro[strcspn(codigoLibro, "\n")] = 0;
                    if (strcmp(codigoLibro, "c") == 0) break;
                    libroEncontrado = buscarLibroPorCodigoTemporal(codigoLibro);
                    if (libroEncontrado == NULL) {
                        printf("\033[0;31mError: Libro con código '%s' no existe. Intente de nuevo.\033[0m\n", codigoLibro);
                    }
                } while (libroEncontrado == NULL);
                
                if (libroEncontrado == NULL) break;

                int cantidadDeseada = 0;
                do {
                    printf("Ingrese la cantidad (Stock disponible: %d): ", libroEncontrado->stock);
                    if (scanf("%d", &cantidadDeseada) != 1) {
                        printf("\033[0;31mError: Ingrese un número válido.\033[0m\n");
                        cantidadDeseada = -1;
                    }
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);

                    if (cantidadDeseada <= 0 || cantidadDeseada > libroEncontrado->stock) {
                        printf("\033[0;31mError: Cantidad inválida o no hay suficiente stock. Intente de nuevo.\033[0m\n");
                    }
                    if (cantidadDeseada > 0 && cantidadDeseada <= libroEncontrado->stock) {
                        printf("Se agrego el libro al carrito\n");
                        printf("Presione enter para continuar...");
                        getchar();
                    }
                } while (cantidadDeseada <= 0 || cantidadDeseada > libroEncontrado->stock);
                
                int indiceExistente = -1;
                for (int i = 0; i < cantidadEnCarrito; i++) {
                    if (strcmp(carrito[i].libro->codigo, codigoLibro) == 0) {
                        indiceExistente = i;
                        break;
                    }
                }
                if (indiceExistente != -1) {
                    if (cantidadDeseada > libroEncontrado->stock) {
                        printf("\033[0;31mError: La cantidad solicitada (%d) excede el stock disponible (%d).\033[0m\n", 
                            cantidadDeseada, libroEncontrado->stock);
                    } else {
                        carrito[indiceExistente].cantidad += cantidadDeseada;
                        actualizarStockTemporal(codigoLibro, -cantidadDeseada);
                        printf("\033[0;32mCantidad actualizada en el carrito. Nueva cantidad: %d\033[0m\n", 
                            carrito[indiceExistente].cantidad);
                    }
                } else {
                    cantidadEnCarrito++;
                    carrito = realloc(carrito, cantidadEnCarrito * sizeof(DetallePedido));
                    carrito[cantidadEnCarrito - 1].libro = buscarLibroPorCodigo(codigoLibro);
                    carrito[cantidadEnCarrito - 1].cantidad = cantidadDeseada;
                    actualizarStockTemporal(codigoLibro, -cantidadDeseada);
                    printf("\033[0;32mLibro agregado al carrito.\033[0m\n");
                }
                break;
            }
            case '2': {
                CLEAR;
                if (cantidadEnCarrito == 0) {
                    printf("\033[0;31mEl carrito está vacío, no hay nada que eliminar.\033[0m\n");
                    printf("\nPresione enter para regresar...");
                    getchar();
                    break;
                }
                int indiceEnCarrito = -1;
                char codigoLibro[10];
                do {
                    printf("--- Carrito de Compras ---\n");
                    for (int i = 0; i < cantidadEnCarrito; i++) {
                       printf("  COD: %s, Titulo: %s, Cant: %d\n", carrito[i].libro->codigo, carrito[i].libro->titulo, carrito[i].cantidad);
                    }
                    printf("---------------------------\n");
                    printf("Ingrese el código del libro a eliminar/modificar (o 'c' para cancelar): ");
                    fgets(codigoLibro, sizeof(codigoLibro), stdin);
                    codigoLibro[strcspn(codigoLibro, "\n")] = 0;
                    if (strcmp(codigoLibro, "c") == 0 || strcmp(codigoLibro, "C") == 0) break;

                    for (int i = 0; i < cantidadEnCarrito; i++) {
                        if (strcmp(carrito[i].libro->codigo, codigoLibro) == 0) {
                            indiceEnCarrito = i;
                            break;
                        }
                    }
                    if (indiceEnCarrito == -1) {
                        printf("\033[0;31mError: El libro con código '%s' no está en el carrito. Intente de nuevo.\033[0m\n", codigoLibro);
                    }
                } while (indiceEnCarrito == -1);
                if (indiceEnCarrito == -1) break;
                printf("Libro seleccionado: %s (Cantidad actual: %d)\n", carrito[indiceEnCarrito].libro->titulo, carrito[indiceEnCarrito].cantidad);
                printf("1. Eliminar todas las unidades del libro\n");
                printf("2. Reducir la cantidad\n");
                printf("Opción (o 'c' para cancelar): ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                inputBuffer[strcspn(inputBuffer, "\n")] = 0;
                if (strcmp(inputBuffer, "c") == 0 || strcmp(inputBuffer, "C") == 0) {
                     printf("Operación cancelada.\n");
                     break;
                }
                if (strcmp(inputBuffer, "1") == 0) {
                    actualizarStockTemporal(carrito[indiceEnCarrito].libro->codigo, carrito[indiceEnCarrito].cantidad);
                    for (int i = indiceEnCarrito; i < cantidadEnCarrito - 1; i++) {
                        carrito[i] = carrito[i + 1];
                    }
                    cantidadEnCarrito--;
                    if (cantidadEnCarrito > 0) {
                        carrito = realloc(carrito, cantidadEnCarrito * sizeof(DetallePedido));
                    } else {
                        free(carrito);
                        carrito = NULL;
                    }
                    printf("\033[0;32mLibro eliminado completamente del carrito.\033[0m\n");

                } else if (strcmp(inputBuffer, "2") == 0) {
                    int cantidadAReducir = 0;
                    bool cantidadValida = false;
                    do {
                        printf("Cantidad actual: %d. ¿Cuántas unidades desea eliminar? (o 'c' para cancelar): ", carrito[indiceEnCarrito].cantidad);
                        fgets(inputBuffer, sizeof(inputBuffer), stdin);
                        inputBuffer[strcspn(inputBuffer, "\n")] = 0;
                        if (strcmp(inputBuffer, "c") == 0 || strcmp(inputBuffer, "C") == 0) break;
                        char* endptr;
                        long tempCantidad = strtol(inputBuffer, &endptr, 10);
                        if (endptr == inputBuffer || *endptr != '\0') {
                            printf("\033[0;31mError: Ingrese un número válido.\033[0m\n");
                            continue;
                        }
                        cantidadAReducir = (int)tempCantidad;
                        if (cantidadAReducir <= 0 || cantidadAReducir > carrito[indiceEnCarrito].cantidad) {
                            printf("\033[0;31mError: Cantidad a eliminar inválida. Debe ser entre 1 y %d.\033[0m\n", carrito[indiceEnCarrito].cantidad);
                        } else {
                            cantidadValida = true;
                        }
                    } while (!cantidadValida);
                    
                    if (cantidadValida) {
                        actualizarStockTemporal(carrito[indiceEnCarrito].libro->codigo, cantidadAReducir);
                        carrito[indiceEnCarrito].cantidad -= cantidadAReducir;
                        printf("\033[0;32m%d unidades de '%s' eliminadas. Quedan %d.\033[0m\n", cantidadAReducir, carrito[indiceEnCarrito].libro->titulo, carrito[indiceEnCarrito].cantidad);
                        
                        if (carrito[indiceEnCarrito].cantidad == 0) {
                            for (int i = indiceEnCarrito; i < cantidadEnCarrito - 1; i++) {
                                carrito[i] = carrito[i + 1];
                            }
                            cantidadEnCarrito--;
                            if (cantidadEnCarrito > 0) {
                                carrito = realloc(carrito, cantidadEnCarrito * sizeof(DetallePedido));
                            } else {
                                free(carrito);
                                carrito = NULL;
                            }
                        }
                    }
                } else {
                    printf("\033[0;31mOpción no válida.\033[0m\n");
                }
                break;
            }
            case '3': {
                CLEAR;
                if (cantidadEnCarrito == 0) {
                    printf("\033[0;31mNo puede generar un pedido con el carrito vacío.\033[0m\n");
                    printf("\nPresione enter para regresar...");
                    getchar();
                    break;
                }

                Cliente* clienteEncontrado = NULL;
                char cedulaCliente[100];
                do {
                    mostrarTodosLosClientes();
                    printf("Ingrese la cédula del cliente (o 'c' para cancelar): ");
                    fgets(cedulaCliente, sizeof(cedulaCliente), stdin);
                    cedulaCliente[strcspn(cedulaCliente, "\n")] = 0;

                    if (strcmp(cedulaCliente, "c") == 0) break;

                    clienteEncontrado = buscarClientePorCedula(cedulaCliente);
                    if (clienteEncontrado == NULL) {
                        printf("\033[0;31mError: Cliente con cédula '%s' no encontrado. Intente de nuevo.\033[0m\n", cedulaCliente);
                    }
                } while (clienteEncontrado == NULL);
                
                if (clienteEncontrado == NULL) {
                    break;
                }
                char fechaPedido[20];
                do {
                    printf("Ingrese la fecha del pedido (dd/mm/aaaa): ");
                    fgets(fechaPedido, sizeof(fechaPedido), stdin);
                    fechaPedido[strcspn(fechaPedido, "\n")] = 0;
                    if (!validarFecha(fechaPedido)) {
                        printf("\033[0;31mFormato de fecha inválido. Intente de nuevo.\033[0m\n");
                    }
                } while (!validarFecha(fechaPedido));
                liberarCopiaTemporalLibros();
                
                generarPedido(carrito, cantidadEnCarrito, subtotalActual, clienteEncontrado, fechaPedido);
                return;
            }
            case '4': {
                liberarCopiaTemporalLibros();
                CLEAR;
                if (carrito != NULL) {
                    free(carrito);
                }
                printf("Creación de pedido cancelada.\n");
                printf("Regresando al menu anterior.\n");
                return;
            }
            default: {
                printf("\033[0;31mOpción no válida. Intente de nuevo.\033[0m\n");
                break;
            }
        }
    }
}


void modificarPedido(Pedido* pedidoAModificar) {
    char opcion[10];
    while (true) {
        CLEAR;
        printf("=== MODIFICANDO PEDIDO: %s ===\n", pedidoAModificar->codigo);
        printf("--- Estado Actual del Pedido ---\n");
        for (int i = 0; i < pedidoAModificar->cantidadDetalles; i++) {
            printf("  %d. (x%d) %s - Subtotal: %.2f\n",
                   i + 1,
                   pedidoAModificar->detalles[i].cantidad,
                   pedidoAModificar->detalles[i].libro->titulo,
                   pedidoAModificar->detalles[i].libro->precio * pedidoAModificar->detalles[i].cantidad);
        }
        printf("--------------------------------\n");
        printf("Total actual: %.2f\n\n", pedidoAModificar->total);
        
        printf("Seleccione una opción:\n");
        printf("1. Agregar o sumar cantidad de un libro\n");
        printf("2. Eliminar o restar cantidad de un libro\n");
        printf("3. Guardar cambios y salir\n");
        printf("Opción: ");

        fgets(opcion, sizeof(opcion), stdin);
        opcion[strcspn(opcion, "\n")] = 0;

        if (strlen(opcion) != 1) continue;

        switch (opcion[0]) {
            case '1': { 
                mostrarTodosLosLibros();
                printf("Ingrese el código del libro a agregar: ");
                char codigoLibro[10];
                fgets(codigoLibro, sizeof(codigoLibro), stdin);
                codigoLibro[strcspn(codigoLibro, "\n")] = 0;

                Libro* libroOriginal = buscarLibroPorCodigo(codigoLibro);
                if (libroOriginal == NULL) {
                    printf("\033[0;31mError: Libro no encontrado.\033[0m\n");
                    continue;
                }

                printf("Ingrese la cantidad a agregar (Stock disponible: %d): ", libroOriginal->stock);
                int cantidad;
                scanf("%d", &cantidad);

                if (cantidad <= 0 || cantidad > libroOriginal->stock) {
                    printf("\033[0;31mError: Cantidad inválida o no hay suficiente stock.\033[0m\n");
                    continue;
                }
                libroOriginal->stock -= cantidad;

                int indiceExistente = -1;
                for (int i = 0; i < pedidoAModificar->cantidadDetalles; i++) {
                    if (strcmp(pedidoAModificar->detalles[i].libro->codigo, codigoLibro) == 0) {
                        indiceExistente = i;
                        break;
                    }
                }

                if (indiceExistente != -1) { 
                    pedidoAModificar->detalles[indiceExistente].cantidad += cantidad;
                } else { 
                    pedidoAModificar->cantidadDetalles++;
                    pedidoAModificar->detalles = realloc(pedidoAModificar->detalles, pedidoAModificar->cantidadDetalles * sizeof(DetallePedido));
                    pedidoAModificar->detalles[pedidoAModificar->cantidadDetalles - 1].libro = libroOriginal;
                    pedidoAModificar->detalles[pedidoAModificar->cantidadDetalles - 1].cantidad = cantidad;
                }
                
                actualizarMontoPedido(pedidoAModificar);
                printf("\033[0;32mLibro agregado/actualizado en el pedido.\033[0m\n");
                break;
            }
            case '2': { 
                 if (pedidoAModificar->cantidadDetalles == 0) {
                    printf("\033[0;31mEl pedido está vacío, no se puede eliminar nada.\033[0m\n");
                    continue;
                }
                printf("Ingrese el número de línea a eliminar/reducir (1 a %d): ", pedidoAModificar->cantidadDetalles);
                int linea;
                scanf("%d", &linea);

                if (linea < 1 || linea > pedidoAModificar->cantidadDetalles) {
                    printf("\033[0;31mNúmero de línea inválido.\033[0m\n");
                    continue;
                }
                int indice = linea - 1;
                
                printf("Ingrese la cantidad a eliminar (actualmente hay %d): ", pedidoAModificar->detalles[indice].cantidad);
                int cantidad;
                scanf("%d", &cantidad);
                if (cantidad <= 0 || cantidad > pedidoAModificar->detalles[indice].cantidad) {
                    printf("\033[0;31mCantidad a eliminar inválida.\033[0m\n");
                    continue;
                }
                agregarStockLibro(pedidoAModificar->detalles[indice].libro->codigo, cantidad);

                pedidoAModificar->detalles[indice].cantidad -= cantidad;
                if (pedidoAModificar->detalles[indice].cantidad == 0) {
                    for (int i = indice; i < pedidoAModificar->cantidadDetalles - 1; i++) {
                        pedidoAModificar->detalles[i] = pedidoAModificar->detalles[i + 1];
                    }
                    pedidoAModificar->cantidadDetalles--;
                    pedidoAModificar->detalles = realloc(pedidoAModificar->detalles, pedidoAModificar->cantidadDetalles * sizeof(DetallePedido));
                }

                actualizarMontoPedido(pedidoAModificar);
                printf("\033[0;32mCantidad reducida/eliminada del pedido.\033[0m\n");
                break;
            }
            case '3': { 
                guardarLibros();   
                guardarPedidos(); 
                printf("\033[0;32mPedido %s guardado exitosamente.\033[0m\n", pedidoAModificar->codigo);
                return;
            }
            default:
                printf("\033[0;31mOpción no válida.\033[0m\n");
                break;
        }
    }
}

void menuModificarPedido() {
    if (cantidadPedidos == 0) {
        printf("\033[0;31mNo hay pedidos registrados para modificar.\033[0m\n");
        printf("Presione enter para regresar...");
        getchar();
        return;
    }
    mostrarTodosLosPedidos();
    char codigoPedido[100];
    Pedido* pedidoAModificar = NULL;
    do {
        printf("\nIngrese el código del pedido que desea modificar (o c para cancelar): ");
        fgets(codigoPedido, sizeof(codigoPedido), stdin);
        codigoPedido[strcspn(codigoPedido, "\n")] = 0;
        if (strcmp(codigoPedido, "c") == 0 || strcmp(codigoPedido, "C") == 0) {
            printf("Modificación cancelada.\n");
            return;
        }
        for (int i = 0; i < cantidadPedidos; i++) {
            if (strcmp(pedidos[i].codigo, codigoPedido) == 0) {
                pedidoAModificar = &pedidos[i];
                break;
            }
        }
        if (pedidoAModificar == NULL) {
            printf("\033[0;31mCódigo de pedido no encontrado. Intente de nuevo.\033[0m\n");
        }
    } while (pedidoAModificar == NULL);
    modificarPedido(pedidoAModificar);
}

int obtenerUltimoAnio(){
    int res=0;
    int temp;
    for(int i =0; i<cantidadPedidos; i++){
        char anio[4];
        strncpy(anio, pedidos[i].fecha + 6, 4);
        anio[4] = '\0';
        temp = atoi(anio);  
        if (temp>res){
            res=temp;
        }
    }
    return res;
}


int obtenerPrimerAnio(){
    int res=9999;
    int temp;
    for(int i =0; i<cantidadPedidos; i++){
        char anio[4];
        strncpy(anio, pedidos[i].fecha + 6, 4);
        anio[4] = '\0';
        temp = atoi(anio);  
        if (temp<res){
            res=temp;
        }
    }
    
    return res;
}


float obtenerTotalMesAnio(int mes, int anio){
    int tempMes;
    int tempAnio;
    int res=0;
    for (int i=0; i< cantidadPedidos; i++){
        char cAnio[4];
        strncpy(cAnio, pedidos[i].fecha + 6, 4);
        cAnio[4] = '\0';
        tempAnio = atoi(cAnio);
        
        char cMes[2];
        strncpy(cMes, pedidos[i].fecha + 3, 2);
        cMes[2] ='\0';
        tempMes = atoi(cMes);
        
        if(tempMes==mes && tempAnio==anio){
            res+= pedidos[i].total;
        }
    }
    return res;
}
float obtenerTotalAnio(int anio){
    int res=0;
    for(int i=1; i<=12; i++){
        res+=obtenerTotalMesAnio(i, anio);
    }
    return res;
}

float obtenerTotalGeneral(){
    int primero = obtenerPrimerAnio();
    int ultimo = obtenerUltimoAnio();
    int res=0;
    for(int i = primero; i<=ultimo; i++){
        res+= obtenerTotalAnio(i);
    }
    return res;
}


int obtenerCantPedidosCliente(Cliente* cliente){
    
    int res=0;
    for(int i=0; i<cantidadPedidos; i++){
        if ((pedidos+i)->cliente->cedula == cliente->cedula){
            
            res++;
        }
    }
    return res;
}

int cantVentasLibro(Libro* libro){
    int res=0;
    for(int i=0; i<cantidadPedidos; i++){
        for(int j=0; j<(pedidos+i)->cantidadDetalles; i++){
            if((pedidos+i)->detalles->libro==libro) res++;
        }
    }
}

int cantVentasLibroAnio(Libro* libro, int anio){
    int res=0;
    int temp;
    for(int i=0; i<cantidadPedidos; i++){
        char anioTemp[4];
        strncpy(anioTemp, pedidos[i].fecha + 6, 4);
        anioTemp[4] = '\0';
        temp= atoi(anioTemp);  
        for(int j=0; j<(pedidos+i)->cantidadDetalles; i++){
            if((((pedidos+i)->detalles + j)->libro == libro) && temp == anio) res++;
            
        }
    }
}

bool yaEsta(char** arr, char* autor, int len){
    for(int i=0; i<len;i++){
        if(*(arr+i)==autor) return true;
    }
    return false;
}

char** todosLosAutores(int anio, int* cantidadAutores) {

    char** autores = NULL;
    int count = 0;

    for (int i = 0; i < cantidadPedidos; i++) {
        for (int j = 0; j < pedidos[i].cantidadDetalles; j++) {
            char* actual = pedidos[i].detalles[j].libro->autor;

            if (!yaEsta(autores, actual, count)) {
                autores = realloc(autores, (count + 1) * sizeof(char*));
                autores[count] = actual;
                count++;
            }
        }
    }
    *cantidadAutores = count;
    return autores;
}


int ventasAutorAnio(char* autor, int anio){
    int res=0;
    int temp=0;
    for(int i=0; i<cantidadPedidos; i++){
        char anioTemp[4];
        strncpy(anioTemp, pedidos[i].fecha + 6, 4);
        anioTemp[4] = '\0';
        temp= atoi(anioTemp); 
        for(int j=0; j<(pedidos+i)->cantidadDetalles; i++){
            char* actual = ((pedidos + i)->detalles + j)->libro->autor;
            if(strcmp(autor, actual)==0 && temp==anio) res++;
        }
    }
    
    return res;
}

