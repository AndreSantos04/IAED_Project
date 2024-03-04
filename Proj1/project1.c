/* iaed-23 - ist1107041 - project1 */
/*
 * File:  project1.c
 * Author:  André Antunes Santos
 * Description: Criacao de um sistema de carreiras de transporte publico
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*Número máximo de Paragens*/
#define MAXPARAGENS 10000

/*Tamanho máximo do nome de uma paragem*/
#define MAXNAMEP 50

/*Número máximo de Carreiras*/
#define MAXCARREIRAS 200

/*Tamanho máximo do nome de uma carreira*/
#define MAXNAMEC 20

/*Número máximo de Ligacoes*/
#define MAXLIGACOES 30000

#define INVERSO "inverso"
#define LENGTH_MAX_INVERSO 7
#define LENGTH_MIN_INVERSO 3

#define PRIMEIRAPARAGEM 0
#define DENTRO 1
#define FORA 0
#define MAXCOORD 16
#define TRUE 1
#define FALSE 0

#define NO_SUCH_STOP_ERR "%s: no such stop.\n"



/*
* Estrutura que define uma paragem
* Caracterizada por: 
* Nome da paragem;
* Coordenadas (latitude e longitude);
* Número de carreiras que passam pela paragem;
* Índice no vetor carreiras das carreiras que passam pela paragem;
*/
typedef struct paragens{
    char nomeParagem [MAXNAMEP];
    double latitude;
    double longitude;
    int numCarreiras;
    int carreirasIndex [MAXCARREIRAS];
}Paragem;

/*
* Estrutura que define uma Carreira
* Caracterizada por: 
* Nome da carreira;
* Primeira e última paragens (paragemOrigem e paragemDestino);
* Nomes das paragens por onde a carreira passa;
* Número de paragens por onde a carreira passa;
* Custo e duracao total da viagem na carreira;
*/
typedef struct carreiras{
    char nomeCarreira [MAXNAMEC];
    char paragemOrigem [MAXNAMEP];
    char paragemDestino [MAXNAMEP];
    char nomesParagensC [MAXPARAGENS] [MAXNAMEP];
    int numParagens;
    float custoTotal;
    float duracaoTotal;
}Carreira;

/*Funcoes auxiliares das Paragens*/
void handle_p_func();
int existsP(char [MAXNAMEP]);
void scan_p_name(char [MAXNAMEP]);
void addNewP(char [MAXNAMEP]);
double handle_coords();

/*Funcoes auxiliares das Carreiras*/
void handle_c_func();
int existsC(char [MAXNAMEC]);
void scan_c_name(char [MAXNAMEC]);
void createNewC(char[MAXNAMEC]);
void eh_inverso(int );
void print_carreiras();

/*Funcoes auxiliares das Ligacoes*/
int handle_l_func();
int checkParagem(char [MAXNAMEP]);
float handle_Cost_Duration();
void add_ligacao(int , int , int , char[MAXNAMEP], char [MAXNAMEP]);
void addParagemDestino(int, char [MAXNAMEP]);
void changeParagemOrigem(int, char [MAXNAMEP], int);


/*Funcoes auxiliares das Intersecoes*/
void handle_i_func();
void sortCarreiras (int [MAXCARREIRAS], int);

/*Vetor das paragens*/
Paragem paragem[MAXPARAGENS];

/*Vetor das carreiras*/
Carreira carreira[MAXCARREIRAS];

/*Contam o total de paragens, carreiras e ligacoes adicionadas, respetivamente*/
int TotalParagens = 0;
int TotalCarreiras = 0;
int TotalLigacoes = 0;

/*Variável que irá ler o stdin*/
int c;

/*
* Funcao Main
* Lê a primeira letra de cada linha de comando
* Iniciando a funcao respetiva à letra
* Termina quando recebe a letra 'q'
*/
int main(){
    while ((c=getchar())!=EOF){
        switch (c){
        case 'p':
            handle_p_func(); 
            break;
        case 'l':
            TotalLigacoes+=handle_l_func();
            break;
        case 'c':
            handle_c_func();
            break;
        case 'i':
            handle_i_func();
            break;
        case 'q':
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
void handle_p_func(){
    int i,iP;
    char nameP[MAXNAMEP];

    /*Se a linha de comando tiver mais argumentos*/
    if ((c=getchar())!='\n'){    
        iP=existsP(nameP);
        if(iP==-1){
            if (c=='\n'){
                /*Caso não existe a paragem que se pretende imprimir*/
                printf(NO_SUCH_STOP_ERR, nameP);
            }
            else{
                /*Caso sejam fornecidas coordenadas*/
                addNewP(nameP);
            }
        }
        else{
            if (c=='\n'){
                printf("%16.12f %16.12f\n", paragem[iP].latitude,
                    paragem[iP].longitude);
            }
            else{
                /*Caso sejam fornecidas coordenadas duma paragem já definida*/
                printf("%s: stop already exists.\n",nameP);
            }
        }
    }
    /*Se não tiver argumentos (apenas p)*/
    else{
        for(i=0;i<TotalParagens;i++){
            printf("%s: %16.12f %16.12f %d\n", paragem[i].nomeParagem, 
                paragem[i].latitude, paragem[i].longitude, 
                paragem[i].numCarreiras);
        }
    }
}

/*
*Retorna o indice no vetor de paragens caso exista uma paragem com o mesmo nome
*Caso contrario retorna -1
*/
int existsP(char nameP[MAXNAMEP]){
    int i;
    scan_p_name(nameP);
    for(i=0;i<TotalParagens;i++){
        if(strcmp(nameP,paragem[i].nomeParagem)==0){
            return i;
        }
    }
    return -1;
}

/*Lê o input correspondente ao nome da paragem e retorna-o*/
void scan_p_name(char name[MAXNAMEP]){
    int aspas = FORA,i=0;

    while(i<MAXNAMEP){
        c = getchar();
        /*Fecha as aspas e acaba a leitura do nome*/
        if (c == '\"' && aspas ==DENTRO){
            aspas=FORA;
            c = getchar();
            break;
        }
        /*Se não tem aspas e há um espaço acaba a leitura do nome*/
        else if(isspace(c) && aspas == FORA){ 
            break;
        }
        /*Abre aspas, não as adicionando ao nome*/
        else if ((c == '\"' && aspas ==FORA)){
            aspas = DENTRO;
        }
        else{
            name[i]=c;
            i++;            
        }
    }
    name[i]='\0';
    if (i==MAXNAMEP){
        while(!isspace(c)){
            c=getchar();
        }
    }
}

/* Cria uma nova paragem com o nome e as coordenadas fornecidas */
void addNewP(char name[MAXNAMEP]){
    strcpy(paragem[TotalParagens].nomeParagem,name);
    paragem[TotalParagens].latitude=handle_coords();
    paragem[TotalParagens].longitude=handle_coords();
    TotalParagens++;
}

/* Lê o input correspondente a uma das coordenadas e retorna-o */
double handle_coords(){
    int i;
    char coord[MAXCOORD];
    double coordinate;

    /* Salta os espaços vazios até chegar a um caracter da coordenada */
    while(c==' '){
        c=getchar();
    }

    for(i=0;!isspace(c);i++){
        coord[i]=c;
        c=getchar();
    }
    coord[i]='\0';
    /* Transforma a string lida num float */
    coordinate=atof(coord);
    return coordinate;
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
void handle_c_func(){
    int i,iC;
    char nameC[MAXNAMEC];
    /*Caso sejam fornecidos argumentos*/
    if ((c=getchar())!='\n'){    
        iC = existsC(nameC);
        /*Cria uma nova carreira*/
        if(iC==-1){
            createNewC(nameC);
        }
        else{
            /*Imprime por ordem de criacao*/
            if (c=='\n'){
                if (carreira[iC].numParagens>0){
                    for(i=0;i<carreira[iC].numParagens-1;i++){
                        printf("%s, ",carreira[iC].nomesParagensC[i]);
                    }
                    printf("%s\n",carreira[iC].nomesParagensC[i]);
                }
            }
            /*Imprime inverso*/
            else{
               eh_inverso(iC);
            }
        }
    }
    /*Caso não sejam fornecidos argumentos*/
    else{
        print_carreiras();
    }
}

/*
*Retorna o indice no vetor de carreiras caso exista uma carreira com nome igual
*Caso contrario retorna -1
*/
int existsC(char nameC[MAXNAMEC]){
    int i;
    scan_c_name(nameC);
    for(i=0;i<TotalCarreiras;i++){
        if(!strcmp(nameC,carreira[i].nomeCarreira)){
            return i;
        }
    }
    return -1;
}

/* Lê o input correspondente ao nome da carreira e retorna-o */
void scan_c_name(char name[MAXNAMEC]){
    int i;

    for (i=0;i<MAXNAMEC;i++){
        c=getchar();
        if (isspace(c)){
            break;
        }
        name[i]=c;
    }
    name[i]='\0';
}

/* Cria uma nova carreira com o nome fornecido e os restantes valores a 0 */
void createNewC(char name[MAXNAMEC]){
    strcpy(carreira[TotalCarreiras].nomeCarreira,name);
    carreira[TotalCarreiras].custoTotal = 0;
    carreira[TotalCarreiras].numParagens = 0;
    carreira[TotalCarreiras].duracaoTotal = 0;
    TotalCarreiras++;
}

/*
* Verifica se o argumento opcional fornecido é a string "inverso"
* Ou alguma abreviatura com 3 ou mais caracteres
*/
void eh_inverso(int iC){
    int i=0, inv = 0;
    char inverso [LENGTH_MAX_INVERSO];
    while((c=getchar())!='\n'){
        inverso[i]=c;
        i++;
    }
    inverso[i]='\0';

    if (i>=LENGTH_MIN_INVERSO && i<=LENGTH_MAX_INVERSO){
        if(!strncmp(inverso,INVERSO,strlen(inverso))){
            inv = 1;
        }
    }
    if (inv == 1){
        if (carreira[iC].numParagens>0){
            for(i=carreira[iC].numParagens-1;i>0;i--){
                printf("%s, ",carreira[iC].nomesParagensC[i]);
            }
                printf("%s\n",carreira[iC].nomesParagensC [PRIMEIRAPARAGEM]);
            }
    }
    else{
        printf("incorrect sort option.\n");
    } 
}

/*
* Imprime as características de todas carreiras
*/
void print_carreiras(){
    int i;
    for (i=0;i<TotalCarreiras;i++){
        if (carreira[i].numParagens==0){
            printf("%s %d %.2f %.2f\n", carreira[i].nomeCarreira,
                carreira[i].numParagens, carreira[i].custoTotal,
                carreira[i].duracaoTotal);
        }
        else{
            printf("%s %s %s %d %.2f %.2f\n", carreira[i].nomeCarreira,
                carreira[i].paragemOrigem,carreira[i].paragemDestino, 
                carreira[i].numParagens, carreira[i].custoTotal,
                carreira[i].duracaoTotal);
        }
    }
}

/* 
* Funcao para o comando da letra 'l' (Ligacoes)
* Recebe um nome de uma carreira, duas paragens (origem e destino), 
* um custo e uma duracao 
*
* Para ser válida a paragem de origem da ligacao deve corresponder a paragem 
* destino da carreira (CASO 1), a paragem destino de ligacao corresponde a 
* origem da carreira (CASO 2), paragem destino de ligacao corresponde a 
* origem da carreira e a paragem origem da ligacao à paragem destino da 
* carreira, tornando-a circular (CASO 3) 
*/
int handle_l_func(){
    int nC,po,pd;   
    char nomeCarreira[MAXNAMEC], nomePOrigem[MAXNAMEP], nomePDestino[MAXNAMEP];
    float custo,duracao;

    c = getchar();

    nC=existsC(nomeCarreira);
    if(nC==-1){
        printf("%s: no such line.\n",nomeCarreira);
        return 0;
    }

    po=checkParagem(nomePOrigem);
    
    pd = checkParagem(nomePDestino);
    if(po==-1||pd ==-1){
        return 0;
    }

    /*
    * Se a origem da ligacao for diferente do fim da carreira 
    * E o destino da ligacao diferente do inicio da carreira 
    * E a carreira já  tiver paragens adicionadas na origem e no destino
    */
    if ((strcmp(carreira[nC].paragemDestino,nomePOrigem) &&
    strcmp(carreira[nC].paragemOrigem,nomePDestino))){  
        if (carreira[nC].numParagens>0){
            printf("link cannot be associated with bus line.\n");
            return 0;
        }
    }

    custo=handle_Cost_Duration();
    duracao=handle_Cost_Duration();
    if (custo<0 || duracao<0){
        printf("negative cost or duration.\n");
        return 0;
    }

    add_ligacao(nC,po, pd, nomePOrigem, nomePDestino);

    carreira[nC].custoTotal+=custo;
    carreira[nC].duracaoTotal+=duracao;

    return 1;
}

/* Analisa o nome da paragem par ver se existe uma igual, se nâo existir 
*  imprime um erro e retorna -1
*/
int checkParagem(char nomeP[MAXNAMEP]){
    int p;
    
    /*
    * Serve para não imprimir de novo o erro caso já tenha sido impresso uma 
    * vez para essa ligacao
    */
    static int err = FALSE;
    p = existsP(nomeP); /*Retorna -1 ou o indice da paragem*/
    if (p==-1 && err == FALSE){
        printf(NO_SUCH_STOP_ERR,nomeP);
        err = TRUE;
        return p;
    }
    err = FALSE;
    return p;
}

/* Lê o input correspondente ao custo ou duracao de uma ligacao e retorna-o */
float handle_Cost_Duration(){
    int i, neg=0;
    char s[MAXCOORD];
    float cd;
    while(c==' '){
        c=getchar();
    }
    if (c=='-'){
        neg = 1;
        c = getchar();
    }
    for(i=0;!isspace(c);i++){
        s[i]=c;
        c=getchar();
    }
    s[i]='\0';

    /*Transforma a string lida num float*/
    cd=atof(s);

    /*Torna o numero num numero negativo se tiver recebido um caracter '-' */
    if (neg==1){
        cd*=-1;
    }
    return cd;
}

/* Adiciona a ligacao a carreira */
void add_ligacao(int nC, int po, int pd, char nomePOrigem[MAXNAMEP], 
char nomePDestino [MAXNAMEP]){

    /* Caso seja a primeira ligacao da paragem */
    if (carreira[nC].numParagens==0){
        
        changeParagemOrigem(nC, nomePOrigem, po);
        addParagemDestino(nC,nomePDestino);
        paragem[pd].carreirasIndex[paragem[pd].numCarreiras]=nC;
        paragem[pd].numCarreiras++;

    }
    /* Criacao de uma carreira circular */
    else if(!strcmp(carreira[nC].paragemDestino,nomePOrigem) && 
            !strcmp(carreira[nC].paragemOrigem,nomePDestino)){
        
        addParagemDestino(nC,nomePDestino);
    }
    /* Extensao da carreira no final da mesma */
    else if (!strcmp(carreira[nC].paragemDestino,nomePOrigem)){

        addParagemDestino(nC,nomePDestino);
        paragem[pd].carreirasIndex[paragem[pd].numCarreiras]=nC;
        paragem[pd].numCarreiras++;

    }
    /* Extensao da carreira no inicio da mesma */
    else if (!strcmp(carreira[nC].paragemOrigem,nomePDestino)){   
            
        changeParagemOrigem(nC,nomePOrigem, po);
    }
}

/* 
* Altera a paragem destino e adiciona o nome da paragem ao fim do vetor 
* nomesParagensC da carreira
*/
void addParagemDestino(int nC, char nomeP[MAXNAMEP]){
    strcpy(carreira[nC].paragemDestino,nomeP);
    strcpy(carreira[nC].nomesParagensC[carreira[nC].numParagens],nomeP);
    carreira[nC].numParagens++;       
}

/*
* Altera a paragem origem e adiciona o nome da paragem ao inicio do vetor
* nomesParagensC da carreira
*/
void changeParagemOrigem(int nC, char nomeP[MAXNAMEP],int po){
    int i,j,numParagens;

    strcpy(carreira[nC].paragemOrigem,nomeP);
    carreira[nC].numParagens++;
    paragem[po].carreirasIndex[paragem[po].numCarreiras]=nC;
    paragem[po].numCarreiras++;
    numParagens= carreira[nC].numParagens;

    for(i=numParagens-1;i>0;i--){
        j=i-1;
        strcpy(carreira[nC].nomesParagensC[i],carreira[nC].nomesParagensC[j]);
    }
    strcpy(carreira[nC].nomesParagensC[PRIMEIRAPARAGEM],nomeP);
}

/* 
* Funcao para o comando da letra 'i' (Intersecoes)
* Caso numa paragem pare mais de uma carreira, imprime todas as carreiras que 
* aí parem por ordem alfabética
*/
void handle_i_func(){
    int i, j, numCarreiras, indCarreira;

    for (i=0;i<TotalParagens;i++){
        numCarreiras=paragem[i].numCarreiras;
        if (numCarreiras>1){
            /*Ordena as carreiras que param na paragem i*/
            sortCarreiras(paragem[i].carreirasIndex, numCarreiras);

            printf("%s %d: ", paragem[i].nomeParagem, numCarreiras);

            for (j=0;j<numCarreiras-1;j++){
                indCarreira = paragem[i].carreirasIndex[j];
                printf("%s ",carreira[indCarreira].nomeCarreira);
            }
            indCarreira = paragem[i].carreirasIndex[j];
            printf("%s\n",carreira[indCarreira].nomeCarreira);
        }
    }
}

/*
* Algoritmo de Insertion Sort que ordena os indices das carreiras que param 
* numa paragem, por ordem alfabética dos nomes das respetivas carreiras
*/
void sortCarreiras (int iCarreiras[MAXCARREIRAS], int numCarreiras){
    int i,j;
    int aux;

    for(i=1;i<numCarreiras;i++){
        aux = iCarreiras[i];
        j=i-1;

        while(j>=0 && (strcmp(carreira[aux].nomeCarreira,
        carreira[iCarreiras[j]].nomeCarreira)<0)){

            iCarreiras[j+1] = iCarreiras[j];
            j--;
        }
        iCarreiras [j+1] = aux; 
    }
}

