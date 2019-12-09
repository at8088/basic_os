#ifndef AFFICHE_CONSOLE_H
#define AFFICHE_CONSOLE_H


#include <cpu.h>
#include <inttypes.h>
#include <ctype.h>
#include <string.h>


uint32_t pos_lig_cur;
uint32_t pos_col_cur;
uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, char c, char couleur);
void efface_ecran(void);
void place_curseur(uint32_t lig, uint32_t col);
void traite_car(char c);
void defilement(void);
void haut_droite(char *s);
#endif