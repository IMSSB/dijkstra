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

typedef struct nd{
   int visitado; //TRUE(1) ou FALSE(0)
   int rotulo; //Rótulo do nó.
   int indice;
   struct nd *prox; //Aponta para o próximo nó
}NODO;

typedef struct{
   NODO **nodo; //é um vetor dinâmico com os ponteiros para os primeiros elementos das listas encadeadas.
   int tam; //Tamanho do vetor listaAdj
}ListaAdj;

typedef struct{
	NODO* topo;
	int tam;
}Fila;

typedef struct {
	int indice;
	int linha;
}Predecessor;

//Zera o campo 'visitado' de todos os nós
void zerarVisitados(ListaAdj listaAdj);

//Retorna TRUE se existe um arco direto entre deRotulo e paraRotulo
int existeArco(ListaAdj listaAdj,int deRotulo,int paraRotulo);

//retorna o rótulo do primeiro nó disponível a partir de "origem".
//Retorna -1 caso não encontre nenhum nó disponível.
int encontrarQualquerDestino(ListaAdj listaAdj,int origemRotulo);

//Retorna a posição do nó cabeça de lista no vetor da lista de adjacência.
//Se não encontrar retorna -1.
int buscarIndiceNodo(ListaAdj listaAdj,int rotulo);

//Exibe a lista de adjacência na tela
void exibirListaAdj(ListaAdj listaAdj);

ListaAdj* criarListaAdj(void);

//Insere numa lista encadeada. Recebe a referência do primeiro elemento da lista.
int inserirEmLista(NODO **lista,int rotulo,int indice);

//Lê a lista de adjacência de um arquivo texto.
int lerEstacoes(char* arquivo,ListaAdj* listaAdj);

int carregarMatriz(char* arquivo,int matriz[][NUM_STATIONS]);

void percursoEmProfundidade(ListaAdj *listaAdj,int origem);
void percursoEmLargura(ListaAdj *listaAdj,int origemRotulo, Fila *fifo);

int dijkstra(ListaAdj *listaAdj,int origemRotulo,int destinoRotulo,
	Predecessor *precede,int distancia[][NUM_STATIONS],int linha[][NUM_STATIONS]);

int peso(ListaAdj listaAdj,int origem,int destino,int distancia[][NUM_STATIONS]);
int exibirMenorCaminho(ListaAdj *listaAdj,Predecessor *precede,int origem,int destino);

//operações sobre Fila do tipo FIFO
void enfileirar(Fila *fifo,NODO *nodo);

//Operações sobre uma Fila do tipo LIFO
void empilhar(Fila *lifo,NODO *nodo);

void limparTela(void);

//Operações sobre uma fila qualquer
Fila* criarFila(void);
void destruirFila(Fila **Fila);
NODO desenfileirar(Fila *fila);
void exibirFila(Fila fila);
void exibirFilaInvertida(NODO *topo);
void copiarFila(Fila *origem,Fila **destino);

void nomeDaLinha(int linha,char *nome);

//---------------------------------------------------------------------------



#endif /* METROPARIS_H_ */
