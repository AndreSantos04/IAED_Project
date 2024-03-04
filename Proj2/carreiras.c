/* iaed-23 - ist1107041 - project1 */
/*
 * File:  carreiras.c
 * Author:  André Antunes Santos
 * Description: Funcoes auxiliares das Carreiras
*/

#include "structures.h"
#include "carreiras.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*
* Dá print das paragens de uma carreira, por ordem de criacao ou inversa, 
* dependendo do input
*/
void print_ligacoesCarreira(nodeCarreira* x){
    int c = leEspacos();
    if (c=='\n'){
        if (x->carreira.numParagens>0){
            nodeLigacao* n;  
            /*Imprime por ordem de criacao*/
            for(n=x->carreira.firstL;n->next!=NULL;n=n->next){ 
                printf("%s, ",n->ligacao.nomePOrigem);
            }
            printf("%s, %s\n",n->ligacao.nomePOrigem,n->ligacao.nomePDestino); 
        }
    }
    /*Imprime por ordem inversa*/
    else{
        print_inverso(x);
    }
}

/*Retorna a carreira com o nome fornecido ou NULL caso não exista*/
nodeCarreira* findCarreira(char *nameC, nodeCarreira* firstC){
    nodeCarreira* x;

    for(x=firstC;x!=NULL;x=x->next){
        if(strcmp(nameC,x->carreira.nomeCarreira)==0){
            return x;
        }
    }

    return NULL;
}

/* Lê o input correspondente ao nome da carreira e retorna-o */
void scan_c_name(char* name){
    int c;

    while((c=getchar())){

        if (isspace(c))
            break;
        else
            *name++=c;
    }
    *name++='\0';
    ungetc(c,stdin);
}

/* Cria uma nova carreira com o nome fornecido e os restantes valores a 0 */
nodeCarreira* addC(char *name){
    nodeCarreira* x = NULL;
    x = (nodeCarreira*) safe_malloc(sizeof(struct nodeCarreiras));
    x->next = x->prev = NULL;
    
    x->carreira.nomeCarreira = NULL;
    x->carreira.nomeCarreira=(char*)safe_malloc(sizeof(char)*(strlen(name)+1));
    strcpy(x->carreira.nomeCarreira,name);

    x->carreira.custoTotal = 0.0;
    x->carreira.numParagens = 0;
    x->carreira.duracaoTotal = 0.0;

    x->carreira.firstL=NULL;
    x->carreira.lastL=NULL;

    return x;
}

/*Adciona uma carreira ao fim da lista de carreiras*/
nodeCarreira* addC_End(char* name, nodeCarreira* *lastC){
    nodeCarreira *x=NULL;

    x = addC(name);
    x->prev = *lastC;
    (*lastC)->next = x;
    (*lastC) = x;

    return *lastC;
}

/*
* Verifica se o argumento opcional fornecido é a string "inverso"
* Ou alguma abreviatura com 3 ou mais caracteres
*/
void print_inverso(nodeCarreira* x){
    int inv = 0, length_inverso = 0;
    char *inverso = NULL;
    inverso= (char*) safe_malloc(sizeof(char)* MAX_TAMANHO_LINHA);

    scanf("%s",inverso);
    length_inverso = strlen(inverso);
    
    if(length_inverso>=LENGTH_MIN_INVERSO&&length_inverso<=LENGTH_MAX_INVERSO){
        
        if(!strncmp(inverso,INVERSO,length_inverso)){
            inv = 1;
        }
    }
    if (inv == 1){
        if (x->carreira.numParagens>0){
            nodeLigacao *n;
            for(n=x->carreira.lastL; n->prev!=NULL;n=n->prev){
                printf("%s, ",n->ligacao.nomePDestino);
            }
            printf("%s, %s\n",n->ligacao.nomePDestino, n->ligacao.nomePOrigem);
        }
    }
    else{
        printf("incorrect sort option.\n");
    } 
    free(inverso);
}

/*Imprime as características de todas as carreiras*/
void print_carreiras(nodeCarreira* firstC){
    nodeCarreira* x=NULL;

    for (x=firstC;x!=NULL;x=x->next){

        if (x->carreira.numParagens==0){
            printf("%s %d %.2f %.2f\n", x->carreira.nomeCarreira,
                x->carreira.numParagens, x->carreira.custoTotal,
                x->carreira.duracaoTotal);
        }
        else{
            printf("%s %s %s %d %.2f %.2f\n", x->carreira.nomeCarreira,
                x->carreira.firstL->ligacao.nomePOrigem,
                x->carreira.lastL->ligacao.nomePDestino, 
                x->carreira.numParagens, x->carreira.custoTotal,
                x->carreira.duracaoTotal);
        }
    }

}

/* Liberta a memória de uma carreira */
void free_carreiras(nodeCarreira*x){
    nodeLigacao *currentL = x->carreira.firstL;


    if (x->carreira.firstL != NULL){
        while (currentL!=NULL){
            nodeLigacao *next = currentL->next;

            free(currentL->ligacao.nomePOrigem);
            currentL->ligacao.nomePOrigem = NULL;

            free(currentL->ligacao.nomePDestino);
            currentL->ligacao.nomePDestino = NULL;
            
            free(currentL);
            currentL = NULL;

            currentL = next;
            
        }
    }
    free(x->carreira.nomeCarreira);

    x->carreira.custoTotal = 0;
    x->carreira.duracaoTotal = 0;

    free(x);
    x = NULL;
}

/*Remove a carreria da lista de carreiras*/
void removeCarreira(nodeCarreira***firstC,nodeCarreira***lastC,nodeCarreira*x){
    nodeCarreira* prev = NULL;
    nodeCarreira* next = NULL;
    prev = x->prev;
    next = x->next;
    
    if(x->prev!=NULL){
        prev->next = next;
    }else{ 
        /*Caso seja a primeira paragem*/
        *(*firstC)=next;
    }
    if(x->next!=NULL){
        next->prev = prev;    
    }else{
        /*Caso seja a ultima paragem*/
        *(*lastC)=prev;
    }
    free_carreiras(x);
}
