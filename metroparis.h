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
#define TRANSHIPMENT 4 		// minutos
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
	int baldeacoes;
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

void zerar_visitados(LISTAADJ listaAdj);

int existe_arco(LISTAADJ listaAdj,int deRotulo,int paraRotulo);

//retorna o rótulo do primeiro nó disponível a partir de "origem".
//Retorna -1 caso não encontre nenhum nó disponível.
int encontrar_qualquer_destino(LISTAADJ listaAdj,int origemRotulo);

//Retorna a posição do nó cabeça de lista no vetor da lista de adjacência.
//Se não encontrar retorna -1.
int buscar_indice_nodo(LISTAADJ listaAdj,int rotulo);

//Exibe a lista de adjacência na tela
void exibir_lista_adj(LISTAADJ listaAdj);

LISTAADJ* criar_lista_adj(void);

//Insere numa lista encadeada. Recebe a referência do primeiro elemento da lista.
int inserir_lista_adj(NODO **lista,int rotulo,int indice);

int ler_estacoes(char* arquivo,LISTAADJ* listaAdj);

int carregar_matriz_int(char* arquivo,int matriz[][NUM_STATIONS]);
int carregar_matriz_double(char* arquivo,double matriz[][NUM_STATIONS]);

void percurso_profundidade(LISTAADJ *listaAdj,int origem);
void percurso_largura(LISTAADJ *listaAdj,int origemRotulo, FILA *fifo);

double dijkstra(LISTAADJ *listaAdj,int origemRotulo,int destinoRotulo,	predecessor *precede,double distancia[][NUM_STATIONS],int linha[][NUM_STATIONS]);
int astar(LISTAADJ *listaAdj,int origemRotulo,int destinoRotulo,double distancia[][NUM_STATIONS],double distancia_real[][NUM_STATIONS],int linha[][NUM_STATIONS],STAR_RESULT *result);
void exibir_star_result(STAR_RESULT *result);
void exibir_nodo(NODO *nodo);
void exibir_lista(LISTA *lista);

double peso(LISTAADJ listaAdj,int origem,int destino,double distancia[][NUM_STATIONS]);
int exibir_menor_caminho(LISTAADJ *listaAdj,predecessor *precede,int origem,int destino);

//FUNÇÕES DE FILAS
FILA* criar_fila(void);
void destruir_fila(FILA **fila);
NODO desenfileirar(FILA *fila);
void exibir_fila(FILA fila);
void exibir_fila_invertida(NODO *topo);
void copiar_fila(FILA *origem,FILA **destino);

//LIFO
void empilhar(FILA *lifo,NODO *nodo);

//FIFO
void enfileirar(FILA *fifo,NODO *nodo);

void nome_da_linha(int linha,char *nome);


int* linhas_rotulo(int linha[][NUM_STATIONS], int destinoRotulo);


//---------------------------------------------------------------------------



#endif /* METROPARIS_H_ */
