#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "filtros.h"
#include "bmp_utils.h"

static imagen_bmp_t* clonar_estructura(const imagen_bmp_t* orig, int nuevo_alto, int nuevo_ancho) {
    imagen_bmp_t* nueva = (imagen_bmp_t*)malloc(sizeof(imagen_bmp_t));
    if (!nueva) return NULL;

    nueva->header = orig->header;
    nueva->info = orig->info;

    // Actualizamos las dimensiones y el tamaño de la cabecera e imagen
    nueva->info.ancho = nuevo_ancho;
    nueva->info.alto = nuevo_alto;

    int padding = (4 - (nuevo_ancho * 3) % 4) % 4;
    unsigned int tamFilaConPadding = (nuevo_ancho * 3) + padding;
    unsigned int tamImagen = tamFilaConPadding * nuevo_alto;

    nueva->info.tam_imagen = tamImagen;
    nueva->header.offset_datos = sizeof(cabecera_archivo_t) + sizeof(cabecera_info_t);
    nueva->header.tam_archivo = nueva->header.offset_datos + tamImagen;

    nueva->pixeles = crear_matriz(nuevo_alto, nuevo_ancho);
    if (!nueva->pixeles) {
        free(nueva);
        return NULL;
    }

    return nueva;
}

static unsigned char clampa(int valor) {
    if (valor < 0) return 0;
    if (valor > 255) return 255;
    return (unsigned char)valor;
}

typedef void (*t_transformador_pixel)(const pixel_t* src, pixel_t* dest);

static imagen_bmp_t* aplicar_filtro_pixel_a_pixel(const imagen_bmp_t* orig, t_transformador_pixel transformador) {
    int alto = abs(orig->info.alto);
    int ancho = orig->info.ancho;

    imagen_bmp_t* res = clonar_estructura(orig, alto, ancho);
    if (!res) return NULL;

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            transformador(&orig->pixeles[i][j], &res->pixeles[i][j]);
        }
    }
    return res;
}

static void pixel_negativo(const pixel_t* src, pixel_t* dest) {
    dest->azul = 255 - src->azul;
    dest->verde = 255 - src->verde;
    dest->rojo = 255 - src->rojo;
}

static void pixel_escala_grises(const pixel_t* src, pixel_t* dest) {
    int gris = (int)(0.114f * src->azul + 0.587f * src->verde + 0.299f * src->rojo);
    unsigned char g = clampa(gris);
    dest->azul = g;
    dest->verde = g;
    dest->rojo = g;
}

imagen_bmp_t* aplicar_negativo(const imagen_bmp_t* orig) {
    return aplicar_filtro_pixel_a_pixel(orig, pixel_negativo);
}

imagen_bmp_t* aplicar_escala_grises(const imagen_bmp_t* orig) {
    return aplicar_filtro_pixel_a_pixel(orig, pixel_escala_grises);
}

imagen_bmp_t* aplicar_tonalidad(const imagen_bmp_t* orig, char canal, int porcentaje) {
    int alto = abs(orig->info.alto);
    int ancho = orig->info.ancho;

    imagen_bmp_t* res = clonar_estructura(orig, alto, ancho);
    if (!res) return NULL;

    float factor = 1.0f + (porcentaje / 100.0f);

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            res->pixeles[i][j] = orig->pixeles[i][j];
            if (canal == 'b' || canal == 'B') res->pixeles[i][j].azul = clampa((int)(orig->pixeles[i][j].azul * factor));
            if (canal == 'g' || canal == 'G') res->pixeles[i][j].verde = clampa((int)(orig->pixeles[i][j].verde * factor));
            if (canal == 'r' || canal == 'R') res->pixeles[i][j].rojo = clampa((int)(orig->pixeles[i][j].rojo * factor));
        }
    }
    return res;
}

imagen_bmp_t* aplicar_contraste(const imagen_bmp_t* orig, int porcentaje, int aumentar) {
    int alto = abs(orig->info.alto);
    int ancho = orig->info.ancho;

    imagen_bmp_t* res = clonar_estructura(orig, alto, ancho);
    if (!res) return NULL;

    float factor;
    if (aumentar) {
        if (porcentaje >= 259) porcentaje = 258;
        factor = (259.0f * (porcentaje + 255.0f)) / (255.0f * (259.0f - porcentaje));
    } else {
        factor = 1.0f - (porcentaje / 100.0f);
    }

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            res->pixeles[i][j].azul = clampa((int)(factor * (orig->pixeles[i][j].azul - 128) + 128));
            res->pixeles[i][j].verde = clampa((int)(factor * (orig->pixeles[i][j].verde - 128) + 128));
            res->pixeles[i][j].rojo = clampa((int)(factor * (orig->pixeles[i][j].rojo - 128) + 128));
        }
    }
    return res;
}

