/*
 * a-star.c
 *
 *  Created on: 07/04/2016
 *      Author: Ruan
 */

#ifndef A_STAR_C_
#define A_STAR_C_
#include "a-star.h"


node closedset[MAX_NODES]; // Nodes to explore
node openset[MAX_NODES]; // Nodes already explored
int closedset_size, openset_size;

// The f-score represents the distance between a node and the goal. A* will explore the closest ones first.
node* find_openset_lowest_f_score(){
	float min = openset[0].f;
	int lowest = 0;
	int i;
	for (i = 1; i < openset_size; i++){
		if (openset[i].f < min){
			min = openset[i].f;
			lowest = i;
		}
	}
	return &openset[lowest];
}

int is_in_closedset(node* n){
	int i;
	for (i = 0; i < closedset_size; i++){
		if (closedset[i].x == n->x && closedset[i].y == n->y)
			return 1;
	}
	return 0;
}

int is_in_openset(node* n){
	int i;
	for (i = 0; i < openset_size; i++){
		if (openset[i].x == n->x && openset[i].y == n->y)
			return 1;
	}
	return 0;
}

node* switch_to_closedset(node* n){
	int i;
	closedset[closedset_size++] = *n;
	for (i = 0; i < openset_size; i++){
		if (openset[i].x == n->x && openset[i].y == n->y){
			memmove(&openset[i], &openset[i + 1], sizeof(node)* (openset_size - i - 1));
			openset_size--;
			break;
		}
	}
	return &closedset[closedset_size - 1];
}

int astar(int* map, int map_width, int map_height, int start_x, int start_y, int goal_x, int goal_y, astar_result* result){
	result->pathsize = closedset_size = openset_size = 0;
	openset[openset_size++] = (node){ start_x, start_y, abs(start_x - goal_x) + abs(start_y - goal_y), 0, 0 };
	while (openset_size > 0){
		int i;
		node* current = find_openset_lowest_f_score();
		// A path to goal was found
		if (current->x == goal_x && current->y == goal_y){
			result->path[result->pathsize++] = *current;
			// Reconstruct the path from goal to start
			while (current->x != start_x || current->y != start_y){
				result->path[result->pathsize++] = *current = *current->previous;
			}
			// Reverse the path array
			node temp;
			for (i = 0; i < result->pathsize / 2; i++){
				temp = result->path[i];
				result->path[i] = result->path[result->pathsize - 1 - i];
				result->path[result->pathsize - 1 - i] = temp;
			}
			return 1;
		}
		current = switch_to_closedset(current);
		// Explore the 4 directions near our current node
		for (i = 0; i < 4; i++){
			struct node neighbor = { current->x, current->y, 0, 0, 0 };
			switch (i){
				case 0: neighbor.y--; break;
				case 1: neighbor.x++; break;
				case 2: neighbor.y++; break;
				case 3: neighbor.x--; break;
			}
			// Ignore already explored cells, out-of-the-map cells and unwalkable ones
			if (is_in_closedset(&neighbor) || neighbor.x < 0 || neighbor.y < 0 || neighbor.x >= map_width || neighbor.y >= map_height || map[neighbor.x * map_height + neighbor.y] > 0)
				continue;
			if (!is_in_openset(&neighbor) || current->g + 1 < neighbor.g){
				neighbor.previous = current;
				neighbor.g = current->g + 1;
				neighbor.f = neighbor.g + abs(neighbor.x - goal_x) + abs(neighbor.y - goal_y);
				if (!is_in_openset(&neighbor))
					openset[openset_size++] = neighbor;
			}
		}
	}
	// No path was found
	return 0;
}



#endif /* A_STAR_C_ */
