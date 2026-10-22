#ifndef FUNCIONES_TEVEZ_H_INCLUDED
#define FUNCIONES_TEVEZ_H_INCLUDED

#include "compartido.h"


typedef void (*t_transformador_pixel)(const pixel_t* src, pixel_t* dest);
typedef void (*t_mapeador_coordenadas)(int i, int j, int alto, int ancho, int* src_i, int* src_j);

unsigned char clampa(int valor);
imagen_bmp_t* aplicar_filtro_pixel_a_pixel(const imagen_bmp_t* orig, t_transformador_pixel transformador);

void pixel_negativo(const pixel_t* src, pixel_t* dest);
void pixel_escala_grises(const pixel_t* src, pixel_t* dest);

imagen_bmp_t* aplicar_transformacion_geometrica(const imagen_bmp_t* orig, int nuevo_alto, int nuevo_ancho, t_mapeador_coordenadas mapear);
void map_espejar_h(int i, int j, int alto, int ancho, int* src_i, int* src_j);
void map_espejar_v(int i, int j, int alto, int ancho, int* src_i, int* src_j);

imagen_bmp_t* aplicar_negativo(const imagen_bmp_t* orig);
imagen_bmp_t* aplicar_escala_grises(const imagen_bmp_t* orig);

// Tonalidad y Contraste
imagen_bmp_t* aplicar_contraste(const imagen_bmp_t* orig, int porcentaje, int aumentar);

// Geometr�a y Dimensiones
imagen_bmp_t* aplicar_espejar_horizontal(const imagen_bmp_t* orig);
imagen_bmp_t* aplicar_espejar_vertical(const imagen_bmp_t* orig);


#endif // FUNCIONES_TEVEZ_H_INCLUDED
