#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#include <stdint.h>

#pragma pack(push,1)
typedef struct {
    char tipo[2];
    unsigned int tam_archivo;
    unsigned short reservado1;
    unsigned short reservado2;
    unsigned int offset_datos;
}cabecera_archivo_t;

typedef struct {
    unsigned int tam_cabecera;
    int ancho;
    int alto;
    unsigned short planos;
    unsigned short bpp;
    unsigned int compresion;
    unsigned int tam_imagen;
    int res_horizontal;
    int res_vertical;
    unsigned int colores_paleta;
    unsigned int colores_importantes;
}cabecera_info_t;
#pragma pack(pop)

typedef struct {
    unsigned char azul;
    unsigned char verde;
    unsigned char rojo;
} pixel_t;

typedef struct {
    cabecera_archivo_t header;
    cabecera_info_t info;
    pixel_t** pixeles;
} imagen_bmp_t;

#endif // ESTRUCTURAS_H_INCLUDED
