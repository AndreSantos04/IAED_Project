/* iaed-23 - ist1107041 - project1 */
/*
 * File:  paragens.c
 * Author:  André Antunes Santos
 * Description: Funcoes auxiliares das Paragens
*/

#include "structures.h"
#include "paragens.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*Lê o input correspondente ao nome da paragem e retorna-o*/
void scan_p_name(char *name){
    int c = getchar();

    if (c != '"') {
        while (c != ' ' && c != '\t' && c != '\n'){
            *name++ = c;
            c = getchar();
        }
        ungetc(c,stdin);
    }
    else {
        while((c = getchar()) != '"')
            *name++ = c;
    }
    *name++='\0';
}

/*Retorna a paragem com o nome fornecido ou NULL caso não exista*/
nodeParagem* findParagem(char *nameP, nodeParagem* firstP ){
    nodeParagem* p;

    for(p=firstP;p!=NULL;p=p->next){    
        if(strcmp(nameP,p->paragem.nomeParagem)==0){    
            return p;
        }
    }
    return NULL;
}

/* Cria uma nova paragem com o nome e as coordenadas fornecidas */
nodeParagem* addP(char *name){
    nodeParagem* p = NULL;
    p = (nodeParagem*) safe_malloc(sizeof(struct nodeParagens));
    p->paragem.nomeParagem = NULL;
    p->paragem.nomeParagem =(char*)safe_malloc(sizeof(char)*(strlen(name)+1));
    p->paragem.carreirasNomes = NULL;
    strcpy(p->paragem.nomeParagem,name);
    scanf("%lf%lf", &p->paragem.latitude, &p->paragem.longitude);
    p->next = p->prev = NULL;
    p->paragem.numCarreiras = 0;

    return p;
}

/*Adciona uma paragem ao fim da lista de paragem*/
nodeParagem* addP_End(char *name, nodeParagem* *lastP){
    nodeParagem* p = NULL;

    p= addP(name);
    p->prev = *lastP;
    (*lastP)->next = p;
    (*lastP) = p;
 
    return *lastP;
}

/*Imprime as características de todas as paragens*/
void print_paragens(nodeParagem* firstP){
    nodeParagem* p = NULL;    
    if (firstP!=NULL){
        for(p=firstP;p!=NULL; p=p->next){
            printf("%s: %16.12f %16.12f %d\n", p->paragem.nomeParagem, 
            p->paragem.latitude, p->paragem.longitude, 
            p->paragem.numCarreiras);
        }
    }
}

/*Imprime as coordenadas de uma paragem*/
void print_coords_paragem(nodeParagem* firstP, nodeParagem* p, char *nameP){
    int c = leEspacos();
    if (firstP!=NULL){
        if (c=='\n'){
            printf("%16.12f %16.12f\n", p->paragem.latitude,
            p->paragem.longitude);
        }
        else{
            /*Caso sejam fornecidas coordenadas duma paragem já definida*/
            printf("%s: stop already exists.\n",nameP);
        }
    }
}

/*Cria um novo nomeCarreira para uma nova carreira que passa numa paragem*/
nomes* NewNameCarreiraP(char* nCarreira, nomes* n){
    n = (nomes*) safe_malloc(sizeof(struct Nomes));
    n->next = n->prev = NULL;
    n->nome = NULL;
    n->nome = (char*) safe_malloc(strlen(nCarreira)+1);
    strcpy(n->nome,nCarreira);
    n->next = NULL;
    n->prev = NULL;

    return n;
}

/*Vê se o nome a ser inserido já foi inserido anteriormente*/
int existsNameCarreiraP(nomes* current, nomes* newName){

    if(!strcmp(current->nome,newName->nome)){
        free(newName->nome);
        newName->nome=NULL;
        free(newName);
        newName = NULL;
        return TRUE;
    }
    return FALSE;
}

/*
* Itera até encontrar o sítio onde o nome vai ser inserido, 
* de modo a ficar ordenado alfabeticamente
*/
int InsertSorted (nomes** firstN, nomes *newName){
    nomes* current = *firstN;

    while (current->next != NULL && current->next->nome !=NULL &&
        strcmp(current->next->nome,newName->nome)<=0){

        current = current->next;
        /*Caso já exista o nome na lista nao volta a inserir*/ 
        if ((existsNameCarreiraP(current,newName)) == TRUE )
            return FALSE;
    }
    if ((existsNameCarreiraP(current,newName)) == TRUE)
        return FALSE;

    newName->next = current->next;
    if (current->next != NULL)
        newName->next->prev = newName;
    current->next = newName;
    newName->prev = current;
    return TRUE;
}

/* 
* Adiciona, por ordem alfabetica, o nome da carreira 
* a uma paragem por onde passa
*/
nomes* sortNomesCarreira(nomes*firstN, int*numCarreiras, char*nomeCarreira){
    nomes* newName = NULL;
    int e = TRUE;
    newName = NewNameCarreiraP(nomeCarreira, newName);

    /*Caso ainda nao existam carreiras que passam na paragem*/
    if (*numCarreiras == 0)
        firstN = newName;
    else if (strcmp(firstN->nome,newName->nome)>0){   
        /*Insere o nome no início dos nomes de carreiras*/
        newName->next = firstN;
        firstN->prev = newName;
        firstN = newName;
    } else{
        /*Itera até encontrar o sítio onde o nome vai ser inserido, 
            de modo a ficar ordenado alfabeticamente*/
        e = InsertSorted (&firstN, newName);
    }
    if(e == TRUE)
        *numCarreiras = *numCarreiras + 1;
    return firstN;
}

/* Liberta a memória de uma paragem */
void free_paragens(nodeParagem**p){
    nomes* currentN = NULL;
    currentN = (*p)->paragem.carreirasNomes;

        while ((*p)->paragem.numCarreiras>0) {
            nomes* nextN = currentN->next;

            if (currentN->nome != NULL){

                free(currentN->nome);

                currentN->nome = NULL;
            }

            currentN->next = NULL;
            currentN->prev = NULL;
            free(currentN);
            currentN = NULL;
            currentN = nextN;
            (*p)->paragem.numCarreiras-=1;
        }

    free((*p)->paragem.nomeParagem);

    free(*p);
    *p = NULL;
}

/*
* Caso a carreira a remover passe numa paragem,
* remove o nome da carreira nessa paragem
*/
void removeNomeCarreiraP(nodeParagem **p, nomes** n){

    if((*p)->paragem.numCarreiras>1){
        nomes* prev = (*n)->prev;
        nomes* next = (*n)->next;

        if(prev!=NULL && next!=NULL){
            prev->next = next;
            next->prev = prev;
        }else if(prev == NULL){
            next->prev=NULL;
            (*p)->paragem.carreirasNomes = next;
        }else if(next==NULL){
            prev->next=NULL;
        }
    }
    free((*n)->nome);
    (*n)->nome = NULL;
    free(*n);
    *n = NULL;
    (*p)->paragem.numCarreiras-=1;
}

/*Remove a paragem da lista de paragens*/
void removeParagem(nodeParagem***firstP, nodeParagem***lastP, nodeParagem*p){
    nodeParagem* prev = NULL;
    nodeParagem* next = NULL;
    prev = p->prev;
    next = p->next;
    
    if(p->prev!=NULL){
        prev->next = next;
    } else{ 
        /*Caso seja a primeira paragem*/
        *(*firstP)=next;
    }
    if(p->next!=NULL){
        next->prev = prev;    
    }else{
        /*Caso seja a ultima paragem*/
        *(*lastP)=prev;
    }
    free_paragens(&p);
}
