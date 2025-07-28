#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TEAMS 10
#define MAX_LENGTH 100
#include "Input.h"
char **GetTeamName() {
    FILE *file = fopen("Teams.txt", "r");
    char **teams = malloc(MAX_TEAMS * sizeof(char *));
    for (int i = 0; i < MAX_TEAMS; i++) {
        teams[i] = malloc(MAX_LENGTH);
        if (!fgets(teams[i], MAX_LENGTH, file)) break;

        // Process string safely
        char *name = teams[i];
        while (*name && !isupper(*name)) name++;
        memmove(teams[i], name, strlen(name) + 1);
        char *nl = strchr(teams[i], '\n');
        if (nl) *nl = '\0';
    }
    fclose(file);
    return teams;
}

char **GetStadium() {
    FILE *file = fopen("Venues.txt", "r");
    char **stadium=malloc(MAX_TEAMS*sizeof(char *));
    for (int i = 0; i < MAX_TEAMS; i++) {
        stadium[i] = malloc(MAX_LENGTH);
        if (!fgets(stadium[i], MAX_LENGTH, file)) break;
        char *name = stadium[i];
        while (*name && !isupper(*name)) name++;
        memmove(stadium[i], name, strlen(name) + 1);
        char *nl = strchr(stadium[i], '\n');
        if (nl) *nl = '\0';
    }
    fclose(file);
    return stadium;
}
