#include "playoffs.h"
#include "shared.h"
#include <stdio.h>
#include <stdlib.h>

void conductPlayoffs(Team teams[]) {
    printf("\n===== PLAYOFF QUALIFIERS =====\n");
    printf("The following teams have qualified for the playoffs:\n");

    for (int i = 0; i < 4; i++) {
        printf("%d. %s (Points: %d, NRR: %.3f)\n", 
               i + 1, teams[i].name, 
               teams[i].points, teams[i].nrr);
    }

     printf("\nQUALIFIER 1: %s vs %s\n", teams[0].name, teams[1].name);
     int q1Winner = rand() % 2;
     int q1Loser = 1 - q1Winner;
     printf("Winner: %s (advances to Final)\n", teams[q1Winner].name);

     printf("\nELIMINATOR: %s vs %s\n", teams[2].name, teams[3].name);
     int elimWinner = 2 + (rand() % 2);
     printf("Winner: %s (advances to Qualifier 2)\n", teams[elimWinner].name);

     printf("\nQUALIFIER 2: %s vs %s\n", teams[q1Loser].name, teams[elimWinner].name);
     int q2Winner = (rand() % 2) == 0 ? q1Loser : elimWinner;
     printf("Winner: %s (advances to Final)\n", teams[q2Winner].name);

     printf("\nIPL FINAL: %s vs %s\n", teams[q1Winner].name, teams[q2Winner].name);
     int finalWinner = (rand() % 2) == 0 ? q1Winner : q2Winner;
     printf("\nIPL CHAMPION: %s\n", teams[finalWinner].name);
     
     printf("\nCongratulations to %s for winning the IPL!\n", teams[finalWinner].name);
 
}

