#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_utils.h"
#include "ProcesarArgumentos.h"

static void extraer_nombre_base(const char* ruta, char* buffer) {
    const char* barra = strrchr(ruta, '\\');
    if (!barra) {
        barra = strrchr(ruta, '/');
    }
    const char* nombre = barra ? barra + 1 : ruta;
    strcpy(buffer, nombre);

    char* punto = strrchr(buffer, '.');
    if (punto) *punto = '\0';
}

int solucion(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s [opciones/filtros...] <imagen.bmp> [opciones/filtros...]\n", argv[0]);
        return 1;
    }

    // 1. Buscar la ruta de la imagen principal entre los argumentos
    int idx_imagen = -1;
    for (int i = 1; i < argc; i++) {
        // La imagen no debe empezar por "--"
        if (strncmp(argv[i], "--", 2) != 0) {
            idx_imagen = i;
            break;
        }
    }

    if (idx_imagen == -1) {
        printf("Error: No se especifico ninguna imagen de entrada (.bmp)\n");
        return 1;
    }

    const char* ruta_img1 = argv[idx_imagen];
    imagen_bmp_t* img1 = cargar_bmp(ruta_img1);
    if (!img1) {
        printf("Error: No se pudo cargar la imagen principal '%s'\n", ruta_img1);
        return 1;
    }

    char nombre_base[256];
    extraer_nombre_base(ruta_img1, nombre_base);

    // 2. Procesar todos los argumentos omitiendo la posición de la imagen principal
    procesar_comandos_flexibles(argc, argv, idx_imagen, img1, nombre_base);

    liberar_imagen(img1);
    return 0;
}
