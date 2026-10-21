#ifndef FUNCIONES_AYBAR_H_INCLUDED
#define FUNCIONES_AYBAR_H_INCLUDED

#include "compartido.h"

imagen_bmp_t* aplicar_rotar(const imagen_bmp_t* orig, int sentido_horario);
imagen_bmp_t* aplicar_recortar(const imagen_bmp_t* orig, int porcentaje);
imagen_bmp_t* aplicar_achicar(const imagen_bmp_t* orig, int porcentaje);

// Composición
imagen_bmp_t* aplicar_concatenar(const imagen_bmp_t* img1, const imagen_bmp_t* img2, int vertical, pixel_t color_relleno);

// Filtro Comodín (Sepia)
imagen_bmp_t* aplicar_comodin(const imagen_bmp_t* orig);

#endif // FUNCIONES_AYBAR_H_INCLUDED
