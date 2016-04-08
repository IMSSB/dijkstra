/*
 * a-star.h
 *
 *  Created on: 07/04/2016
 *      Author: Ruan
 */

#ifndef A_STAR_H_
#define A_STAR_H_
/*  Simple and fast A* pathfinding, v1.1
*
*	This algorithm is 4 directions only (no diagonals) and all map cells are considered of equal cost.
*
*	Feel free to use it for your projects !
*
*	Author : St�phane Damo
*	stephane.damo@gmail.com / https://github.com/stephanedamo / http://stephanedamo.fr
*/

#ifndef ASTAR_H
#define ASTAR_H
#include <string.h>
#include <math.h>

// Max analyzed nodes during pathfinding. Increase it for big maps. Will crash if MAX_NODES is reached during calculation.
#define MAX_NODES 1000

// A node is a cell (x, y) of your map
typedef struct node node;
struct node{
	int x, y; // Node coordinates
	float f, g, h; // Internal variables used for A* algorithm
	node* previous; // Previous node
	int rotulo;
};

typedef struct astar_result{
	node path[MAX_NODES];
	int pathsize;
}astar_result;


// Calculate the path between start and goal.
// map : pointer to a 2D int array (you can change int for short, char or other to suit your needs). 0 : walkable, anything > 0 : unwalkable. See astar.c line 82 (last condition) to change which values are considered walkable or not.
// result : pointer to a structure that holds the results of the pathfinding. Read result->path[i] (i from 0 to result->pathsize-1) to get your path.
// Returns 1 if a path was found, 0 if not.
int aastar(int* map, int map_width, int map_height, int start_x, int start_y, int goal_x, int goal_y, astar_result* result);


#endif



#endif /* A_STAR_H_ */
