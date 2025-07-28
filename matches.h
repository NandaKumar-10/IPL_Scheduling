#ifndef MATCHES_H
#define MATCHES_H
//#define "schedule.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "teams.h"

typedef struct {
    char team1[50];
    char team2[50];
    char date[20];
    char time[20];
    char venue[100];
    char winner[50];
    char losser_team[50];
} Match;

void simulateMatch(Match *match, Team teams[]);
void displayMatchDetails(Match *match, Team teams[]);

#endif



