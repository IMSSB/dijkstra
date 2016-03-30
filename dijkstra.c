/*
 * dijkstra.c
 *
 *  Created on: 29/03/2016
 *      Author: ricardo
 */
#include "dijkstra.h"

void  alloc_mat(int ordem, Matriz *mat)
{
	int c;


	mat->elementos = (unsigned long int **)malloc(sizeof(unsigned long int)*ordem);
	if(!mat->elementos)
	{
		printf("Error pointer for pointer dinamic alocation");
		exit(0);
	}
	else
	{

		for(c=0;c<ordem;c++)
		{
			mat->elementos[c] = (unsigned long int *)malloc(sizeof(unsigned long int)*ordem);
				if(!(mat->elementos[c]))
				{
					printf("Error pointer float alocation n(%d)",c+1);
					exit(1);
				}
		}

	}
}
void print_mat(int ordem,Matriz *x)
{
	int c,c1;
	printf("\n");
	for(c=0;c<ordem;c++)
	{	printf("|");
		for(c1=0;c1<ordem;c1++)
		{
			printf(" %lu",x->elementos[c][c1]);
		}
		printf(" |\n");
	}
	printf("\n");
}

int ler_mat_arq(Matriz *x)
{
	int n,l,c;
	FILE *mat;

	if(!(mat=fopen("simi.txt","a+")))
	{
		printf("Erro ao abrir arquivo");
		exit(1);
	}

	fscanf(mat,"%d",&n);
	alloc_mat(n,x);

	for(l=0;l<n;l++)
		for(c=0;c<n;c++)
		{
			fscanf(mat,"%lu",&(x->elementos[l][c]));
		}

	return n;
}
void shortpath(int weight[][MAXNODES], int s, int t, int *pd, int precede[])
{
	int distance[MAXNODES], perm[MAXNODES];
	int current,i,k,dc;
	int smalldist, newdist;

	for (i = 0; i < MAXNODES; ++i)
	{
		perm[i] = NONMEMBER;
		distance[i] = INFINITY;
	}
	perm[s] = MEMBER;
	distance[s] = 0 ;
	current = s;
	while(current != t)
	{
		smalldist = INFINITY;
		dc = distance[current];

		for (i = 0; i < MAXNODES; i++)
			if (perm[i] == NONMEMBER)
				newdist = dc + weight[current][i];


		if (newdist < distance[i])
		{
			distance[i] = newdist;
			precede[i] = current;
		}

		if (distance[i] < smalldist)
		{
			smalldist = distance[i];
			k = i;
		}

		current = k;
		perm[current] = MEMBER;
	}
	*pd = distance[t];

}

