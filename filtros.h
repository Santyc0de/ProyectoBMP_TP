#ifndef FILTROS_H_INCLUDED
#define FILTROS_H_INCLUDED

#include "estructuras.h"

imagen_bmp_t* aplicar_negativo(const imagen_bmp_t* orig);
imagen_bmp_t* aplicar_escala_grises(const imagen_bmp_t* orig);

// Tonalidad y Contraste
imagen_bmp_t* aplicar_tonalidad(const imagen_bmp_t* orig, char canal, int porcentaje);
imagen_bmp_t* aplicar_contraste(const imagen_bmp_t* orig, int porcentaje, int aumentar);



#endif // FILTROS_H_INCLUDED
