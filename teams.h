#ifndef TEAMS_H
#define TEAMS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TOTAL_TEAMS 10

typedef struct {
    char name[50];
    int matches_played;
    int wins;
    int losses;
    int ties;
    int points;
    float nrr;
} Team;

Team* initializeTeams(void);
void sort(Team* teams, int n);

#endif


