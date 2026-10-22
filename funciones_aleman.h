#ifndef FUNCIONES_ALEMAN_H_INCLUDED
#define FUNCIONES_ALEMAN_H_INCLUDED

#include "compartido.h"

imagen_bmp_t* aplicar_recortar(const imagen_bmp_t* orig, int porcentaje);

imagen_bmp_t* aplicar_tonalidad(const imagen_bmp_t* orig, char canal, int porcentaje);

// Filtro Comodín (Sepia)
imagen_bmp_t* aplicar_comodin(const imagen_bmp_t* orig);


#endif // FUNCIONES_ALEMAN_H_INCLUDED
