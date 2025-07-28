//
// Created by nanda on 03-04-2025.
//

#ifndef SEARCH_MODULE_H
#define SEARCH_MODULE_H

#include <stdio.h>
#include <string.h>
#include "matches.h"

void searchByTeamName(Match* matches, int total_matches, const char* team_name);
void searchByDate(Match* matches, int total_matches, const char* date);

#endif //SEARCH_MODULE_H
