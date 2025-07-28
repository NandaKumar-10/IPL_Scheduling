#include <stdio.h>
#include "teams.h"
#include "points_table.h"
/*
typedef struct Teams {
    char name[10];
    int total;
    int wins;
    int loss;
    float nrr;
    int points;
}Team;*/

void sort(Team teams[],int n){
    for (int i=0;i<n;i++) {
        for (int j=i+1;j<n;j++) {
            if (teams[i].points<teams[j].points || (teams[i].points==teams[j].points && teams[i].nrr<teams[j].nrr)) {
                Team temp = teams[i];
                teams[i]=teams[j];
                teams[j]=temp;
            }
        }
    }
}

void PrintPointsTable(Team teams[],int n) {
    printf("\n+-------+------------------------------+--------+-------+-------+-------+--------+------------+\n");
    printf("| S.No  | Team Name                    | Matches| Wins  | Loss  | Ties  | Points | NRR        |\n");
    printf("+-------+------------------------------+--------+-------+-------+-------+--------+------------+\n");

    for (int i = 0; i < n; i++) {
        printf("| %-5d | %-28s | %-6d | %-5d | %-5d | %-5d | %-6d | %-10.3f |\n",
               i+1, teams[i].name, teams[i].matches_played, teams[i].wins, 
               teams[i].losses, teams[i].ties, teams[i].points, teams[i].nrr);
    }
    printf("+-------+------------------------------+--------+-------+-------+-------+--------+------------+\n");
}

