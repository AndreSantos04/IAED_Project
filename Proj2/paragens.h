/* iaed-23 - ist1107041 - project1 */
/*
 * File:  paragens.h
 * Author:  André Antunes Santos
 * Description: Declaracoes das Funcoes das Paragens
*/

#ifndef _PARAGENS_H_
#define _PARAGENS_H_

#include "structures.h"

/*Funcoes auxiliares das Paragens*/

nodeParagem* findParagem(char*, nodeParagem*);

void scan_p_name(char*);

nodeParagem* addP(char*);

nodeParagem* addP_End(char*, nodeParagem**);

void print_paragens(nodeParagem* );

void print_coords_paragem(nodeParagem* , nodeParagem* , char*);

int existsNameCarreiraP(nomes* , nomes* );

nomes* NewNameCarreiraP(char* , nomes* );

int InsertSorted (nomes** , nomes* );

nomes* sortNomesCarreira(nomes* , int* , char* );

void removeNomeCarreiraP(nodeParagem** , nomes** );

void removeParagem(nodeParagem*** , nodeParagem*** , nodeParagem* );

void free_paragens(nodeParagem** );

nodeParagem* handle_p_func(nodeParagem* , nodeParagem** );

#endif