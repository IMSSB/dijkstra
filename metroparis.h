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
//#pragma hdrstop

#define FILE_STATIONS "estacoes.txt"
#define FILE_DISTANCES "distancias.txt"
#define FILE_LINES "linhas.txt"
#define SPEED 30
#define NUM_STATIONS 14
#define TRUE 1
#define FALSE 0
#define INFINITE 99999999
#define pause printf("\nDigite algo para continuar\n"); getchar();
#define cls system("CLS || clear");
#define nl printf("\n");
#define big_line printf("\n____________________________________________________________\n");

typedef struct nodo {
   int visited; //TRUE(1) ou FALSE(0)
   int rotulo; //Rótulo do nó.
   int index;
   /*double G, F, H;*/
   struct nodo *next; //Aponta para o próximo nó
}NODO;

typedef struct {
   NODO **nodo; //	é um vetor dinâmico com os ponteiros para os primeiros elementos das listas encadeadas.
   int tam; 	//	Tamanho do vetor listaAdj
}LISTAADJ;

typedef struct{
	NODO* topo;
	int tam;
}FILA;

typedef struct {
	int index;
	int line;
}Predecessor;

void error_m(char *errormessage);
void project_presentation();
void formated_message(char *string);

//Zera o campo 'visited' de todos os nós
void zerar_visitados(LISTAADJ listaAdj);

//Retorna TRUE se existe um arco direto entre deRotulo e paraRotulo
int existeArco(LISTAADJ listaAdj,int deRotulo,int paraRotulo);

//retorna o rótulo do primeiro nó disponível a partir de "origem".
//Retorna -1 caso não encontre nenhum nó disponível.
int encontrarQualquerDestino(LISTAADJ listaAdj,int origemRotulo);

//Retorna a posição do nó cabeça de lista no vetor da lista de adjacência.
//Se não encontrar retorna -1.
int buscarIndiceNodo(LISTAADJ listaAdj,int rotulo);

//Exibe a lista de adjacência na tela
void exibir_lista_adj(LISTAADJ listaAdj);

LISTAADJ* criar_lista_adj(void);

//Insere numa lista encadeada. Recebe a referência do primeiro elemento da lista.
int inserir_lista(NODO **lista,int rotulo,int indice);

//Lê a lista de adjacência de um arquivo texto.
int ler_estacoes(char* arquivo,LISTAADJ* listaAdj);

int carregar_matriz_int(char* arquivo,int matriz[][NUM_STATIONS]);
int carregar_matriz_double(char* arquivo,double matriz[][NUM_STATIONS]);

void percurso_profundidade(LISTAADJ *listaAdj,int origem);
void percurso_largura(LISTAADJ *listaAdj,int origemRotulo, FILA *fifo);

int dijkstra(LISTAADJ *listaAdj,int origemRotulo,int destinoRotulo,
	Predecessor *precede,int distancia[][NUM_STATIONS],int linha[][NUM_STATIONS]);

int peso(LISTAADJ listaAdj,int origem,int destino,int distancia[][NUM_STATIONS]);
int exibirMenorCaminho(LISTAADJ *listaAdj,Predecessor *precede,int origem,int destino);

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

void nomeDaLinha(int linha,char *nome);

//---------------------------------------------------------------------------



#endif /* METROPARIS_H_ */
