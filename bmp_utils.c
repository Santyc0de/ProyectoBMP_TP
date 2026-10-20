#include "bmp_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BMP_BPP_24 24

pixel_t** crear_matriz(int alto, int ancho) {
    if (alto <= 0 || ancho <= 0) return NULL;

    pixel_t** matriz = (pixel_t**)malloc(alto * sizeof(pixel_t*));
    if (!matriz) return NULL;

    for (int i = 0; i < alto; i++) {
        matriz[i] = (pixel_t*)malloc(ancho * sizeof(pixel_t));
        if (!matriz[i]) {
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }
    }
    return matriz;
}

void liberar_matriz(pixel_t** matriz, int alto) {
    if (!matriz) return;
    for (int i = 0; i < alto; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void liberar_imagen(imagen_bmp_t* img) {
    if (!img) return;
    liberar_matriz(img->pixeles, abs(img->info.alto));
    free(img);
}

//-----------------------------------------------------

imagen_bmp_t* cargar_bmp(const char* ruta) {
    FILE* f = fopen(ruta, "rb");
    if (!f) return NULL;

    imagen_bmp_t* img = (imagen_bmp_t*)malloc(sizeof(imagen_bmp_t));
    if (!img) {
        fclose(f);
        return NULL;
    }

    if (fread(&img->header, sizeof(cabecera_archivo_t), 1, f) != 1 ||
        fread(&img->info, sizeof(cabecera_info_t), 1, f) != 1) {
        free(img);
        fclose(f);
        return NULL;
    }

    // Validamos que el tipo sea exactamente "BM" usando memcmp y bpp sea 24
    if (memcmp(img->header.tipo, "BM", 2) != 0 || img->info.bpp != BMP_BPP_24) {
        free(img);
        fclose(f);
        return NULL;
    }

    int ancho = img->info.ancho;
    int alto_original = abs(img->info.alto);

    if (ancho <= 0 || alto_original <= 0) {
        free(img);
        fclose(f);
        return NULL;
    }

    img->pixeles = crear_matriz(alto_original, ancho);
    if (!img->pixeles) {
        free(img);
        fclose(f);
        return NULL;
    }

    if (fseek(f, img->header.offset_datos, SEEK_SET) != 0) {
        liberar_imagen(img);
        fclose(f);
        return NULL;
    }

    int padding = (4 - (ancho * (int)sizeof(pixel_t)) % 4) % 4;

    int inicio = (img->info.alto > 0) ? (alto_original - 1) : 0;
    int fin = (img->info.alto > 0) ? -1 : alto_original;
    int paso = (img->info.alto > 0) ? -1 : 1;

    for (int i = inicio; i != fin; i += paso) {
        if (fread(img->pixeles[i], sizeof(pixel_t), ancho, f) != (size_t)ancho) {
            liberar_imagen(img);
            fclose(f);
            return NULL;
        }
        if (padding > 0) {
            if (fseek(f, padding, SEEK_CUR) != 0) {
                liberar_imagen(img);
                fclose(f);
                return NULL;
            }
        }
    }

    fclose(f);
    return img;
}

int guardar_bmp(const char* ruta, const imagen_bmp_t* img) {
    if (!img || !img->pixeles) return 0;

    FILE* f = fopen(ruta, "wb");
    if (!f) return 0;

    int ancho = img->info.ancho;
    int alto = abs(img->info.alto);

    int padding = (4 - (ancho * (int)sizeof(pixel_t)) % 4) % 4;
    unsigned int tamFilaConPadding = (ancho * (int)sizeof(pixel_t)) + padding;
    unsigned int tamImagen = tamFilaConPadding * alto;

    cabecera_archivo_t fh = img->header;
    cabecera_info_t ih = img->info;

    // Aseguramos la firma "BM" en el arreglo de caracteres
    fh.tipo[0] = 'B';
    fh.tipo[1] = 'M';
    fh.offset_datos = sizeof(cabecera_archivo_t) + sizeof(cabecera_info_t);

    ih.tam_cabecera = sizeof(cabecera_info_t);
    ih.ancho = ancho;
    ih.alto = alto;
    ih.planos = 1;
    ih.bpp = BMP_BPP_24;
    ih.compresion = 0;
    ih.tam_imagen = tamImagen;

    fh.tam_archivo = fh.offset_datos + tamImagen;

    if (fwrite(&fh, sizeof(cabecera_archivo_t), 1, f) != 1 ||
        fwrite(&ih, sizeof(cabecera_info_t), 1, f) != 1) {
        fclose(f);
        return 0;
    }

    unsigned char padding_bytes[3] = {0, 0, 0};

    for (int i = alto - 1; i >= 0; i--) {
        if (fwrite(img->pixeles[i], sizeof(pixel_t), ancho, f) != (size_t)ancho) {
            fclose(f);
            return 0;
        }
        if (padding > 0) {
            if (fwrite(padding_bytes, 1, padding, f) != (size_t)padding) {
                fclose(f);
                return 0;
            }
        }
    }

    fclose(f);
    return 1;
}
