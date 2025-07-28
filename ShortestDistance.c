#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "ShortestDistance.h"

// Stadium names array
char stadium[V][50] = {
    "Chennai",
    "Bangalore",
    "Hyderabad",
    "Mumbai",
    "Ahmedabad",
    "Jaipur",
    "Delhi",
    "Mohali",
    "Lucknow",
    "Kolkata"
};

// Distance matrix between venues (in hours of travel)
int dis[V][V] = {
    {0, 1, 2, 0, 0, 0, 0, 0, 0, 0},    // Chennai
    {1, 0, 2, 3, 0, 0, 0, 0, 0, 0},    // Bangalore
    {2, 2, 0, 3, 0, 0, 0, 0, 0, 4},    // Hyderabad
    {0, 3, 3, 0, 1, 2, 0, 0, 0, 0},    // Mumbai
    {0, 0, 0, 1, 0, 1, 3, 3, 0, 0},    // Ahmedabad
    {0, 0, 0, 2, 1, 0, 2, 2, 0, 0},    // Jaipur
    {0, 0, 0, 0, 3, 2, 0, 1, 3, 0},    // Delhi
    {0, 0, 0, 0, 3, 2, 1, 0, 0, 0},    // Mohali
    {0, 0, 0, 0, 0, 0, 3, 0, 0, 2},    // Lucknow
    {0, 0, 4, 0, 0, 0, 0, 0, 2, 0}     // Kolkata
};

int find_Index_ofCity(char Stadium[], char name[50]) {
    int index = 0;
    for (int i = 0; i < V; i++) {
        if (strcmp(name, Stadium) == 0) {
            index = i;
            return index;
        }
    }
    return 0;
}

int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX;
    int min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

void printSolution(int dist[]) {
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t\t %d\n", i, dist[i]);
}

int *dijkstra(int graph[V][V], int src) {
    int *dist = (int *)malloc(V * sizeof(int));
    bool sptSet[V];

    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;
        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    return dist;
}

char *ShortestCityName(int shortDis[], int k) {
    int temp[V];
    for (int i = 0; i < V; i++) {
        temp[i] = shortDis[i];
    }
    int minIndex = -1;
    for (int j = 0; j < k; j++) {
        int minValue = INT_MAX;
        for (int i = 0; i < V; i++) {
            if (temp[i] < minValue) {
                minValue = temp[i];
                minIndex = i;
            }
        }
        temp[minIndex] = INT_MAX;
    }
    char *name = (char *)malloc(20 * sizeof(char));
    strcpy(name, stadium[minIndex]);
    return name;
} 