/*
#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEAMS_PER_GROUP 5
#define TOTAL_TEAMS 10
#define MAX_NAME_LENGTH 30

#define INTRA_GROUP_MATCHES_PER_TEAM 4
#define CROSS_GROUP_MATCHES_PER_TEAM 10
#define MATCHES_PER_TEAM (INTRA_GROUP_MATCHES_PER_TEAM + CROSS_GROUP_MATCHES_PER_TEAM)

#define TOTAL_INTRA_GROUP_MATCHES 20
#define TOTAL_CROSS_GROUP_MATCHES 50
#define TOTAL_MATCHES (TOTAL_INTRA_GROUP_MATCHES + TOTAL_CROSS_GROUP_MATCHES)

// Structure for team information
typedef struct {
    char name[MAX_NAME_LENGTH];
    int group;               // 0 for Group A, 1 for Group B
    int played;
    int won;
    int lost;
    int tied;
    int points;
    float nrr;  // Net Run Rate
} Team;

// Structure for match information
typedef struct {
    int team1;
    int team2;
    int played;
    int winner;  // -1 for not played, 0 for tie, 1 for team1, 2 for team2
    int isCrossGroupMatch;  // 0 for intra-group, 1 for cross-group
    char date[20];         // Date of the match
    char time[10];         // Time of the match
    char venue[50];        // Venue of the match
} Match;

// Array of venues
extern const char* venues[];

#endif
*/

#ifndef SHARED_H
#define SHARED_H
#include "teams.h"
#define NUM_TEAMS 10
#define NUM_VENUES 10

//extern Team teams[NUM_TEAMS];  // Array of team structures
extern const char *venues[NUM_VENUES];

#endif //SHARED_H