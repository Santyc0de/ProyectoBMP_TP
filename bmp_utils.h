#ifndef BMP_UTILS_H_INCLUDED
#define BMP_UTILS_H_INCLUDED

#include "estructuras.h"

pixel_t** crear_matriz(int alto, int ancho);
void liberar_matriz(pixel_t** matriz, int alto);
imagen_bmp_t* cargar_bmp(const char* ruta);
int guardar_bmp(const char* ruta, const imagen_bmp_t* img);
void liberar_imagen(imagen_bmp_t* img);


#endif // BMP_UTILS_H_INCLUDED
