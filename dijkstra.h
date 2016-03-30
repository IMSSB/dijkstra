/*
 * dijkstra.h
 *
 *  Created on: 29/03/2016
 *      Author: ricardo
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_
#include <stdio.h>
#define INFINITY 1
#define MAXNODES 1
#define MEMBER 1
#define NONMEMBER 0

typedef struct
{
	int **elementos;

}Matriz;
void print_mat(int ordem,Matriz *x);
void  alloc_mat(int ordem, Matriz *mat);
void ler_mat_arq(Matriz *x);
void shortpath(int weight[][MAXNODES], int s, int t, int *pd, int precede[]);
#endif /* DIJKSTRA_H_ */
