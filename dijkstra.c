/*
 * dijkstra.c
 *
 *  Created on: 29/03/2016
 *      Author: ricardo
 */
#include <stdio.h>
#define INFINITY 1
#define MAXNODES 1
#define MEMBER 1
#define NONMEMBER 0

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
void main(void)
{

}
