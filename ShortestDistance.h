#ifndef SHORTESTDISTANCE_H
#define SHORTESTDISTANCE_H

#include <stdbool.h>
#include <limits.h>

#define V 10

// Distance matrix between venues
extern int dis[V][V];

// Function declarations
int find_Index_ofCity(char Stadium[], char name[50]);
int minDistance(int dist[], bool sptSet[]);
void printSolution(int dist[]);
int *dijkstra(int graph[V][V], int src);
char *ShortestCityName(int shortDis[], int k);

#endif // SHORTESTDISTANCE_H 

