/* iaed-23 - ist1107041 - project1 */
/*
 * File:  project2.c
 * Author:  André Antunes Santos
 * Description: Criacao de um sistema de carreiras de transporte publico
*/

#include "structures.h"
#include "paragens.h"
#include "carreiras.h"
#include "ligacoes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*Garante que existe memória para alocar as variaveis*/
void* safe_malloc(unsigned int size){
	void* p = malloc(size);
	if (p != NULL) 
        return p;

	printf(ERR_NO_MEMORY);
	exit(1);
	return NULL;
}

/*Lê os espacos do input*/
int leEspacos(){
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return c;
    ungetc(c, stdin);
    return c;
}

/*
* Funcao Main
* Lê a primeira letra de cada linha de comando
* Iniciando a funcao respetiva à letra
* Termina quando recebe a letra 'q'
*/
int main(){
    nodeParagem* firstP =  NULL, *lastP = NULL;
    nodeCarreira* firstC = NULL, *lastC = NULL;
    int c;
    while ((c=getchar())!=EOF){
        switch (c){
        case 'p':
            firstP = handle_p_func(firstP, &lastP);
            break;
        case 'c':
            firstC = handle_c_func(firstC, &lastC);
            break;
        case 'l':
            handle_l_func(firstP, firstC);
            break;
        case 'i':
            handle_i_func(firstP);
            break;
        case 'e':
            handle_e_func(&firstP, &firstC, &lastP);
            break;
        case 'r':
            handle_r_func(&firstP, &firstC, &lastC);
            break;
        case 'a':
            handle_a_func(&firstP, &firstC);
            firstP =  NULL;
            lastP = NULL;
            firstC = NULL;
            lastC = NULL;
            break;
        case 'q':
            handle_a_func(&firstP, &firstC);
            return 0;
        }
    }
    return 0;
}

/* 
* Funcao para o comando da letra 'p' (Paragens)
* Caso não receba argumentos, escreve todas as paragens, as coordenadas e o 
* numero de carreiras que passam na paragem
* Caso contrário cria uma nova paragem, se forem fornecidas coordenadas
* Ou escreve as coordenadas da paragem pedida
*/
nodeParagem* handle_p_func(nodeParagem* firstP, nodeParagem* *lastP){
    int c;
    nodeParagem* p = NULL;  
    char *nameP = (char*) safe_malloc(sizeof(char) * MAX_TAMANHO_LINHA);
    
    if ((c=leEspacos())!='\n'){   
        scan_p_name(nameP);
        p = findParagem(nameP,firstP);
        
        if(p==NULL||firstP==NULL){
            if (c=='\n'){
                /*Caso não existe a paragem que se pretende imprimir*/
                printf(NO_SUCH_STOP_ERR, nameP);
            }
            else{
                /*Caso sejam fornecidas coordenadas*/
                if(firstP==NULL)
                    *lastP = firstP = addP(nameP);
                else
                    *lastP=addP_End(nameP, lastP);
            }
        }
        else
            print_coords_paragem(firstP, p, nameP);
    }else
        print_paragens(firstP);
    free(nameP);
    return firstP;
}

/* 
* Funcao para o comando da letra 'c' (Carreiras)
* Caso não receba argumentos, escreve todas as carreiras, 
* as paragens de origem e destino, o número de paragens da carreira,
* o custo e a duracao total para cada carreira;
*
* Caso contrário cria uma nova carreira, ou imprime as suas paragens se a 
* carreira já existir, por ordem de criação ou ordem inversa
*/
nodeCarreira* handle_c_func(nodeCarreira* firstC, nodeCarreira* *lastC){
    int c;
    char *nameC = NULL;
    nodeCarreira *x = NULL;
    nameC = (char*) safe_malloc(sizeof(char)*(MAX_TAMANHO_LINHA));

    /*Caso sejam fornecidos argumentos*/
    if ((c=leEspacos())!='\n'){    
        scan_c_name(nameC);
        x = findCarreira(nameC, firstC);

        /*Cria uma nova carreira*/
        if(x==NULL||firstC==NULL){
            if (firstC==NULL)
                *lastC = firstC = addC(nameC);
            else
                *lastC = addC_End(nameC, lastC);
        }
        else
            print_ligacoesCarreira(x);
    }
    else{
        print_carreiras(firstC);
    }
    free(nameC);
    return firstC;
}

/* 
* Funcao para o comando da letra 'l' (Ligacoes)
* Cria uma nova ligacao para uma carreira
*/
void handle_l_func(nodeParagem* firstP, nodeCarreira* firstC){
    double custo, duracao;
    char *nomeCarreira= NULL;
    char *nomePOrigem = NULL; 
    char *nomePDestino = NULL;
    nodeCarreira* x = NULL;
    nodeParagem* po = NULL;
    nodeParagem* pd = NULL;
    int error;

    le_auxiliares_ligacao(&nomeCarreira, &nomePOrigem, &nomePDestino);
    scanf("%lf %lf", &custo, &duracao);

    x=findCarreira(nomeCarreira, firstC);
    po=findParagem(nomePOrigem, firstP);
    pd = findParagem(nomePDestino, firstP);


    error = erros_ligacao(nomeCarreira,nomePOrigem, nomePDestino, x, po, pd,
            custo, duracao);
    if (error==TRUE){
        free_aux_ligacao(nomeCarreira,nomePOrigem, nomePDestino);
        return;
    }

    add_ligacao(&x,&po, &pd, nomePOrigem, nomePDestino, custo, duracao);

    free_aux_ligacao(nomeCarreira,nomePOrigem, nomePDestino);

    x->carreira.custoTotal+=custo;
    x->carreira.duracaoTotal+=duracao;
}

/*
* Funcao para o comando da letra 'i' (Intersecoes)
* Caso numa paragem pare mais de uma carreira, imprime todas as carreiras que 
* aí parem por ordem alfabética
*
*/
void handle_i_func(nodeParagem* firstP){
    nodeParagem* p = NULL;
    leEspacos();

    for(p = firstP; p != NULL; p = p->next){
        if(p->paragem.numCarreiras > 1){
            nomes *n = NULL;
            printf("%s %d:", p->paragem.nomeParagem, p->paragem.numCarreiras);
            
            for(n = p->paragem.carreirasNomes; n!= NULL; n = n->next){
                printf(" %s", n->nome);
            }
            printf("\n");    
        }
    }
}

/*
* Funcao para o comando da letra 'a'
* Liberta a memoria armazenada por todas as paragens/carreiras
*/
void handle_a_func(nodeParagem**firstP, nodeCarreira**firstC){
    leEspacos();
    /*Carreiras*/
    if (*firstC!=NULL){
        nodeCarreira*x=NULL; 
        x = *firstC;
        while(x!=NULL){
            nodeCarreira* next = x ->next;
            free_carreiras(x);
            x = next;
        }
    }
    /*Paragens*/
    if (*firstP!=NULL){
        nodeParagem*p=NULL;
        p = *firstP;
        while (p!= NULL){
            nodeParagem* next = p->next;
            free_paragens(&p);
            p = next;
        }
    }
}

/*
* Funcao para o comando da letra 'r'
* Remove uma carreira do sistema de carreiras 
* E remove-a de todas as paragens por onde passa
*/
void handle_r_func(nodeParagem**firstP,nodeCarreira**firstC,nodeCarreira**lC){

    char* nameC = (char*) safe_malloc (sizeof(char)* MAX_TAMANHO_LINHA);
    nodeCarreira* x = NULL;
    nodeParagem* p = NULL;


    leEspacos();
    scan_c_name(nameC);
    x = findCarreira(nameC, *firstC);

    if(x==NULL){
        printf(NO_SUCH_LINE_ERR, nameC);
    } else{
        removeCarreira(&firstC,&lC, x);

        for(p=(*firstP);p!=NULL;p=p->next){
            nomes* n=p->paragem.carreirasNomes;
            while (n!=NULL){
                nomes* next = n->next;
                if(!strcmp(n->nome,nameC))
                    removeNomeCarreiraP(&p,&n);
                n=next;
            }
        }
    }
    leEspacos();
    free(nameC);
}

/*
* Funcao para o comando da letra 'e'    
* Remove uma paragem da lista de paragens
* E remove as ligacoes onde uma das paragens corresponde a paragem a remover
*/
void handle_e_func(nodeParagem**firstP,nodeCarreira**firstC,nodeParagem**lP){
    char* nameP = (char*) safe_malloc (sizeof(char)* MAX_TAMANHO_LINHA);
    nodeCarreira* x = NULL;
    nodeParagem* p=NULL;

    leEspacos();
    scan_c_name(nameP);
    p = findParagem(nameP, *firstP);
    if((*firstP==NULL||(*firstP)->paragem.nomeParagem==NULL)||p==NULL)
        printf(NO_SUCH_STOP_ERR, nameP);
    else{
        removeParagem(&firstP, &lP, p);
        for(x=(*firstC);x!=NULL;x=x->next){
            nodeLigacao* l = x->carreira.firstL;
            if(x->carreira.numParagens==2)
                removeUnicaLigacao(&x, nameP);
            else{
                removeLigacao(&x, &l, nameP);  
            }
        }
    }
    leEspacos();
    free(nameP);
}
