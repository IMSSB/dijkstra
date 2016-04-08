/*
 * metroparis.c
 *
 *  Created on: 31/03/2016
 *      Author: Ruan
 */
#include "metroparis.h"
/*NODO lista_abertos[NUM_STATIONS];
NODO lista_fechados[NUM_STATIONS];
int tam_abertos, tam_fechados;*/

int main()
{
	project_presentation();
	metro_paris();

	return 0;
}
//---------------------------------------------------------------------------
int metro_paris()
{
	int origem=1,destino=1;
	int linha[NUM_STATIONS][NUM_STATIONS];
	double distancia[NUM_STATIONS][NUM_STATIONS], distancia_real[NUM_STATIONS][NUM_STATIONS];
	STAR_RESULT *AEstrela;
	int horas,minutos,segundos;
	//distancia[i][j] refere-se a distancia em linha reta entre o nó de rótulo [i+1] e o nó de rotulo[j+1]
	LISTAADJ *listaAdj;
	predecessor *precede;
	if (!(AEstrela = (STAR_RESULT *)malloc(sizeof(STAR_RESULT))))
		error_m("Erro ao alocar memória.");

	listaAdj=criar_lista_adj();

	if(!ler_estacoes(FILE_STATIONS,listaAdj))
		error_m("Erro ao ler arquivo de estacões.");
	if(!carregar_matriz_double(FILE_DISTANCES,distancia))
		error_m("Erro ao ler arquivo de distancias.");
	if(!carregar_matriz_double(FILE_REAL_DISTANCES,distancia_real))
		error_m("Erro ao ler arquivo de distancias.");
	if(!carregar_matriz_int(FILE_LINES,linha))
		error_m("Erro ao ler arquivo de linhas.");

	while(origem!=0)
	{
		exibir_lista_adj(*listaAdj);
		pause;
		printf("\n\nDigite a estação de origem: E");
		scanf("%d",&origem);
		if(!origem)
			return 1;
		printf("\n\nDigite a estação de destino: E");
		scanf("%d",&destino);
		if(!destino)
			return 1;

		if (!(precede=(predecessor*)malloc((sizeof(predecessor)*listaAdj->tam))))
			error_m("Erro ao alocar memória.");

		astar(listaAdj,origem,destino,distancia,distancia_real,linha,AEstrela);
		exibir_star_result(AEstrela);
		pause;

		printf("\ntempo: %lf, distancia: %lf\n",AEstrela->tempo,AEstrela->distancia);

		pause;
		if(AEstrela->tempo > 0)
		{
			horas = AEstrela->tempo/60;
			minutos = ((int)AEstrela->tempo)%60;
			segundos = (AEstrela->tempo-((int)AEstrela->tempo))*60;
			printf("Tempo do trajeto mais rápido pelo Algoritmo A* (A-Estrela)= %d horas, %d minutos e %d segundos\n",horas, minutos,segundos);
			printf("Distância do trajeto = %.2lf km\n",AEstrela->distancia);
			printf("Trajeto: \n");
			exibir_trajeto_mais_rapido(AEstrela,linha);
		}
		else
			printf("\n\nNão existe uma rota de E%d para E%d\n",origem,destino);

		getc(stdin);
		getc(stdin);
		cls;
	}
	return 0;
}


void error_m(char *errormessage)
{ 	/* Fun��o para facilitar exibi��o de mensagens de erro */
    printf("\n%s",errormessage);
    pause;
    exit(1);
}
void project_presentation()
{
	big_line;
	formated_message("Metro de Paris");
	formated_message("Trabalho da Disciplina Algoritmos e Estrutura de Dados II");
	formated_message("Professora: Ana Emilia de Melo Queiroz");
	formated_message("Alunos:     Ricardo Valério Teixeira de Medeiros Silva");
	formated_message("            Ruan de Medeiros Bahia");
	formated_message("Calculando o trajeto mais rápido entre duas estações no ");
	formated_message("Metro de Paris utilizando o algoritmo A* (A-Estrela).");
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

int buscar_indice_nodo(LISTAADJ listaAdj,int rotulo)
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

LISTAADJ* criar_lista_adj(){
	LISTAADJ* new;
	if (!(new=(LISTAADJ*)malloc(sizeof(LISTAADJ))))
		error_m("Erro na alocação de memória.");
	new->nodo=NULL;
	new->tam=0;
	return new;
}

LISTA* criar_lista()
{
	LISTA *new;
	if (!(new=(LISTA*)malloc(sizeof(LISTA))))
		error_m("Erro na alocação de memória.");
	new->primeiro = NULL;
	new->tam = 0;
	return new;
}

void retirar_lista(LISTA *lista, int pos)
{
	NODO *rem, *aux;
	if (pos >=0 && pos < lista->tam)
	{
		for (aux = rem = lista->primeiro; (pos+1); pos--)
		{
			aux = rem;
			rem = rem->next;
		}
		if (aux->next)
			aux->next = rem->next;
		lista->tam--;
		free(rem);
	}
	return;
}

void destruir_lista(LISTA *lista)
{
	NODO *ant, *prox;
	if (lista->primeiro != NULL)
	{
		ant = lista->primeiro;
		prox = ant->next;
		while(prox != NULL)
		{
			free(ant);
			ant=prox;
			prox=prox->next;
		}
		free(ant);
	}
	free(lista);
	lista = NULL;

	return;
}
int inserir_lista(LISTA *lista,int rotulo,int index,NODO *origin,double f, double g, double h)
{
	NODO *aux,*novo;
	aux=lista->primeiro;

	if(!(novo=(NODO *)malloc(sizeof(NODO))))
		error_m("Erro na alocação de memória.");

	novo->rotulo=rotulo;
	novo->next=NULL;
	novo->origin = origin;
	novo->index=index;
	novo->visited=FALSE;
	novo->f = f;
	novo->g = g;
	novo->h = h;
	if(lista->primeiro==NULL)
		lista->primeiro=novo;
	else
	{
		while(aux->next)
			aux=aux->next;
		aux->next=novo;
	}
	lista->tam++;
	return TRUE;
}

int inserir_lista_adj(NODO **lista,int rotulo,int index)
{
	NODO *aux,*novoNodo;
	aux=(*lista);

	if((novoNodo=(NODO *)malloc(sizeof(NODO)))==NULL)
		return FALSE;

	novoNodo->visited=FALSE;
	novoNodo->rotulo=rotulo;
	novoNodo->next=novoNodo->origin=NULL;
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
	  fseek(fp,sizeof(char)*-1,SEEK_CUR);
	  fscanf(fp,"%d%c",&rotulo,&aux);

		if(aux=='-')
		{
			listaAtual=rotulo;
			index=buscar_indice_nodo(*listaAdj,rotulo);

			if(index==-1)
			{
				(listaAdj->tam)++;
				listaAdj->nodo=(NODO **) realloc(listaAdj->nodo,(listaAdj->tam+1)*sizeof(NODO*)); //Realoca o vetor de listas
				listaAdj->nodo[listaAdj->tam-1]=NULL; //Inicializa a lista
				index=listaAdj->tam-1;
				inserir_lista_adj(&(listaAdj->nodo[index]),rotulo,index); //Insere na lista 'i'
			}
		}
		else
		{
			index=buscar_indice_nodo(*listaAdj,rotulo);
			if(index<0)
			{
				(listaAdj->tam)++;
				listaAdj->nodo=(NODO **) realloc(listaAdj->nodo,(listaAdj->tam+1)*sizeof(NODO*)); //Realoca o vetor de listas
				listaAdj->nodo[listaAdj->tam-1]=NULL; //Inicializa a lista
				inserir_lista_adj(&(listaAdj->nodo[listaAdj->tam-1]),rotulo,listaAdj->tam-1); //Insere na lista
				temp=buscar_indice_nodo(*listaAdj,listaAtual);
				inserir_lista_adj(&(listaAdj->nodo[temp]),rotulo,listaAdj->tam-1);
			}
			else
			{
				temp=buscar_indice_nodo(*listaAdj,listaAtual);
				inserir_lista_adj(&(listaAdj->nodo[temp]),rotulo,index);
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

	if((fifo->topo)==NULL)
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

int esta_na_lista(LISTA *lista,NODO *nodo)
{
	NODO *aux;
	for (aux = lista->primeiro;aux; aux = aux->next)
		if (aux->rotulo == nodo->rotulo)
			return 1;
	return 0;
}

NODO *encontrar_menor_valor_f(LISTA *lista)
{
	NODO *aux,*menor = lista->primeiro;
	double min = lista->primeiro->f;
	printf("->f=%lf\n",min);
	for (aux = lista->primeiro->next;aux; aux = aux->next)
	{
		printf("->f=%lf\n",aux->f);
		if (aux->f < min)
		{
			min = aux->f;
			menor = aux;
 		}
	}
	return menor;
}

void trocar_nodo_de_lista(NODO *nodo, LISTA *de, LISTA *para)
{
	NODO *aux, *ant;
	for (ant = aux = de->primeiro; aux != nodo ; aux = aux->next)
		ant = aux;
	printf("TRETA1\n");
	if (!(--de->tam))
		de->primeiro = NULL;
	else if (ant == aux)
		de->primeiro = ant->next;
	else
		ant->next = aux->next;
	printf("TRETA2\n");
	if (para->tam)
	{
		printf("TRETA3\n");
		aux = para->primeiro;
		printf("TRETA3.2\n");
		while(aux->next)
			aux=aux->next;
		printf("TRETA3.3\n");
		aux->next=nodo;
	}
	else
		para->primeiro = nodo;
	nodo->next = NULL;
	printf("TRETA4\n");
	para->tam++;

	return;
}

int astar(LISTAADJ *listaAdj,int origemRotulo,int destinoRotulo,double distancia[][NUM_STATIONS],double distancia_real[][NUM_STATIONS],int linha[][NUM_STATIONS],STAR_RESULT *result)
{
	int indice_origem,indice_destino, *linhas;
	int baldeou = 0, heuri = 0, linha_atual;
	NODO *aux;
	LISTA *abertos = criar_lista(), *fechados=criar_lista();

	result->tempo = result->distancia = 0;
	result->tam = 0;

	printf("TESTE1\n");

	indice_origem=buscar_indice_nodo(*listaAdj,origemRotulo);
	indice_destino=buscar_indice_nodo(*listaAdj,destinoRotulo);

	inserir_lista(abertos,origemRotulo,indice_origem,NULL,peso(*listaAdj,indice_origem,indice_destino,distancia)/SPEEDM,0,peso(*listaAdj,indice_origem,indice_destino,distancia)/SPEEDM);

	linhas = linhas_rotulo(linha,destinoRotulo);	//	Linhas usadas na comparação para otimização heurística
	printf("TESTE2\n");
	while (abertos->tam > 0)
	{
		int i;
		double f,g,h;
		NODO *atual = encontrar_menor_valor_f(abertos);
		if (atual->rotulo == destinoRotulo) // Um caminho foi encontrado
		{
			NODO temp;
			result->caminho[result->tam++] = *atual;
			while (atual->rotulo != origemRotulo)
				result->caminho[result->tam++] = *atual = *atual->origin;

			for (i = 0;i<result->tam /2;i++)
			{
				temp = result->caminho[i];
				result->caminho[i] = result->caminho[result->tam - 1 - i];
				result->caminho[result->tam - 1 - i] = temp;
			}
			for (i = 1;i<result->tam; i++)
				result->distancia += peso(*listaAdj,result->caminho[i].origin->index,result->caminho[i].index,distancia_real);

			result->tempo = result->caminho[result->tam-1].g;
			printf("\n\n\n\n\n\n\nTEMPO: %lf\n\n\n",result->tempo);
			pause;
			return 1;
		}
		printf("TESTE3 =\n");
		exibir_lista(abertos);
		pause;
		printf("é\n");
		exibir_lista(fechados);
		pause;
		printf("HEHEHEHEH-------------------------\n");exibir_nodo(atual);
		trocar_nodo_de_lista(atual, abertos, fechados);
		printf("TESTE4\n");
		for(aux = listaAdj->nodo[atual->index]->next; aux;aux = aux->next)
		{
			printf("TESTE5\n");
			if (esta_na_lista(fechados,aux))
				continue;
			printf("TESTE6\n");
			baldeou = heuri = 0;
			linha_atual = linha[atual->rotulo-1][aux->rotulo-1];
			if (atual->origin)
			{	//	Se linha anterior difere de linha atual
				if(linha[atual->origin->rotulo-1][atual->rotulo-1] != linha_atual)
					baldeou = 1;
			}
			printf("TESTE7\n");
			if (linha_atual != linhas[0] && linha_atual != linhas[1])
				heuri = 1;	//	Verifica se a linha atual é uma das linhas que passam pelo destino

			if (!esta_na_lista(abertos,aux))
			{
				h = (peso(*listaAdj,aux->index,indice_destino,distancia)/SPEEDM) + (heuri?TRANSHIPMENT:0);
				g = atual->g + (peso(*listaAdj,atual->index,aux->index,distancia_real)/SPEEDM) + (baldeou?TRANSHIPMENT:0);
				f = g + h;
				if (aux->rotulo == 9)
				{
					exibir_lista(abertos);
					pause;

				}
				printf("aux=%d, aux->index=%d, aux->rotulo=%d, indice_destino=%d",aux, aux->index,aux->rotulo,indice_destino);
				printf("f = %lf  g= %lf  h= %lf \n",f,g,h);
				printf("\nheuri=%d,baldeou=%d\n",heuri,baldeou);

				inserir_lista(abertos,aux->rotulo,aux->index,atual,f,g,h);
				printf("TESTE8\n");
			}
			else
			{	printf("TESTE9\n");
				NODO *aux2 = abertos->primeiro;
				while (aux2->rotulo != aux->rotulo && aux2->next)
					aux2 = aux2->next;
				if (!aux2->next)
					continue;
				if (atual->g + peso(*listaAdj,aux->index,indice_destino,distancia) < aux2->g)
				{
					aux2->origin = atual;
					aux2->h = (peso(*listaAdj,aux->index,indice_destino,distancia)/SPEEDM) + (heuri?TRANSHIPMENT:0);
					aux2->g = atual->g + (peso(*listaAdj,atual->index,aux->index,distancia_real)/SPEEDM) + (baldeou?TRANSHIPMENT:0);
					aux2->f = aux2->g + aux2->h;
				}
			}
			pause;
		}
	}
	return 0;
	printf("TESTE - FINALLL\n");
}

void exibir_star_result(STAR_RESULT *result)
{
	int c;
	printf("EXIBIR STAR_RESULT\n");
	printf("STAR_RESULT - tempo: %lf\n",result->tempo);
	printf("STAR_RESULT - distancia: %lf\n",result->distancia);
	printf("STAR_RESULT - tamanho: %d\n",result->tam);
	for (c=0;c<result->tam;c++)
		exibir_nodo(&result->caminho[c]);
}

void exibir_nodo(NODO *nodo)
{
	printf("nodo->rotulo = %d\n",nodo->rotulo);
	printf("nodo->index = %d\n",nodo->index);
	printf("nodo->f = %lf\n",nodo->f);
	printf("nodo->g = %lf\n",nodo->g);
	printf("nodo->h = %lf\n",nodo->h);
	printf("nodo->origin = %d\n",nodo->origin);
	printf("nodo->next = %d\n",nodo->next);
	printf("nodo->visited = %d\n",nodo->visited);
}

void exibir_lista(LISTA *lista)
{
	NODO *aux;
	int c;
	printf("\n\nEXIBINDO LISTA\n");
	printf("lista->tam = %d\n",lista->tam);
	for (c = 0, aux = lista->primeiro; aux ;aux = aux->next, c++)
	{
		printf("NODO %d\n",c);
		exibir_nodo(aux);
	}
}

double peso(LISTAADJ listaAdj,int origem,int destino,double distancia[][NUM_STATIONS])
{
	int rotuloOrigem,rotuloDestino;
	double d;
	if(origem!=destino)
	{
		rotuloOrigem=listaAdj.nodo[origem]->rotulo;
		rotuloDestino=listaAdj.nodo[destino]->rotulo;

		d = distancia[rotuloOrigem-1][rotuloDestino-1];
		if(d==-1)
			return INFINITE;
		return d;
	}
	return 0;
}

int exibir_trajeto_mais_rapido(STAR_RESULT *result,int linha[][NUM_STATIONS])
{
	NODO *temp;
	int c, linha_atual, proxima_linha;
	char nomeLinha[20];

	printf("\n\nPartindo da estação E%d ",result->caminho[0].rotulo);
	linha_atual = linha[result->caminho[0].rotulo -1][result->caminho[1].rotulo -1];
	nome_da_linha(linha_atual,nomeLinha);
	printf("pegue a linha %s.\n",nomeLinha);
	printf("Você passará pela(s) estacão(ões)  ");
	for (c = 1;c < result->tam; c++)
	{
		printf("E%d",result->caminho[c].rotulo);
		if (c < result->tam-1)
		{
			proxima_linha = linha[result->caminho[c].rotulo -1][result->caminho[c+1].rotulo -1];
			if (proxima_linha != linha_atual)
			{
				nome_da_linha(proxima_linha,nomeLinha);
				printf(".\nNa estacão E%d troque para a linha %s passando pela(s) estação(ões) ",result->caminho[c].rotulo,nomeLinha);
				linha_atual = proxima_linha;
			}
			else
				printf(" ");
		}
		else
			printf(".\n");
	}

	return 0;
}

void nome_da_linha(int linha,char *nome)
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
int* linhas_rotulo(int linha[][NUM_STATIONS], int destinoRotulo)
{
	int c, *linhas,v=0,aux;
	if (!(linhas = (int *) malloc(sizeof(int)*2)))
		error_m("Erro ao alocar memória.");

	linhas[0] = linhas[1] = 0;
	for (c=0;c < NUM_STATIONS;c++)
	{
		aux = linha[destinoRotulo -1][c];
		if (aux > 0)
		{
			if (!v)
			{
				linhas[0] = linhas[1] = aux;
				v = 1;
			}
			else if (aux != linhas[0])
			{
				linhas[1] = aux;
				break;
			}
			else
				continue;
		}
	}
	return linhas;
}
