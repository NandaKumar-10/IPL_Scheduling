#ifndef POINTS_TABLE_H
#define POINTS_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "teams.h"
#include "matches.h"

void updatePointsTable(Team teams[], Match *match);
void sortPointsTable(Team teams[]);
void displayPointsTable(Team teams[]);
void PrintPointsTable(Team* teams, int n);
void sort(Team teams[], int n);

#endif

