#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Funciones_Tevez.h"

unsigned char clampa(int valor) {
    if (valor < 0) return 0;
    if (valor > 255) return 255;
    return (unsigned char)valor;
}


imagen_bmp_t* aplicar_filtro_pixel_a_pixel(const imagen_bmp_t* orig, t_transformador_pixel transformador) {
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

void pixel_negativo(const pixel_t* src, pixel_t* dest) {
    dest->azul = 255 - src->azul;
    dest->verde = 255 - src->verde;
    dest->rojo = 255 - src->rojo;
}

void pixel_escala_grises(const pixel_t* src, pixel_t* dest) {
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

imagen_bmp_t* aplicar_transformacion_geometrica(const imagen_bmp_t* orig, int nuevo_alto, int nuevo_ancho, t_mapeador_coordenadas mapear) {
    imagen_bmp_t* res = clonar_estructura(orig, nuevo_alto, nuevo_ancho);
    if (!res) return NULL;

    int orig_alto = abs(orig->info.alto);
    int orig_ancho = orig->info.ancho;

    for (int i = 0; i < nuevo_alto; i++) {
        for (int j = 0; j < nuevo_ancho; j++) {
            int src_i, src_j;
            mapear(i, j, orig_alto, orig_ancho, &src_i, &src_j);
            res->pixeles[i][j] = orig->pixeles[src_i][src_j];
        }
    }
    return res;
}

void map_espejar_h(int i, int j, int alto, int ancho, int* src_i, int* src_j) {
    *src_i = i;
    *src_j = ancho - 1 - j;
}

void map_espejar_v(int i, int j, int alto, int ancho, int* src_i, int* src_j) {
    *src_i = alto - 1 - i;
    *src_j = j;
}

imagen_bmp_t* aplicar_espejar_horizontal(const imagen_bmp_t* orig) {
    return aplicar_transformacion_geometrica(orig, abs(orig->info.alto), orig->info.ancho, map_espejar_h);
}

imagen_bmp_t* aplicar_espejar_vertical(const imagen_bmp_t* orig) {
    return aplicar_transformacion_geometrica(orig, abs(orig->info.alto), orig->info.ancho, map_espejar_v);
}
