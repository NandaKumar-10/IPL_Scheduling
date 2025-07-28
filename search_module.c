//
// Created by nanda on 03-04-2025.
//

#include<stdio.h>
#include "matches.h"
#include "search_module.h"

void searchByTeamName(Match* matches, int total_matches, const char* team_name) {
    printf("\nMatches for team '%s':\n", team_name);
    int found = 0;

    for (int i = 0; i < total_matches; i++) {
        if (strcmp(matches[i].team1, team_name) == 0 || strcmp(matches[i].team2, team_name) == 0) {
            printf("Match %d: %s vs %s on %s at %s\n",
                   i+1,
                   matches[i].team1,
                   matches[i].team2,
                   matches[i].date,
                   matches[i].venue);
            found = 1;
        }
    }

    if (!found) {
        printf("No matches found for team '%s'\n", team_name);
    }
}

void searchByDate(Match* matches, int total_matches, const char* date) {
    printf("\nMatches on date '%s':\n", date);
    int found = 0;

    for (int i = 0; i < total_matches; i++) {
        if (strcmp(matches[i].date, date) == 0) {
            printf("Match %d: %s vs %s at %s, Time: %s\n",
                   i+1,
                   matches[i].team1,
                   matches[i].team2,
                   matches[i].venue,
                   matches[i].time);
            found = 1;
        }
    }

    if (!found) {
        printf("No matches found on date '%s'\n", date);
    }
}