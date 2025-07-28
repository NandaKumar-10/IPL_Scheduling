#include "teams.h"
#include "shared.h"
#include "Input.h"
#include <stdlib.h>
#define MAX_TEAMS 10
Team* initializeTeams(){
    Team* teams = (Team*)malloc(MAX_TEAMS * sizeof(Team));
    char **teamNames=GetTeamName();
    for (int i = 0; i < TOTAL_TEAMS; i++) {
      	//teams[i].name = (char*)malloc(50 * sizeof(char));
        strcpy(teams[i].name, teamNames[i]);
        teams[i].matches_played = 0;
        teams[i].wins = 0;
        teams[i].losses = 0;
        teams[i].ties = 0;
        teams[i].points = 0;
        teams[i].nrr = 0.0;
    }
    
    // Free the teamNames array
    for (int i = 0; i < MAX_TEAMS; i++) {
        free(teamNames[i]);
    }
    free(teamNames);
    
    return teams;
}

void displayTeamAssignments(Team teams[]) {
    printf("\n===== TEAM ASSIGNMENTS =====\n");
    for (int i = 0; i < TOTAL_TEAMS; i++) {
        printf("%d. %s\n", i+1, teams[i].name);
    }
    printf("\n");
}

