/*
 * metroparis.h
 *
 *  Created on: 31/03/2016
 *      Author: Ruan
 */

#ifndef METROPARIS_H_
#define METROPARIS_H_
//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_STATIONS "estacoes.txt"
#define FILE_DISTANCES "distancias.txt"
#define FILE_REAL_DISTANCES "distancias_reais.txt"
#define FILE_LINES "linhas.txt"
#define SPEED 30			// km/hora
#define SPEEDM (SPEED/60.0)   //km/min
#define NUM_STATIONS 14		// MAX_NODOS
#define TRANSHIPMENT 120 		// minutos
#define TRUE 1
#define FALSE 0
#define INFINITE 9999999999
#define pause printf("\nDigite algo para continuar\n"); getchar();
#define cls system("CLS || clear");
#define nl printf("\n");
#define big_line printf("\n____________________________________________________________\n");

/*typedef struct nodo
{
   int visited;
   int rotulo;
   int index;
   double f,g,h;
   struct nodo *next;
}NODO;*/;

typedef struct nodo
{
   int rotulo;
   int index;
   double f,g,h;
   struct nodo *next;
   struct nodo *origin;
   int visited;
}NODO;

typedef struct
{
   NODO **nodo; //	é um vetor dinâmico com os ponteiros para os primeiros elementos das listas encadeadas.
   int tam; 	//	Tamanho do vetor listaAdj
}LISTAADJ;

typedef struct
{
	NODO *primeiro;
	int tam;
}LISTA;

typedef struct
{
	NODO *topo;
	int tam;
}FILA;

typedef struct
{
	NODO caminho[NUM_STATIONS];
	double distancia;
	double tempo;
	int tam;
}STAR_RESULT;

typedef struct
{
	int index;
	int line;
}predecessor;

int metro_paris();

void error_m(char *errormessage);
void project_presentation();
void formated_message(char *string);

//Retorna a posição do nó cabeça de lista no vetor da lista de adjacência.
//Se não encontrar retorna -1.
int buscar_indice_nodo(LISTAADJ listaAdj,int rotulo);

//Exibe a lista de adjacência na tela
void exibir_lista_adj(LISTAADJ listaAdj);
void exibir_star_result(STAR_RESULT *result);
void exibir_nodo(NODO *nodo);
void exibir_lista(LISTA *lista);
int exibir_trajeto_mais_rapido(STAR_RESULT *resultado,int linha[][NUM_STATIONS]);

LISTAADJ* criar_lista_adj(void);
int inserir_lista_adj(NODO **lista,int rotulo,int indice);

LISTA* criar_lista();
void retirar_lista(LISTA *lista, int pos);
void destruir_lista(LISTA *lista);
int inserir_lista(LISTA *lista,int rotulo,int index,NODO *origin,double f, double g, double h);

int ler_estacoes(char* arquivo,LISTAADJ* listaAdj);

int carregar_matriz_int(char* arquivo,int matriz[][NUM_STATIONS]);
int carregar_matriz_double(char* arquivo,double matriz[][NUM_STATIONS]);

double dijkstra(LISTAADJ *listaAdj,int origemRotulo,int destinoRotulo,	predecessor *precede,double distancia[][NUM_STATIONS],int linha[][NUM_STATIONS]);
int astar(LISTAADJ *listaAdj,int origemRotulo,int destinoRotulo,double distancia[][NUM_STATIONS],double distancia_real[][NUM_STATIONS],int linha[][NUM_STATIONS],STAR_RESULT *result);
int esta_na_lista(LISTA *lista,NODO *nodo);
NODO *encontrar_menor_valor_f(LISTA *lista);
void trocar_nodo_de_lista(NODO *nodo, LISTA *de, LISTA *para);

double peso(LISTAADJ listaAdj,int origem,int destino,double distancia[][NUM_STATIONS]);
int exibir_menor_caminho(LISTAADJ *listaAdj,predecessor *precede,int origem,int destino);

//FUNÇÕES DE FILAS
FILA* criar_fila(void);
void destruir_fila(FILA **fila);
NODO desenfileirar(FILA *fila);
void copiar_fila(FILA *origem,FILA **destino);
void exibir_fila(FILA fila);
void exibir_fila_invertida(NODO *topo);

//LIFO
void empilhar(FILA *lifo,NODO *nodo);

//FIFO
void enfileirar(FILA *fifo,NODO *nodo);

void nome_da_linha(int linha,char *nome);


int* linhas_rotulo(int linha[][NUM_STATIONS], int destinoRotulo);


//---------------------------------------------------------------------------



#endif /* METROPARIS_H_ */
