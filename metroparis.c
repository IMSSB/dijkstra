/*
 * metroparis.c
 *
 *  Created on: 31/03/2016
 *      Author: Ruan
 */
#include "metroparis.h"

int main()
{
	int origem=1,destino,temp;
	int distancia[NUM_STATIONS][NUM_STATIONS],linha[NUM_STATIONS][NUM_STATIONS];
	double distancia2[NUM_STATIONS][NUM_STATIONS];
	int horas,minutos;
	//distancia[i][j] refere-se a distancia em linha reta entre o nó de rótulo [i+1] e o nó de rotulo[j+1]
	LISTAADJ *listaAdj;
	Predecessor *precede;

	listaAdj=criar_lista_adj();

	if(!ler_estacoes(FILE_STATIONS,listaAdj))
		error_m("Erro ao ler arquivo de estacões.");
	if(!carregar_matriz_int(FILE_DISTANCES,distancia))
		error_m("Erro ao ler arquivo de distancias.");
	if(!carregar_matriz_double("distancias2.txt",distancia2))
		error_m("Erro ao ler arquivo de distancias.");
	pause;
	if(!carregar_matriz_int(FILE_LINES,linha))
		error_m("Erro ao ler arquivo de linhas.");

	while(origem!=0)
	{
		//exibirLISTAADJ(*listaAdj);
		printf("\n\nDigite a estação de origem: E");
		scanf("%d",&origem);
		if(!origem)
			return 1;
		printf("\n\nDigite a estação de destino: E");
		scanf("%d",&destino);
		if(!destino)
			return 1;

		if (!(precede=(Predecessor*)malloc((sizeof(Predecessor)*listaAdj->tam))))
				error_m("Erro ao alocar memória.");

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
			printf("\n\nNão existe uma rota de %d para %d\n",origem,destino);

		getc(stdin);
		getc(stdin);
		cls;
	}
	return 0;
}
//---------------------------------------------------------------------------
//retorna o primeiro nó disponível a partir de "origem"
void error_m(char *errormessage)
{ 	/* Fun��o para facilitar exibi��o de mensagens de erro */
    printf("\n%s",errormessage);
    pause;
    exit(1);
}
void project_presentation()
{
	big_line;
	formated_message("E-Server v.2.0");
	formated_message("Trabalho da Disciplina Algoritmos e Estrutura de Dados II");
	formated_message("Professora: Ana Emilia de Melo Queiroz");
	formated_message("Alunos:     Ricardo Valério Teixeira de Medeiros Silva");
	formated_message("            Ruan de Medeiros Bahia");
	formated_message("Simulador de um servidor de e-mails, sendo salvos em arquivos,");
	formated_message("com uso de Árvores B para pesquisas nos dados");
	big_line;

	pause;
	cls;

	return;
}
void formated_message(char *string){
	printf("%s %-66s %s\n",
				"\\\\\\\\\t",
				string,
				"\t\\\\\\\\");
}

int encontrarQualquerDestino(LISTAADJ listaAdj,int origemRotulo)
{
	int temp;
	NODO *aux;

	if((temp=buscarIndiceNodo(listaAdj,origemRotulo))==-1)
		return -1;

	aux=listaAdj.nodo[temp]->next; //Segundo nó da lista de adjacência

	while(aux!=NULL)
	{
		temp=buscarIndiceNodo(listaAdj,aux->rotulo);
		if(listaAdj.nodo[temp]->visited==FALSE)
			return listaAdj.nodo[temp]->rotulo;
		aux=aux->next;
	}
	return -1;
}

void zerar_visitados(LISTAADJ listaAdj)
{
	int i;

	for(i=0;i<listaAdj.tam;i++)
		listaAdj.nodo[i]->visited=FALSE;
}

int existeArco(LISTAADJ listaAdj,int deRotulo,int paraRotulo)
{
	int posicao;
	NODO* aux;

	if((posicao=buscarIndiceNodo(listaAdj,deRotulo))<0)
		return FALSE;
	aux=listaAdj.nodo[posicao]->next;
	while((aux!=NULL)&&(aux->rotulo!=paraRotulo))
		aux=aux->next;
	if(aux!=NULL)
		return TRUE;
	return FALSE;
}

int buscarIndiceNodo(LISTAADJ listaAdj,int rotulo)
{
	int i;
	if(rotulo)
	for(i=0;i<listaAdj.tam;i++)
		if((listaAdj.nodo[i]!=NULL)&&((listaAdj.nodo[i]->rotulo)==rotulo))
			return i;
	return -1;
}

void exibir_lista_adj(LISTAADJ listaAdj){
	int i;
	NODO* aux;

	printf("\nLista de adjacência:\n");

	for(i=0;i<listaAdj.tam;i++)
	{
		printf("\n%d(%d) - ",(listaAdj.nodo[i])->rotulo,(listaAdj.nodo[i])->index);
		aux=(listaAdj.nodo[i])->next;
		while(aux!=NULL){
			printf("%d(%d)",aux->rotulo,aux->index);
			aux=aux->next;
			if(aux!=NULL)
				printf(", ");
		}
	}
}

LISTAADJ* criar_lista_adj(void){
	LISTAADJ* new;
	if (!(new=(LISTAADJ*)malloc(sizeof(LISTAADJ))))
	{
		printf("Error at memory reservation.\n");
		exit(1);
	}
	new->nodo=NULL;
	new->tam=0;
	return new;
}

int inserir_lista(NODO **lista,int rotulo,int index)
{
	NODO *aux,*novoNodo;
	aux=(*lista);

	if((novoNodo=(NODO *)malloc(sizeof(NODO)))==NULL)
		return FALSE;

	novoNodo->visited=FALSE;
	novoNodo->rotulo=rotulo;
	novoNodo->next=NULL;
	novoNodo->index=index;

	if(*lista==NULL) //se a lista está vazia
		*lista=novoNodo;

	else
	{
		while(aux->next!=NULL)
			aux=aux->next;
		aux->next=novoNodo;
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
int ler_estacoes(char* arquivo,LISTAADJ* listaAdj) //Retorna FALSE caso não consiga ler um arquivo com uma lista de adjacências
{
   FILE *fp;
   int rotulo,index,temp,listaAtual;
   char aux;

   if((fp=fopen(arquivo,"r"))==NULL)
	  return FALSE;
   while((getc(fp))!=EOF)
   {
	  fseek(fp,-1*sizeof(char),SEEK_CUR);
	  fscanf(fp,"%d%c",&rotulo,&aux);

		if(aux=='-')
		{
			listaAtual=rotulo;
			index=buscarIndiceNodo(*listaAdj,rotulo);
			//printf("\n\nINDICE: %d\n",index);
			if(index==-1)
			{
				(listaAdj->tam)++;
				listaAdj->nodo=(NODO **) realloc(listaAdj->nodo,(listaAdj->tam+1)*sizeof(NODO*)); //Realoca o vetor de listas
				listaAdj->nodo[listaAdj->tam-1]=NULL; //Inicializa a lista
				index=listaAdj->tam-1;
				inserir_lista(&(listaAdj->nodo[index]),rotulo,index); //Insere na lista 'i'
			}
		}
		else
		{
			index=buscarIndiceNodo(*listaAdj,rotulo);
			if(index<0)
			{
				(listaAdj->tam)++;
				listaAdj->nodo=(NODO **) realloc(listaAdj->nodo,(listaAdj->tam+1)*sizeof(NODO*)); //Realoca o vetor de listas
				listaAdj->nodo[listaAdj->tam-1]=NULL; //Inicializa a lista
				inserir_lista(&(listaAdj->nodo[listaAdj->tam-1]),rotulo,listaAdj->tam-1); //Insere na lista
				temp=buscarIndiceNodo(*listaAdj,listaAtual);
				inserir_lista(&(listaAdj->nodo[temp]),rotulo,listaAdj->tam-1);
			}
			else
			{
				temp=buscarIndiceNodo(*listaAdj,listaAtual);
				inserir_lista(&(listaAdj->nodo[temp]),rotulo,index);
			}
		}
   }
   fclose(fp);
   return TRUE;
}
int carregar_matriz_int(char* arquivo,int matriz[][NUM_STATIONS])
{
	FILE *fp;
	int i,j;
	char aux;

	if((fp=fopen(arquivo,"r"))==NULL)
		return FALSE;
	for(i=0;i<NUM_STATIONS;i++)
	{
		for(j=0;j<NUM_STATIONS;j++)
			fscanf(fp,"%d%c",&(matriz[i][j]),&aux);
	}
	fclose(fp);

	return TRUE;
}
int carregar_matriz_double(char* arquivo,double matriz[][NUM_STATIONS])
{
	FILE *fp;
	int i,j;
	char aux;

	if((fp=fopen(arquivo,"r"))==NULL)
		return FALSE;
	for(i=0;i<NUM_STATIONS;i++)
	{
		for(j=0;j<NUM_STATIONS;j++)
		{
			fscanf(fp,"%lf%c",&(matriz[i][j]),&aux);
			printf("lido: %lf\n",matriz[i][j]);
		}
	}
	fclose(fp);

	return TRUE;
}


FILA* criar_fila(void)
{
	FILA *aux;
	if (!(aux=(FILA*)malloc(sizeof(FILA))))
		error_m("Erro ao alocar memória.");
	aux->topo=NULL;
	aux->tam=0;
	return aux;
}

void destruir_fila(FILA **fila)
{
	NODO *ant,*next;

	if((*fila)->topo!=NULL)
	{
		ant=(*fila)->topo;
		next=ant->next;
		while(next!=NULL)
		{
			free(ant);
			ant=next;
			next=next->next;
		}
		free(ant);
	}
	free(*fila);
	*fila=NULL;
}
void empilhar(FILA *lifo,NODO *nodo)
{
	NODO *aux;

	if (!(aux=(NODO*)malloc(sizeof(NODO))))
		error_m("Erro ao alocar memória.");
	aux->rotulo=nodo->rotulo;
	aux->index=nodo->index;
	aux->visited=nodo->visited;
	aux->next=lifo->topo;

	lifo->topo=aux;
	(lifo->tam)++;
}

void enfileirar(FILA *fifo,NODO *nodo)
{
	NODO *aux,*novo;

	if(!(novo=(NODO*)malloc(sizeof(NODO))))
			error_m("Erro ao alocar memória.");

	novo->rotulo=nodo->rotulo;
	novo->index=nodo->index;
	novo->visited=nodo->visited;
	novo->next=NULL;

	if((fifo->topo)==NULL) //Se a fila está vazia
		fifo->topo=novo;
	else
	{
		aux=fifo->topo;
		while((aux->next)!=NULL)
			aux=aux->next;
		aux->next=novo;
	}
	(fifo->tam)++;
}

NODO desenfileirar(FILA *fila)
{
	NODO *aux,temp;

	if((fila->topo)!=NULL)
	{
		aux=fila->topo;
		temp=*aux;

		fila->topo=fila->topo->next;
		(fila->tam)--;
		free(aux);
		return temp;
	}
	temp.rotulo=-1;
	return temp;

}
void exibir_fila(FILA fila)
{
	NODO* aux;

	aux=fila.topo;
	while(aux!=NULL)
	{
		printf("%d  ",aux->rotulo);
		aux=aux->next;
	}
}

void exibir_fila_invertida(NODO *topo)
{
	if(topo!=NULL)
	{
		exibir_fila_invertida(topo->next);
		printf("%d  ",topo->rotulo);
	}
}

void copiar_fila(FILA *origem,FILA **destino)
{
	NODO* aux;
	FILA *lifoTemp;

	destruir_fila(destino);
	*destino=criar_fila();
	lifoTemp=criar_fila();
	if((origem->topo)!=NULL)
	{
		aux=origem->topo;
		while(aux!=NULL)
		{
			empilhar(lifoTemp,aux);
			aux=aux->next;
		}
		aux=lifoTemp->topo;
		while(aux!=NULL)
		{
			empilhar(*destino,aux);
			aux=aux->next;
		}
	}
	destruir_fila(&lifoTemp);
}

void percurso_profundidade(LISTAADJ *listaAdj,int origemRotulo)
{
	int tempRotulo,tempIndice;
	NODO* aux;

	if((tempIndice=buscarIndiceNodo(*listaAdj,origemRotulo))<0)
		return;
	printf("%d ",listaAdj->nodo[tempIndice]->rotulo);

	listaAdj->nodo[tempIndice]->visited=TRUE;
	aux=listaAdj->nodo[tempIndice]->next;

	while(aux!=NULL)
	{
		if((listaAdj->nodo[aux->index]->visited)==FALSE)
			percurso_profundidade(listaAdj,aux->rotulo);
		aux=aux->next;
	}
}

void percurso_largura(LISTAADJ *listaAdj,int origemRotulo, FILA *fifo)
{
	int tempIndice;
	NODO *aux,temp;

	if((tempIndice=buscarIndiceNodo(*listaAdj,origemRotulo))<0)
		return;
	printf("%d ",listaAdj->nodo[tempIndice]->rotulo);

	aux=listaAdj->nodo[tempIndice]->next;

	while(aux!=NULL)
	{
		if((listaAdj->nodo[aux->index]->visited)==FALSE)
		{
			enfileirar(fifo,aux);
			listaAdj->nodo[aux->index]->visited=TRUE;
		}
		aux=aux->next;
	}
	temp=desenfileirar(fifo);
	if(temp.rotulo!=-1)
			percurso_largura(listaAdj,temp.rotulo,fifo);
}
int dijkstra(LISTAADJ *listaAdj,int origemRotulo,int destinoRotulo,	Predecessor *precede,int distancia[][NUM_STATIONS],int linha[][NUM_STATIONS])
{
	int *dist,*perm;
	int i,j,indexOrigem,indexDestino,corrente,menorDist,novaDist,dc,cont=0;
	int linhaPrecede[NUM_STATIONS],novaLinha;
	indexOrigem=buscarIndiceNodo(*listaAdj,origemRotulo);
	indexDestino=buscarIndiceNodo(*listaAdj,destinoRotulo);

	dist=NULL;
	perm=NULL;
	dist=(int*)malloc(sizeof(int)*(listaAdj->tam));
	perm=(int*)malloc(sizeof(int)*(listaAdj->tam));

	for(i=0;i<listaAdj->tam;i++)
	{
		dist[i]=INFINITE;
		perm[i]=FALSE;
		precede[i].index=-1;
		precede[i].line=-1;
	}
	perm[indexOrigem]=TRUE;
	dist[indexOrigem]=0;
	corrente=indexOrigem;
	while(corrente!=indexDestino)
	{
		//printf("\nCORRENTE: %d\n",listaAdj->nodo[corrente]->rotulo);

		cont++;
		menorDist=INFINITE;
		dc = dist[corrente];
		for(i=0;i<listaAdj->tam;i++)
		{
			if(perm[i]==FALSE)
			{
				novaDist=dc+peso(*listaAdj,corrente,i,distancia);
				novaLinha=linha[listaAdj->nodo[corrente]->rotulo-1][listaAdj->nodo[i]->rotulo-1];
				//printf("\nNOVA LINHA: %d\n",novaLinha);
				if(novaDist<dist[i])
				{
					if((precede[corrente].line!=-1)&&(precede[corrente].line!=novaLinha))
						novaDist+=2;
					if(novaDist<dist[i])
					{
						dist[i]=novaDist;
						precede[i].index=corrente;
						precede[i].line=novaLinha;
					}
				}
				if(dist[i]<menorDist)
				{
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
	j=dist[indexDestino];
	free(dist);
	free(perm);
	return j;
}

int peso(LISTAADJ listaAdj,int origem,int destino,int distancia[][NUM_STATIONS])
{
	NODO* aux;
	int rotuloOrigem,rotuloDestino;
	if(origem!=destino)
	{
		aux=listaAdj.nodo[origem]->next;
		while((aux!=NULL)&&((aux->index)!=destino))
			aux=aux->next;
		if(aux==NULL)
			return INFINITE;
		rotuloOrigem=listaAdj.nodo[origem]->rotulo;
		rotuloDestino=listaAdj.nodo[destino]->rotulo;
		return distancia[rotuloOrigem-1][rotuloDestino-1];
	}
	return 0;
}

int exibirMenorCaminho(LISTAADJ *listaAdj,Predecessor *precede,int origem,int destino)
{
	int aux,origemIndice,destinoIndice;
	FILA* lifo; //Pilha para auxiliar na exibição do caminho
	NODO *temp;
	char nomeLinha[20];
	lifo=criar_fila();

	origemIndice=buscarIndiceNodo(*listaAdj,origem);
	destinoIndice=buscarIndiceNodo(*listaAdj,destino);

	empilhar(lifo,listaAdj->nodo[destinoIndice]);
	aux=precede[destinoIndice].index;
	while(aux!=origemIndice)
	{
		empilhar(lifo,listaAdj->nodo[aux]);
		aux=precede[aux].index;
	}

	empilhar(lifo,listaAdj->nodo[origemIndice]);

	temp=lifo->topo;
	printf("\n\nPartindo da estação: E%d, ",temp->rotulo);
	temp=temp->next;
	nomeDaLinha(precede[temp->index].line,nomeLinha);
	printf("pegue a linha %s",nomeLinha);
	printf("\nVoce passará pela(s) estacão(ões): ");
	while(temp!=NULL)
	{
		printf("E%d  ",temp->rotulo);
		if((temp->next!=NULL)&&(precede[temp->index].line!=precede[temp->next->index].line))
		{
			nomeDaLinha(precede[temp->next->index].line,nomeLinha);
			printf("\nNa estacão E%d, troque para a linha %s passando pela(s) estação(ões): ",temp->rotulo,nomeLinha);
		}
		temp=temp->next;
	}
	destruir_fila(&lifo);
	return 1;
}

void nomeDaLinha(int linha,char *nome)
{
	switch(linha){
		case 1:
			strcpy(nome,"azul");
			break;
		case 2:
			strcpy(nome,"amarelo");
			break;
		case 3:
			strcpy(nome,"vermelho");
			break;
		case 4:
			strcpy(nome,"verde");
			break;
	}
}
