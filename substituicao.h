//Julio Hebert Silva
//Tomaz Miranda de Oliveira
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define tam 250  // metade+1 do teto de ( min + max )/2 do tam de pag    /   ( min + max )/2 tam da mem

struct dados{
    int addr;   // int addr     addr shiftado
    int referenciada ; // bit R
    int alterada ;  // bit M
    int acesso ; // ultimo acesso , se -1 nunca acessada
    struct dados *prox;
};

typedef struct dados Dados;
typedef Dados* Hash[tam]; 

typedef struct celula{		
	int addr;
	struct celula *prox;
    struct celula *ant;
}Celula;

typedef struct lista_se{
	Celula *prim;
	Celula *ultimo;
	int tamanho;
}Lista_se;	

unsigned Valor_S(int tam_page); //Deviva o valor de S segundo especificação

// Algoritmos de substituição

//LRU retorna a pagina que ficou sem ser usava a mais tempo, nesse caso a ultima posicao da lista
int LRU(Lista_se *lista); // retorna addr que deeve ser substituido

//NRU escolhe a pagina de menor classe no Hash de paginas
int NRU(Hash tab); // retorna addr que deeve ser substituido
//Seta os valores do bit R para 0 ao passar do tempo
void zerarR( Hash tab, int tempo);

//Segunda chance Percorre a lista a partir da ultima pagina referenciada até achar alguma pagina com bit R = 0
int segunda_Chance(Hash tab, int *lista, int **index, int qnt_page); // retorna addr que deeve ser substituido

//Funcções referentes a struc Lista_se

void inicializaLista(Lista_se *lista);   // inicializa a lista passada

int insere(Lista_se *lista, int addr);  // insere na lista addr passado na primeira posicao, empurrando a lista para tras , retorna 1 se sucesso

int remove_celula(Lista_se *lista, int addr);	//remove addr determinado , procura a partir do começo, pois pagina requerida tende a estar entre as recentes,  retorna 1 

//Funcoes que lidaram com os parametros de entrada e saida

//Imprime na tela do terminal a saida padrao
void saida(char *alg, char *nome_file, int tam_mem, int tam_page, int lidas_page, int suja_page, int tempo);

//Organiza os parametros recebidos pelo argv e ja faz suas respectivas verificacoes de valores
int valor_entrada(int argc, char **argv, char* alg, char* nome_file, int *tam_page, int *tam_mem); // tratamento de paranetros

//Funcoes relacionadas ao Hash

void inicializaHash(Hash tab); //Inicializa os valores iniciais da Hash

int funcaoHash(int addr); //Chave = Valor mod tam (da hash)

void removeHash(Hash tab, int addr, int **suja_page); // Remove da Hash o item passado pelo addr e caso pagina tenha sido modificada adiciona 1 no valor das Paginas Sujas

//Nessa funcao é onde basicamente tudo acontece, vai inserindo  na Hash as paginas, modificando seus valores R, M, ultimo acesso e,
//caso haja falta de pagina adiciona 1 ao valor de paginas lidas e chama o Algoritimo de substituicao caso necessario 
void insereHash(Hash tab, char *alg, int addr, int tempo, char *rw, int *lidas_page, int qnt_page, int *suja_page, int *list, int *index, Lista_se *lista);
