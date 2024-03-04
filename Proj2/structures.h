/* iaed-23 - ist1107041 - project1 */
/*
 * File:  structures.h
 * Author:  André Antunes Santos
 * Description: Declaracoes de estruturas e funcoes do projeto
*/

#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#define ERR_NO_MEMORY "No memory.\n"
#define EXIT_CODE_NO_MEMORY 1
#define NO_SUCH_STOP_ERR "%s: no such stop.\n"
#define NO_SUCH_LINE_ERR "%s: no such line.\n"

#define INVERSO "inverso"
#define LENGTH_MAX_INVERSO 7
#define LENGTH_MIN_INVERSO 3

#define TRUE 1
#define FALSE 0

#define MAX_TAMANHO_LINHA 65536

/*Structures*/
typedef struct Nomes{
    char* nome;
    struct Nomes* next, *prev;
}nomes;

/*
* Estrutura que define uma paragem
* Caracterizada por: 
* Nome da paragem;
* Coordenadas latitude e longitude;
* Número de carreiras que passam pela paragem;
* Índice no vetor carreiras das carreiras que passam pela paragem;
*/
typedef struct paragens{
    char *nomeParagem;
    double latitude;
    double longitude;
    int numCarreiras;
    nomes *carreirasNomes;
} Paragem;

typedef struct nodeParagens{
    Paragem paragem;
    struct nodeParagens *next, *prev;
} nodeParagem;

typedef struct ligacoes{

    char *nomePOrigem; 
    char *nomePDestino;
    double custo, duracao;
} Ligacao;

typedef struct nodeLigacoes{
    Ligacao ligacao;
    struct nodeLigacoes *next, *prev;
} nodeLigacao;

/*
* Estrutura que define uma Carreira
* Caracterizada por: 
* Nome da carreira;
* Primeira e última paragens paragemOrigem e paragemDestino;
* Nomes das paragens por onde a carreira passa;
* Número de paragens por onde a carreira passa;
* Custo e duracao total da viagem na carreira;
*/
typedef struct carreiras{
    char *nomeCarreira;
    nodeLigacao *firstL;
    nodeLigacao *lastL;
    int numParagens;
    double custoTotal;
    double duracaoTotal;
} Carreira;

typedef struct nodeCarreiras{
    Carreira carreira;
    struct nodeCarreiras *next, *prev;
} nodeCarreira;


/*Declaracao das Funcoes*/
void* safe_malloc(unsigned int);

int leEspacos();

void handle_a_func(nodeParagem**, nodeCarreira**);

void handle_i_func(nodeParagem*);

void handle_r_func(nodeParagem** , nodeCarreira** , nodeCarreira**);

void handle_e_func(nodeParagem**, nodeCarreira**, nodeParagem**);


#endif