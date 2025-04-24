#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PRODUCTO {
    int cod;
    float vida_util;
    char fab[20];
};

struct FALLA {
    int cod;
    char desc_falla[50];
};

int main() {
    FILE *FP_PRO, *FP_FAL, *FP_CON;

    struct PRODUCTO prod, aux;
    struct FALLA fall;

    int N, I, CONT, MAX = 0, MAXCOD = 0, falla;
    char MAXFAB[20];

    printf("====================================\n");
    printf(" CANTIDAD DE FALLAS EN PRODUCTOS\n");
    printf("====================================\n\n");

    // Abrir archivos binarios
    FP_PRO = fopen("PRODUCTOS", "rb");
    if (FP_PRO == NULL) {
        printf("Error al abrir archivo PRODUCTOS.\n");
        return 1;
    }

    FP_FAL = fopen("FALLAS", "rb");
    if (FP_FAL == NULL) {
        printf("Error al abrir archivo FALLAS.\n");
        fclose(FP_PRO);
        return 1;
    }

    // Calcular cantidad de productos
    fseek(FP_PRO, 0, SEEK_END);
    N = ftell(FP_PRO) / sizeof(prod);
    rewind(FP_PRO);

    // Buscar fabricante con más fallas
    for (I = 0; I < N; I++) {
        fseek(FP_PRO, I * sizeof(prod), SEEK_SET);
        fread(&prod, sizeof(prod), 1, FP_PRO);

        CONT = 0;
        rewind(FP_PRO);
        while (fread(&aux, sizeof(prod), 1, FP_PRO) == 1) {
            if (strcmp(prod.fab, aux.fab) == 0) {
                rewind(FP_FAL);
                while (fread(&fall, sizeof(fall), 1, FP_FAL) == 1) {
                    if (aux.cod == fall.cod) {
                        CONT++;
                    }
                }
            }
        }

        if (CONT > MAX) {
            strcpy(MAXFAB, prod.fab);
            MAX = CONT;
        }
    }

    printf("Fabricante con mas fallas: %s (%d fallas)\n", MAXFAB, MAX);

    // Buscar producto con más fallas
    MAX = 0;
    rewind(FP_PRO);
    while (fread(&prod, sizeof(prod), 1, FP_PRO) == 1) {
        CONT = 0;
        rewind(FP_FAL);
        while (fread(&fall, sizeof(fall), 1, FP_FAL) == 1) {
            if (prod.cod == fall.cod) {
                CONT++;
            }
        }

        if (CONT > MAX) {
            MAXCOD = prod.cod;
            MAX = CONT;
        }
    }

    printf("Producto mas fallado: Codigo %d (%d fallas)\n", MAXCOD, MAX);

    // Crear archivo de productos confiables
    FP_CON = fopen("CONFIABLES", "wb");
    if (FP_CON == NULL) {
        printf("Error al crear archivo CONFIABLES.\n");
        fclose(FP_PRO);
        fclose(FP_FAL);
        return 1;
    }

    rewind(FP_PRO);
    while (fread(&prod, sizeof(prod), 1, FP_PRO) == 1) {
        falla = 0;
        rewind(FP_FAL);
        while (fread(&fall, sizeof(fall), 1, FP_FAL) == 1) {
            if (prod.cod == fall.cod) {
                falla = 1;
                break;
            }
        }

        if (falla == 0) {
            fwrite(&prod, sizeof(prod), 1, FP_CON);
        }
    }

    // Cierre de archivos
    fclose(FP_PRO);
    fclose(FP_FAL);
    fclose(FP_CON);

    printf("\nProceso finalizado correctamente.\n");

    return 0;
}
