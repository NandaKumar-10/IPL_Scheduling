#include "matches.h"
#include "shared.h"
#include "teams.h"
#include <stdlib.h>
#include <string.h>

void simulateMatch(Match *match, Team teams[]) {
    int random = rand() % 100;
    if (random < 3) {
        strcpy(match->winner, "Draw");
        strcpy(match->losser_team, "None");
    } else if (random < 53) {
        strcpy(match->winner, match->team1);
        strcpy(match->losser_team, match->team2);
    } else {
        strcpy(match->winner, match->team2);
        strcpy(match->losser_team, match->team1);
    }
    
    float nrrChange = (float)(rand() % 100) / 100.0;
    
    if (strcmp(match->winner, match->team1) == 0) {
        for (int i = 0; i < TOTAL_TEAMS; i++) {
            if (strcmp(teams[i].name, match->team1) == 0) {
                teams[i].nrr += nrrChange;
            }
            if (strcmp(teams[i].name, match->team2) == 0) {
                teams[i].nrr -= nrrChange;
            }
        }
    } else if (strcmp(match->winner, match->team2) == 0) {
        for (int i = 0; i < TOTAL_TEAMS; i++) {
            if (strcmp(teams[i].name, match->team1) == 0) {
                teams[i].nrr -= nrrChange;
            }
            if (strcmp(teams[i].name, match->team2) == 0) {
                teams[i].nrr += nrrChange;
            }
        }
    }
}

void displayMatchDetails(Match *match, Team teams[]) {
    printf("%-25s\t%-25s\t%s\t%s\t%-30s\t", 
           match->team1, 
           match->team2,
           match->date,
           match->time,
           match->venue);
    
    if (strlen(match->winner) > 0) {
        if (strcmp(match->winner, "Draw") == 0) {
            printf("Tied\n");
        } else {
            printf("%s won\n", match->winner);
        }
    } else {
        printf("Not played\n");
    }
}


