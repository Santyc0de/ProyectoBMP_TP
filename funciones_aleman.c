#include <stdlib.h>
#include "funciones_aleman.h"


imagen_bmp_t* aplicar_recortar(const imagen_bmp_t* orig, int porcentaje) {
    int orig_alto = abs(orig->info.alto);
    int orig_ancho = orig->info.ancho;

    int nuevo_ancho = (orig_ancho * porcentaje) / 100;
    int nuevo_alto = (orig_alto * porcentaje) / 100;
    if (nuevo_ancho < 1) nuevo_ancho = 1;
    if (nuevo_alto < 1) nuevo_alto = 1;

    imagen_bmp_t* res = clonar_estructura(orig, nuevo_alto, nuevo_ancho);
    if (!res) return NULL;

    int offset_x = (orig_ancho - nuevo_ancho) / 2;
    int offset_y = (orig_alto - nuevo_alto) / 2;

    for (int i = 0; i < nuevo_alto; i++) {
        for (int j = 0; j < nuevo_ancho; j++) {
            res->pixeles[i][j] = orig->pixeles[i + offset_y][j + offset_x];
        }
    }
    return res;
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

imagen_bmp_t* aplicar_comodin(const imagen_bmp_t* orig) {
    int alto = abs(orig->info.alto);
    int ancho = orig->info.ancho;

    imagen_bmp_t* res = clonar_estructura(orig, alto, ancho);
    if (!res) return NULL;

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            int r = orig->pixeles[i][j].rojo;
            int g = orig->pixeles[i][j].verde;
            int b = orig->pixeles[i][j].azul;

            res->pixeles[i][j].rojo = clampa((int)(r * 0.393f + g * 0.769f + b * 0.189f));
            res->pixeles[i][j].verde = clampa((int)(r * 0.349f + g * 0.686f + b * 0.168f));
            res->pixeles[i][j].azul = clampa((int)(r * 0.272f + g * 0.534f + b * 0.131f));
        }
    }
    return res;
}
