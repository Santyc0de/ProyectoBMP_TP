#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "compartido.h"
#include "funciones_aybar.h"

imagen_bmp_t* aplicar_rotar(const imagen_bmp_t* orig, int sentido_horario) {
    int orig_alto = abs(orig->info.alto);
    int orig_ancho = orig->info.ancho;

    imagen_bmp_t* res = clonar_estructura(orig, orig_ancho, orig_alto);
    if (!res) return NULL;

    for (int i = 0; i < orig_alto; i++) {
        for (int j = 0; j < orig_ancho; j++) {
            if (sentido_horario) {
                res->pixeles[j][orig_alto - 1 - i] = orig->pixeles[i][j];
            } else {
                res->pixeles[orig_ancho - 1 - j][i] = orig->pixeles[i][j];
            }
        }
    }
    return res;
}

imagen_bmp_t* aplicar_achicar(const imagen_bmp_t* orig, int porcentaje) {
    int orig_alto = abs(orig->info.alto);
    int orig_ancho = orig->info.ancho;

    int nuevo_ancho = (orig_ancho * porcentaje) / 100;
    int nuevo_alto = (orig_alto * porcentaje) / 100;
    if (nuevo_ancho < 1) nuevo_ancho = 1;
    if (nuevo_alto < 1) nuevo_alto = 1;

    imagen_bmp_t* res = clonar_estructura(orig, nuevo_alto, nuevo_ancho);
    if (!res) return NULL;

    for (int i = 0; i < nuevo_alto; i++) {
        for (int j = 0; j < nuevo_ancho; j++) {
            int src_i = (i * orig_alto) / nuevo_alto;
            int src_j = (j * orig_ancho) / nuevo_ancho;
            res->pixeles[i][j] = orig->pixeles[src_i][src_j];
        }
    }
    return res;
}

imagen_bmp_t* aplicar_concatenar(const imagen_bmp_t* img1, const imagen_bmp_t* img2, int vertical, pixel_t color_relleno) {
    int alto1 = abs(img1->info.alto);
    int ancho1 = img1->info.ancho;
    int alto2 = abs(img2->info.alto);
    int ancho2 = img2->info.ancho;

    int alto_res, ancho_res;

    if (vertical) {
        ancho_res = (ancho1 > ancho2) ? ancho1 : ancho2;
        alto_res = alto1 + alto2;
    } else {
        ancho_res = ancho1 + ancho2;
        alto_res = (alto1 > alto2) ? alto1 : alto2;
    }

    imagen_bmp_t* res = clonar_estructura(img1, alto_res, ancho_res);
    if (!res) return NULL;

    for (int i = 0; i < alto_res; i++) {
        for (int j = 0; j < ancho_res; j++) {
            res->pixeles[i][j] = color_relleno;
        }
    }

    for (int i = 0; i < alto1; i++) {
        for (int j = 0; j < ancho1; j++) {
            res->pixeles[i][j] = img1->pixeles[i][j];
        }
    }

    if (vertical) {
        for (int i = 0; i < alto2; i++) {
            for (int j = 0; j < ancho2; j++) {
                res->pixeles[i + alto1][j] = img2->pixeles[i][j];
            }
        }
    } else {
        for (int i = 0; i < alto2; i++) {
            for (int j = 0; j < ancho2; j++) {
                res->pixeles[i][j + ancho1] = img2->pixeles[i][j];
            }
        }
    }

    return res;
}