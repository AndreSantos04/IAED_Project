/* iaed-23 - ist1107041 - project1 */
/*
 * File:  ligacoes.c
 * Author:  André Antunes Santos
 * Description: Funcoes auxiliares das Ligacoes
*/
#include "structures.h"
#include "ligacoes.h"
#include "carreiras.h"
#include "paragens.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*Liberta as variáveis lidas para criar a ligacao*/
void free_aux_ligacao(char*nomeCarreira,char*nomePOrigem,char*nomePDestino){

    free(nomeCarreira);
    free(nomePOrigem);
    free(nomePDestino);
}

/*Lê as variaveis necessárias para criar a ligacao*/
void le_auxiliares_ligacao(char** nomeCarreira, char** nomePOrigem, 
    char** nomePDestino){
    
    *nomeCarreira = (char*) safe_malloc(sizeof(char)* MAX_TAMANHO_LINHA);
    *nomePOrigem = (char*) safe_malloc(sizeof(char)* MAX_TAMANHO_LINHA);
    *nomePDestino = (char*) safe_malloc(sizeof(char)* MAX_TAMANHO_LINHA);
    leEspacos();
    scan_c_name(*nomeCarreira);
    leEspacos();
    scan_p_name(*nomePOrigem);
    leEspacos();
    scan_p_name(*nomePDestino);
}

/*
* Verifica se existem erros para a criacao de uma 
* ligacao de acordo com o input
*/
int erros_ligacao(char* nomeCarreira, char* nomePOrigem, char* nomePDestino, 
    nodeCarreira*x,nodeParagem*po,nodeParagem*pd,double custo,double duracao){
    
    
    int erro = FALSE;
    if(x==NULL){
        printf(NO_SUCH_LINE_ERR,nomeCarreira);
        erro = TRUE;
    }else if(po==NULL){
	    printf(NO_SUCH_STOP_ERR, nomePOrigem);
        erro = TRUE;
    }else if(pd ==NULL){
	    printf(NO_SUCH_STOP_ERR, nomePDestino);
        erro = TRUE;
    }else if (x->carreira.numParagens>0){
    /*
    * Se a origem da ligacao for diferente do fim da carreira 
    * E o destino da ligacao diferente do inicio da carreira 
    * E a carreira já  tiver paragens adicionadas na origem e no destino
    */
        if ((strcmp(x->carreira.lastL->ligacao.nomePDestino, nomePOrigem)!=0) 
        && (strcmp(x->carreira.firstL->ligacao.nomePOrigem,nomePDestino)!=0)){ 
            
            printf("link cannot be associated with bus line.\n");
            erro = TRUE;
        }
    }
    else if (custo<0 || duracao<0){
        printf("negative cost or duration.\n");
        erro = TRUE;
    }
    return erro;
}

/*Cria uma nova ligacao, iniciando os seus valores*/
nodeLigacao* newLigacao (char* nomePOrigem, char*nomePDestino, 
            double duracao, double custo){
    
    nodeLigacao* l =NULL; 
    l = (nodeLigacao*) safe_malloc(sizeof(nodeLigacao));

    l->next=l->prev=NULL;
    l->ligacao.nomePOrigem = NULL; 
    l->ligacao.nomePDestino = NULL;
    l->ligacao.nomePOrigem = (char*) safe_malloc((strlen(nomePOrigem) + 1));
    l->ligacao.nomePDestino = (char*) safe_malloc((strlen(nomePDestino) + 1));

    strcpy(l->ligacao.nomePOrigem, nomePOrigem);  
    strcpy(l->ligacao.nomePDestino,nomePDestino);

    l->ligacao.custo= custo;
    l->ligacao.duracao = duracao;

    return l;
}

/*Adiciona uma ligacao no inicio da carreira*/
nodeLigacao* addLigacaoBegin(nodeLigacao* firstL, char* nomePOrigem, 
            char*nomePDestino, double duracao, double custo){


    nodeLigacao* l = NULL;
    l = newLigacao(nomePOrigem, nomePDestino, duracao, custo);
    l -> next = firstL;
    firstL -> prev = l;
    return l;
}

/*Adiciona a ligacao no final da carreira, devolve a Primeira Ligacao*/
nodeLigacao* addLigacaoLast(nodeLigacao*lastL, char* nomePOrigem, 
            char*nomePDestino, double duracao, double custo){


    nodeLigacao* l = NULL;
    l = newLigacao(nomePOrigem, nomePDestino, duracao, custo);
    l->prev = lastL;
    (lastL)->next = l;
    (lastL) = l;
    return lastL;
}

/*Adiciona uma nova ligacao à carreira*/
void add_ligacao(nodeCarreira **x, nodeParagem **po, nodeParagem **pd, 
    char *nomePOrigem, char *nomePDestino, double custo, double duracao ){

    /* Adiciona a primeira ligacao*/
    if ((*x)->carreira.numParagens==0){
        
        (*x)->carreira.firstL=newLigacao(nomePOrigem,nomePDestino,duracao,custo);
        (*x)->carreira.lastL = (*x)->carreira.firstL;
        (*x)->carreira.numParagens+=2;
        (*po)->paragem.carreirasNomes = 
                sortNomesCarreira((*po)->paragem.carreirasNomes,
                &(*po)->paragem.numCarreiras,(*x)->carreira.nomeCarreira);   
        
        
        (*pd)->paragem.carreirasNomes = 
                sortNomesCarreira((*pd)->paragem.carreirasNomes,
                &(*pd)->paragem.numCarreiras,(*x)->carreira.nomeCarreira);

    }
    /*Transforma numa carreira Circular*/
    else if(!strcmp((*x)->carreira.lastL->ligacao.nomePDestino, nomePOrigem) && 
            !strcmp((*x)->carreira.firstL->ligacao.nomePOrigem,nomePDestino)){
        
        (*x)->carreira.lastL = addLigacaoLast((*x)->carreira.lastL,nomePOrigem, 
                                nomePDestino, duracao, custo);

        (*x)->carreira.numParagens++;
    }
    /*Adiciona Ligacao no Fim*/
    else if (!strcmp((*x)->carreira.lastL->ligacao.nomePDestino, nomePOrigem)){

        (*x)->carreira.lastL = addLigacaoLast((*x)->carreira.lastL,nomePOrigem,
                                 nomePDestino, duracao, custo);
        
        (*x)->carreira.numParagens++;
        (*pd)->paragem.carreirasNomes = 
                sortNomesCarreira((*pd)->paragem.carreirasNomes,
                &(*pd)->paragem.numCarreiras,(*x)->carreira.nomeCarreira);
    }
    /*Adiciona Ligacao no Inicio*/
    else if (!strcmp((*x)->carreira.firstL->ligacao.nomePOrigem,nomePDestino)){

        (*x)->carreira.firstL=addLigacaoBegin((*x)->carreira.firstL,nomePOrigem
                                 ,nomePDestino, duracao, custo);
        (*x)->carreira.numParagens++;
        (*po)->paragem.carreirasNomes = 
                sortNomesCarreira((*po)->paragem.carreirasNomes,
                &(*po)->paragem.numCarreiras,(*x)->carreira.nomeCarreira);
    }
}


/*Remove a única ligacao de uma carreira*/
void removeUnicaLigacao(nodeCarreira **x, char *nameP){
    nodeLigacao* l = (*x)->carreira.firstL;
    /*Caso o nome corresponda a uma das paragens*/
    if(!strcmp(l->ligacao.nomePOrigem,nameP)||
        !strcmp(l->ligacao.nomePDestino,nameP)){

        free(l->ligacao.nomePDestino);
        free(l->ligacao.nomePOrigem);
        free(l);
        (*x)->carreira.custoTotal=0.0;
        (*x)->carreira.duracaoTotal=0.0;
        (*x)->carreira.numParagens=0;
        (*x)->carreira.firstL = (*x)->carreira.lastL = NULL;
    }
}

/*Remove a primeira ligacao, passa a segunda a ser a primeira ligacao*/
void removePrimeiraLigacao(nodeCarreira ***x, nodeLigacao ***l){
    
    nodeLigacao* next = (**l)->next;

    (**x)->carreira.custoTotal-=(**l)->ligacao.custo;
    (**x)->carreira.duracaoTotal-=(**l)->ligacao.duracao;

    next->prev = NULL;
    (**x)->carreira.firstL = next;
    (**x)->carreira.numParagens--;
}

/*Remove a ultima ligacao, passando a penultima a ser a ultima ligacao*/
void removeUltimaLigacao(nodeCarreira ***x, nodeLigacao ***l){
    nodeLigacao* prev = (**l)->prev;
    
    (**x)->carreira.custoTotal-=(**x)->carreira.lastL->ligacao.custo;
    (**x)->carreira.duracaoTotal-=(**x)->carreira.lastL->ligacao.duracao;

    prev->next = NULL;
    (**x)->carreira.lastL = prev;
    (**x)->carreira.numParagens--; 
}

/*
* Remove uma ligacao do meio e passando os valores do custo e duracao 
* para a ligacao anterior
*/
void removeLigacaoMeio(nodeCarreira ***x, nodeLigacao ***l){
    nodeLigacao* next = (**l)->next;
    nodeLigacao* prev = (**l)->prev;  
    
    free(prev->ligacao.nomePDestino);
    prev->ligacao.nomePDestino = safe_malloc(sizeof(char)*
                                (strlen((**l)->ligacao.nomePDestino)+1));

    strcpy(prev->ligacao.nomePDestino,(**l)->ligacao.nomePDestino);

    prev->ligacao.custo+=(**l)->ligacao.custo;
    prev->ligacao.duracao+=(**l)->ligacao.duracao;
    (**x)->carreira.numParagens-=1;

    if(next==NULL){
        prev->next=NULL;
        (**x)->carreira.lastL = prev;

    }
    else{
        next->prev=prev;
        prev->next=next;
    }  
}

/*Remove todas as ligacoes onde a paragem corresponde a paragem a remover*/
void removeLigacao(nodeCarreira **x, nodeLigacao **l, char *nameP){
    while((*l)!=NULL){
        int e = FALSE;
        nodeLigacao* next = (*l)->next;
        nodeLigacao* prev = (*l)->prev; 
        if(!strcmp((*l)->ligacao.nomePOrigem,nameP)){
            if(prev==NULL)
                removePrimeiraLigacao(&x,&l);
            else
                removeLigacaoMeio(&x,&l);
            
            e=TRUE;
        }
        /*Remove ultima ligacao*/
        if(next==NULL && !strcmp((*l)->ligacao.nomePDestino,nameP)){
                removeUltimaLigacao(&x,&l);
                e = TRUE;
        }
        if (e==TRUE){
            free((*l)->ligacao.nomePDestino);
            free((*l)->ligacao.nomePOrigem);
            free(*l);
        }
        *l= next;
    }              
}
