#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ProcesarArgumentos.h"
#include "funciones_grupo.h"

#define NOMBRE_GRUPO "VARIABLE"

static int filtro_ya_procesado(char** procesados, int cant, const char* filtro) {
    for (int i = 0; i < cant; i++) {
        if (strcmp(procesados[i], filtro) == 0) return 1;
    }
    return 0;
}

int procesar_comandos_flexibles(int argc, char* argv[], int idx_imagen, imagen_bmp_t* img1, const char* nombre_base) {
    char* filtros_ejecutados[64];
    int cant_filtros = 0;

    for (int i = 1; i < argc; i++) {
        // Si el argumento actual es la ruta de la imagen principal, lo salteamos
        if (i == idx_imagen) continue;

        char* arg = argv[i];

        // Si no empieza con "--", es un argumento secundario suelto no reconocido
        if (strncmp(arg, "--", 2) != 0) {
            printf("Aviso: Argumento '%s' ignorado (no es una opcion valida).\n", arg);
            continue;
        }

        // Control de duplicados
        if (filtro_ya_procesado(filtros_ejecutados, cant_filtros, arg)) {
            printf("Aviso: El filtro '%s' ya fue ejecutado. Se ignora el duplicado.\n", arg);
            continue;
        }

        imagen_bmp_t* img_res = NULL;
        char tag_filtro[128] = "";

        // --- FILTROS SIN PARÁMETROS ---
        if (strcmp(arg, "--negativo") == 0) {
            img_res = aplicar_negativo(img1);
            strcpy(tag_filtro, "negativo");
        }
        else if (strcmp(arg, "--escala-de-grises") == 0) {
            img_res = aplicar_escala_grises(img1);
            strcpy(tag_filtro, "escala-de-grises");
        }
        else if (strcmp(arg, "--espejar-horizontal") == 0) {
            img_res = aplicar_espejar_horizontal(img1);
            strcpy(tag_filtro, "espejar-horizontal");
        }
        else if (strcmp(arg, "--espejar-vertical") == 0) {
            img_res = aplicar_espejar_vertical(img1);
            strcpy(tag_filtro, "espejar-vertical");
        }
        else if (strcmp(arg, "--rotar-derecha") == 0) {
            img_res = aplicar_rotar(img1, 1);
            strcpy(tag_filtro, "rotar-derecha");
        }
        else if (strcmp(arg, "--rotar-izquierda") == 0) {
            img_res = aplicar_rotar(img1, 0);
            strcpy(tag_filtro, "rotar-izquierda");
        }
        else if (strcmp(arg, "--comodin") == 0) {
            img_res = aplicar_comodin(img1);
            strcpy(tag_filtro, "comodin");
        }
        // --- FILTROS CON PARÁMETROS NUMÉRICOS ---
        else {
            char nombre_filtro[64];
            int val;
            if (sscanf(arg, "--tonalidad-%62[^--]=%d", nombre_filtro, &val) == 2 ||
                sscanf(arg, "--%62[^--]=%d", nombre_filtro, &val) == 2) {

                if (val >= 0 && val <= 100) {
                    if (strcmp(nombre_filtro, "tonalidad-azul") == 0) {
                        img_res = aplicar_tonalidad(img1, 'b', val);
                        strcpy(tag_filtro, "tonalidad-azul");
                    } else if (strcmp(nombre_filtro, "tonalidad-verde") == 0) {
                        img_res = aplicar_tonalidad(img1, 'g', val);
                        strcpy(tag_filtro, "tonalidad-verde");
                    } else if (strcmp(nombre_filtro, "tonalidad-roja") == 0) {
                        img_res = aplicar_tonalidad(img1, 'r', val);
                        strcpy(tag_filtro, "tonalidad-roja");
                    } else if (strcmp(nombre_filtro, "aumentar-contraste") == 0) {
                        img_res = aplicar_contraste(img1, val, 1);
                        strcpy(tag_filtro, "aumentar-contraste");
                    } else if (strcmp(nombre_filtro, "reducir-contraste") == 0) {
                        img_res = aplicar_contraste(img1, val, 0);
                        strcpy(tag_filtro, "reducir-contraste");
                    } else if (strcmp(nombre_filtro, "recortar") == 0 && val > 0) {
                        img_res = aplicar_recortar(img1, val);
                        strcpy(tag_filtro, "recortar");
                    } else if (strcmp(nombre_filtro, "achicar") == 0 && val > 0) {
                        img_res = aplicar_achicar(img1, val);
                        strcpy(tag_filtro, "achicar");
                    } else {
                        printf("Aviso: Comando con parametro desconocido '%s'. Se ignora.\n", arg);
                    }
                } else {
                    printf("Error: Porcentaje fuera de rango (0-100) en %s\n", arg);
                }
            }
            // --- FILTROS DE CONCATENACIÓN ---
            else if (strcmp(arg, "--concatenar-horizontal") == 0 || strcmp(arg, "--concatenar-vertical") == 0) {
                if (i + 1 < argc && strncmp(argv[i + 1], "--", 2) != 0) {
                    const char* ruta_img2 = argv[++i];
                    imagen_bmp_t* img2 = cargar_bmp(ruta_img2);
                    if (img2) {
                        int vertical = (strcmp(arg, "--concatenar-vertical") == 0);
                        pixel_t azul_relleno = {255, 0, 0};
                        img_res = aplicar_concatenar(img1, img2, vertical, azul_relleno);
                        strcpy(tag_filtro, vertical ? "concatenar-vertical" : "concatenar-horizontal");
                        liberar_imagen(img2);
                    } else printf("Error: No se pudo cargar la segunda imagen '%s'\n", ruta_img2);
                } else printf("Error: Falta el argumento de la segunda imagen para concatenar.\n");
            }
            else {
                // Cualquier argumento no reconocido se notifica y se ignora sin detener el programa
                printf("Aviso: Comando desconocido '%s'. Se ignora.\n", arg);
            }
        }

        // Si se generó la imagen con éxito, se guarda
        if (img_res) {
            char nombre_salida[512];
            snprintf(nombre_salida, sizeof(nombre_salida), "%s_%s_%s.bmp", NOMBRE_GRUPO, tag_filtro, nombre_base);

            if (guardar_bmp(nombre_salida, img_res)) {
                printf("Archivo generado exitosamente: %s\n", nombre_salida);
            } else {
                printf("Error al guardar la imagen: %s\n", nombre_salida);
            }
            liberar_imagen(img_res);

            filtros_ejecutados[cant_filtros] = strdup(arg);
            cant_filtros++;
        }
    }

    for (int i = 0; i < cant_filtros; i++) {
        free(filtros_ejecutados[i]);
    }
    return 0;
}
