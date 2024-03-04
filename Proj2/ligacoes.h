/* iaed-23 - ist1107041 - project1 */
/*
 * File:  ligacoes.h
 * Author:  André Antunes Santos
 * Description: Declaracoes das Funcoes das Ligacoes
*/
#ifndef _LIGACOES_H_
#define _LIGACOES_H_

#include "structures.h"

/*Declaracao das Funcoes das Ligacoes*/

void handle_l_func(nodeParagem*, nodeCarreira*);

void free_aux_ligacao(char* , char* , char* );

void le_auxiliares_ligacao(char** , char** , char** );

int erros_ligacao(char*,char*,char*,nodeCarreira*,nodeParagem*,nodeParagem*,double,double);

nodeLigacao* newLigacao (char* , char* , double , double );

nodeLigacao* addLigacaoBegin(nodeLigacao* , char* , char* , double, double);

nodeLigacao* addLigacaoLast(nodeLigacao*, char* , char*, double , double);

void add_ligacao(nodeCarreira**,nodeParagem**,nodeParagem**,char*,char*,double,double);

void removeLigacao(nodeCarreira**, nodeLigacao**, char*);

void removeLigacaoMeio(nodeCarreira***, nodeLigacao***);

void removeUltimaLigacao(nodeCarreira***, nodeLigacao***);

void removePrimeiraLigacao(nodeCarreira***, nodeLigacao***);

void removeUnicaLigacao(nodeCarreira**, char*);

#endif