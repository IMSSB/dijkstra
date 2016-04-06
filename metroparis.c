/*
 * metroparis.c
 *
 *  Created on: 31/03/2016
 *      Author: Ruan
 */

/* UNIVASF - Universidade Federal do Vale do São Francisco
 * Disciplina: Estrutura de Dados 2
 * Professora: Ana Emíla
 * Algoritmo de Dijkstra Aplicado ao Problema do Metrô de Paris
 * Alunos: Danilo Macário
 *         Ivonaldo
 *         Renato Gabriel
 *         Rosana Guimarães
 *         Sérgio Soares
 *         Tainara Barreto
 */
#include "metroparis.h"

//#pragma argsused
int main(int argc, char* argv[])
{
	int origem=1,destino,temp,i,j;
	int distancia[NUM_STATIONS][NUM_STATIONS];
	int linha[NUM_STATIONS][NUM_STATIONS];
	int horas,minutos;
	//distancia[i][j] refere-se a distancia em linha reta entre o nó de rótulo [i+1] e o nó de rotulo[j+1]
	ListaAdj *listaAdj;
	Fila *fifo;
	Predecessor *precede;

	listaAdj=criarListaAdj();

	if(!lerEstacoes(FILE_STATIONS,listaAdj)){
		printf("\n\nErro na leitura do arquivo de estacoes!\n");
		getchar();
		return 1;
	}
	if(!carregarMatriz(FILE_DISTANCES,distancia)){
		printf("\n\nErro na leitura do arquivo de distancias!\n");
		getchar();
		return 1;
	}
	printf("\n\n");
	if(!carregarMatriz(FILE_LINES,linha)){
		printf("\n\nErro na leitura do arquivo de linhas!\n");
		getchar();
		return 1;
	}
	while(origem!=0){
		exibirListaAdj(*listaAdj);
		printf("\n\nDigite um noh de origem (ou 0 para sair): ");
		scanf("%d",&origem);
		if(!origem)
			return 1;
		printf("\n\nDigite um noh de destino(ou 0 para sair): ");
		scanf("%d",&destino);
		if(!destino)
			return 1;

		//fifo=criarFila();

		/*printf("\n\nPercurso em Largura: ");
		zerarVisitados(*listaAdj);
		percursoEmLargura(listaAdj,origem,fifo); */

		/*printf("\n\nPercurso em Profundidade: ");
		zerarVisitados(*listaAdj);
		percursoEmProfundidade(listaAdj,origem);    */

		precede=(Predecessor*)malloc((sizeof(Predecessor)*listaAdj->tam));
		temp=dijkstra(listaAdj,origem,destino,precede,distancia,linha);
		if(temp>0){
			printf("\n\nMenor distancia pelo algoritmo de Dijkstra = %d",temp);
			printf("\n\nTrajeto: ");
			exibirMenorCaminho(listaAdj,precede,origem,destino);
			minutos=(temp*60)/30;
			horas=minutos/60;
			minutos=minutos%60;
			printf("\n\nTempo estimado do trajeto: %d hora(s) e %d minuto(s)",horas,minutos);
		}else
			printf("\n\nNao existe uma rota de %d para %d\n",origem,destino);

		//destruirFila(&fifo);
		getc(stdin);
		getc(stdin);
		limparTela();
	}
	return 0;
}
//---------------------------------------------------------------------------
//retorna o primeiro nó disponível a partir de "origem"
int encontrarQualquerDestino(ListaAdj listaAdj,int origemRotulo){
	int temp;
	NODO* aux;

	if((temp=buscarIndiceNodo(listaAdj,origemRotulo))==-1)
		return -1;

	aux=listaAdj.nodo[temp]->prox; //Segundo nó da lista de adjacência

	while(aux!=NULL){
		temp=buscarIndiceNodo(listaAdj,aux->rotulo);
		if(listaAdj.nodo[temp]->visitado==FALSE)
			return listaAdj.nodo[temp]->rotulo;
		aux=aux->prox;
	}
	return -1;
}
void zerarVisitados(ListaAdj listaAdj)
{
	int i;

	for(i=0;i<listaAdj.tam;i++)
		listaAdj.nodo[i]->visitado=FALSE;
}
int existeArco(ListaAdj listaAdj,int deRotulo,int paraRotulo)
{
	int posicao;
	NODO* aux;

	if((posicao=buscarIndiceNodo(listaAdj,deRotulo))<0)
		return FALSE;
	aux=listaAdj.nodo[posicao]->prox;
	while((aux!=NULL)&&(aux->rotulo!=paraRotulo))
		aux=aux->prox;
	if(aux!=NULL)
		return TRUE;
	return FALSE;
}
int buscarIndiceNodo(ListaAdj listaAdj,int rotulo)
{
	int i;
	if(rotulo)
	for(i=0;i<listaAdj.tam;i++)
		if((listaAdj.nodo[i]!=NULL)&&((listaAdj.nodo[i]->rotulo)==rotulo))
			return i;
	return -1;
}
void exibirListaAdj(ListaAdj listaAdj){
	int i;
	NODO* aux;

	printf("\nLista de adjacencia:\n");

	for(i=0;i<listaAdj.tam;i++){
		printf("\n%d(%d) - ",(listaAdj.nodo[i])->rotulo,(listaAdj.nodo[i])->indice);
		aux=(listaAdj.nodo[i])->prox;
		while(aux!=NULL){
			printf("%d(%d)",aux->rotulo,aux->indice);
			aux=aux->prox;
			if(aux!=NULL)
				printf(", ");
		}
	}
}
ListaAdj* criarListaAdj(void){
	ListaAdj* aux;
	aux=(ListaAdj*)malloc(sizeof(ListaAdj));
	aux->nodo=NULL;
	aux->tam=0;
	return aux;
}
int inserirEmLista(NODO **lista,int rotulo,int indice){
	NODO *aux,*novoNodo;
	aux=(*lista);

	if((novoNodo=(NODO *)malloc(sizeof(NODO)))==NULL)
		return FALSE;

	novoNodo->visitado=FALSE;
	novoNodo->rotulo=rotulo;
	novoNodo->prox=NULL;
	novoNodo->indice=indice;

	if(*lista==NULL) //se a lista está vazia
		*lista=novoNodo;

	else{
		while(aux->prox!=NULL)
			aux=aux->prox;
		aux->prox=novoNodo;
	}
	return TRUE;
}
/* O arquivo deve ser da seguinte forma:
   NodoPai-filho1,filho2,filhoN...
   O arquivo deve ser encerrado com uma linha vazia!
   Exemplo:
   1-2,3,4
   2-5,6

   Não é necessário incluir nós sem filhos.
*/
int lerEstacoes(char* arquivo,ListaAdj* listaAdj) //Retorna FALSE caso não consiga ler um arquivo com uma lista de adjacências
{
   FILE *fp;
   int rotulo,indice,temp,listaAtual;
   char aux;

   if((fp=fopen(arquivo,"r"))==NULL)
	  return FALSE;
   while((getc(fp))!=EOF)
   {
	  fseek(fp,-1*sizeof(char),SEEK_CUR);
	  fscanf(fp,"%d%c",&rotulo,&aux);

		if(aux=='-'){
			listaAtual=rotulo;
			indice=buscarIndiceNodo(*listaAdj,rotulo);
			//printf("\n\nINDICE: %d\n",indice);
			if(indice==-1){
				(listaAdj->tam)++;
				listaAdj->nodo=realloc(listaAdj->nodo,(listaAdj->tam+1)*sizeof(NODO*)); //Realoca o vetor de listas
				listaAdj->nodo[listaAdj->tam-1]=NULL; //Inicializa a lista
				indice=listaAdj->tam-1;
				inserirEmLista(&(listaAdj->nodo[indice]),rotulo,indice); //Insere na lista 'i'
			}
		}
		else{
			indice=buscarIndiceNodo(*listaAdj,rotulo);
			if(indice<0){
				(listaAdj->tam)++;
				listaAdj->nodo=realloc(listaAdj->nodo,(listaAdj->tam+1)*sizeof(NODO*)); //Realoca o vetor de listas
				listaAdj->nodo[listaAdj->tam-1]=NULL; //Inicializa a lista
				inserirEmLista(&(listaAdj->nodo[listaAdj->tam-1]),rotulo,listaAdj->tam-1); //Insere na lista
				temp=buscarIndiceNodo(*listaAdj,listaAtual);
				inserirEmLista(&(listaAdj->nodo[temp]),rotulo,listaAdj->tam-1);
			}
			else{
				temp=buscarIndiceNodo(*listaAdj,listaAtual);
				inserirEmLista(&(listaAdj->nodo[temp]),rotulo,indice);
			}
		}
   }
   fclose(fp);
   return TRUE;
}
int carregarMatriz(char* arquivo,int matriz[][NUM_STATIONS]){
	FILE *fp;
	int i,j;
	char aux;

	if((fp=fopen(arquivo,"r"))==NULL)
		return FALSE;
	for(i=0;i<NUM_STATIONS;i++){
		//printf("\n");
		for(j=0;j<NUM_STATIONS;j++){
			fscanf(fp,"%d%c",&(matriz[i][j]),&aux);
			//printf("%02d  ",matriz[i][j]);
		}
	}
	fclose(fp);

	return TRUE;
}
Fila* criarFila(void){
	Fila *aux;
	aux=(Fila*)malloc(sizeof(Fila));
	aux->topo=NULL;
	aux->tam=0;
	return aux;
}
void destruirFila(Fila **fila){
	NODO *ant,*prox;

	if((*fila)->topo!=NULL){
		ant=(*fila)->topo;
		prox=ant->prox;
		while(prox!=NULL){
			free(ant);
			ant=prox;
			prox=prox->prox;
		}
		free(ant);
	}
	free(*fila);
	*fila=NULL;
}
void empilhar(Fila *lifo,NODO *nodo){
	NODO *aux;

	aux=(NODO*)malloc(sizeof(NODO));
	aux->rotulo=nodo->rotulo;
	aux->indice=nodo->indice;
	aux->visitado=nodo->visitado;
	aux->prox=lifo->topo;

	lifo->topo=aux;
	(lifo->tam)++;
}
void enfileirar(Fila *fifo,NODO *nodo){
	NODO *aux,*novo;


	novo=(NODO*)malloc(sizeof(NODO));
	novo->rotulo=nodo->rotulo;
	novo->indice=nodo->indice;
	novo->visitado=nodo->visitado;
	novo->prox=NULL;

	if((fifo->topo)==NULL) //Se a fila está vazia
		fifo->topo=novo;
	else{
		aux=fifo->topo;
		while((aux->prox)!=NULL)
			aux=aux->prox;
		aux->prox=novo;
	}
	(fifo->tam)++;
}
NODO desenfileirar(Fila *fila){
	NODO *aux,temp;

	if((fila->topo)!=NULL){
		aux=fila->topo;
		temp=*aux;

		fila->topo=fila->topo->prox;
		(fila->tam)--;
		free(aux);
		return temp;
	}
	temp.rotulo=-1;
	return temp;

}
void exibirFila(Fila fila){
	NODO* aux;

	aux=fila.topo;
	while(aux!=NULL){
		printf("%d  ",aux->rotulo);
		aux=aux->prox;
	}
}
void exibirFilaInvertida(NODO *topo){
	if(topo!=NULL){
		exibirFilaInvertida(topo->prox);
		printf("%d  ",topo->rotulo);
	}
}
void copiarFila(Fila *origem,Fila **destino){
	NODO* aux;
	Fila *lifoTemp;

	destruirFila(destino);
	*destino=criarFila();
	lifoTemp=criarFila();
	if((origem->topo)!=NULL){
		aux=origem->topo;
		while(aux!=NULL){
			empilhar(lifoTemp,aux);
			aux=aux->prox;
		}
		aux=lifoTemp->topo;
		while(aux!=NULL){
			empilhar(*destino,aux);
			aux=aux->prox;
		}
	}
	destruirFila(&lifoTemp);
}
void percursoEmProfundidade(ListaAdj *listaAdj,int origemRotulo){
	int tempRotulo,tempIndice;
	NODO* aux;

	if((tempIndice=buscarIndiceNodo(*listaAdj,origemRotulo))<0)
		return;
	printf("%d ",listaAdj->nodo[tempIndice]->rotulo);

	listaAdj->nodo[tempIndice]->visitado=TRUE;
	aux=listaAdj->nodo[tempIndice]->prox;

	while(aux!=NULL){
		if((listaAdj->nodo[aux->indice]->visitado)==FALSE)
			percursoEmProfundidade(listaAdj,aux->rotulo);
		aux=aux->prox;
	}
}
void percursoEmLargura(ListaAdj *listaAdj,int origemRotulo, Fila *fifo){
	int tempIndice;
	NODO *aux,temp;

	if((tempIndice=buscarIndiceNodo(*listaAdj,origemRotulo))<0)
		return;
	printf("%d ",listaAdj->nodo[tempIndice]->rotulo);

	aux=listaAdj->nodo[tempIndice]->prox;

	while(aux!=NULL){
		if((listaAdj->nodo[aux->indice]->visitado)==FALSE){
			enfileirar(fifo,aux);
			listaAdj->nodo[aux->indice]->visitado=TRUE;
		}
		aux=aux->prox;
	}
	temp=desenfileirar(fifo);
	if(temp.rotulo!=-1){
			percursoEmLargura(listaAdj,temp.rotulo,fifo);
	}
}
int dijkstra(ListaAdj *listaAdj,int origemRotulo,int destinoRotulo,
	Predecessor *precede,int distancia[][NUM_STATIONS],int linha[][NUM_STATIONS])
	{
	int *dist,*perm;
	int i,j,indiceOrigem,indiceDestino,corrente,menorDist,novaDist,dc,cont=0;
	int linhaPrecede[NUM_STATIONS],novaLinha;
	indiceOrigem=buscarIndiceNodo(*listaAdj,origemRotulo);
	indiceDestino=buscarIndiceNodo(*listaAdj,destinoRotulo);

	dist=NULL;
	perm=NULL;
	dist=(int*)malloc(sizeof(int)*(listaAdj->tam));
	perm=(int*)malloc(sizeof(int)*(listaAdj->tam));

	for(i=0;i<listaAdj->tam;i++){
		dist[i]=INFINITE;
		perm[i]=FALSE;
		precede[i].indice=-1;
		precede[i].linha=-1;
	}
	perm[indiceOrigem]=TRUE;
	dist[indiceOrigem]=0;
	corrente=indiceOrigem;
	while(corrente!=indiceDestino){
		//printf("\nCORRENTE: %d\n",listaAdj->nodo[corrente]->rotulo);

		cont++;
		menorDist=INFINITE;
		dc = dist[corrente];
		for(i=0;i<listaAdj->tam;i++){
			if(perm[i]==FALSE){
				novaDist=dc+peso(*listaAdj,corrente,i,distancia);
				novaLinha=linha[listaAdj->nodo[corrente]->rotulo-1][listaAdj->nodo[i]->rotulo-1];
				//printf("\nNOVA LINHA: %d\n",novaLinha);
				if(novaDist<dist[i]){
					if((precede[corrente].linha!=-1)&&(precede[corrente].linha!=novaLinha))
						novaDist+=2;
					if(novaDist<dist[i]){
						dist[i]=novaDist;
						precede[i].indice=corrente;
						precede[i].linha=novaLinha;
					}
				}
				if(dist[i]<menorDist){
					menorDist=dist[i];
					j=i;
				}
			}
		}
		corrente=j;
		perm[corrente]=TRUE;
		if(cont>listaAdj->tam)
			return -1;
	 }
	 j=dist[indiceDestino];
	free(dist);
	free(perm);
	return j;
}

int peso(ListaAdj listaAdj,int origem,int destino,int distancia[][NUM_STATIONS]){
	NODO* aux;
	int rotuloOrigem,rotuloDestino;
	if(origem!=destino){

		aux=listaAdj.nodo[origem]->prox;
		while((aux!=NULL)&&((aux->indice)!=destino))
			aux=aux->prox;
		if(aux==NULL)
			return INFINITE;
		rotuloOrigem=listaAdj.nodo[origem]->rotulo;
		rotuloDestino=listaAdj.nodo[destino]->rotulo;
		return distancia[rotuloOrigem-1][rotuloDestino-1];
	}
	return 0;
}
int exibirMenorCaminho(ListaAdj *listaAdj,Predecessor *precede,int origem,int destino){
	int aux,origemIndice,destinoIndice;
	Fila* lifo; //Pilha para auxiliar na exibição do caminho
	NODO *temp;
	char nomeLinha[20];
	lifo=criarFila();

	origemIndice=buscarIndiceNodo(*listaAdj,origem);
	destinoIndice=buscarIndiceNodo(*listaAdj,destino);

		empilhar(lifo,listaAdj->nodo[destinoIndice]);
		aux=precede[destinoIndice].indice;
		while(aux!=origemIndice){
			empilhar(lifo,listaAdj->nodo[aux]);
			aux=precede[aux].indice;
		}
		//aux=precede[origemIndice].indice;
		empilhar(lifo,listaAdj->nodo[origemIndice]);
		//exibirFila(*lifo);
		temp=lifo->topo;
		printf("\n\nPartindo da estacao: E%d, ",temp->rotulo);
		temp=temp->prox;
		nomeDaLinha(precede[temp->indice].linha,nomeLinha);
		printf("pegue a linha %s",nomeLinha);
		printf("\nVoce passara pela(s) estacao(oes): ");
		while(temp!=NULL){
			printf("E%d  ",temp->rotulo);
			if((temp->prox!=NULL)&&(precede[temp->indice].linha!=precede[temp->prox->indice].linha)){
				nomeDaLinha(precede[temp->prox->indice].linha,nomeLinha);
				printf("\nNa estacao E%d, troque para a linha %s passando pela(s) estacao(oes): ",temp->rotulo,nomeLinha);
			}
			temp=temp->prox;
		}
		destruirFila(&lifo);
		return 1;
}
void limparTela(){
	int i;

	for(i=0;i<50;i++)
    	printf("\n\n\n\n\n\n\n\n\n\n");
}

void nomeDaLinha(int linha,char *nome){
	switch(linha){
		case 1:
			strcpy(nome,"Amarela");
			break;
		case 2:
			strcpy(nome,"Azul");
			break;
		case 3:
			strcpy(nome,"Verde");
			break;
		case 4:
			strcpy(nome,"Vermelha");
			break;
	}
}
