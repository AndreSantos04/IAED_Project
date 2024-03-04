/* iaed-23 - ist1107041 - project1 */
/*
 * File:  carreiras.h
 * Author:  André Antunes Santos
 * Description: Declaracoes das Funcoes das Carreiras
*/

#ifndef _CARREIRAS_H_
#define _CARREIRAS_H_

#include "structures.h"

/*Declaracao das Funcoes das Carreiras*/
nodeCarreira* handle_c_func(nodeCarreira*, nodeCarreira**);

nodeCarreira* findCarreira(char* , nodeCarreira*);

void scan_c_name(char* );

nodeCarreira* addC(char*);

nodeCarreira* addC_End(char*, nodeCarreira**);

void print_inverso(nodeCarreira* );

void print_carreiras(nodeCarreira*);

void print_ligacoesCarreira(nodeCarreira* x);

void free_carreiras(nodeCarreira* );

void removeCarreira(nodeCarreira*** , nodeCarreira*** , nodeCarreira* );

#endif