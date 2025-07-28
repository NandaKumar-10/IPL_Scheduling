#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include "ShortestDistance.h"
#include "teams.h"
#include "matches.h"
#include "search_module.h"
#include "playoffs.h"
//#include "Results.h"
//initizilateTeams(team[]);
#include "points_table.h"

extern int dis[10][10];

struct Date {
    int day;
    int month;
    int year;
};

typedef struct {
    float rain_probability;
    float temperature;
    char condition[50];
} WeatherData;

WeatherData getWeatherForStadium(int stadium_id, struct Date date) {
    WeatherData weather;
    srand(time(NULL) + stadium_id + date.day + date.month + date.year);
    weather.rain_probability = (float)(rand() % 10);
    weather.temperature = 20.0 + (float)(rand() % 20);
    
    if (weather.rain_probability > 8) {
        strcpy(weather.condition, "Heavy Rain Expected");
    } else if (weather.rain_probability > 5) {
        strcpy(weather.condition, "Light Rain Possible");
    } else {
        strcpy(weather.condition, "Clear");
    }
    
    return weather;
}

bool shouldRescheduleMatch(int stadium_id, struct Date date) {
    WeatherData weather = getWeatherForStadium(stadium_id, date);
    return weather.rain_probability > 8.0;
}

void rescheduleMatch(Match* matches, int match_index, int total_matches) {
    if (match_index < total_matches - 1) {
        Match temp = matches[match_index];
        for (int i = match_index; i < total_matches - 1; i++) {
            matches[i] = matches[i + 1];
        }
        matches[total_matches - 1] = temp;
    }
}

void determineWinner(Match *match, Team *teams);
void displayResults(Match *match);

const char* VENUES[10] = {
    "Wankhede Stadium, Mumbai",
    "M. A. Chidambaram Stadium, Chennai",
    "M. Chinnaswamy Stadium, Bangalore",
    "Eden Gardens, Kolkata",
    "Arun Jaitley Stadium, Delhi",
    "Punjab Cricket Association Stadium, Mohali",
    "Sawai Mansingh Stadium, Jaipur",
    "Rajiv Gandhi International Stadium, Hyderabad",
    "BRSABV Ekana Cricket Stadium, Lucknow",
    "Narendra Modi Stadium, Ahmedabad"
};

struct ipl_schedule {
    char date[100];
    char day[100];
    char time[100];
    char a_final[100];
    char b_final[100];
    char venue_final[100];
};

struct ipl_schedule *ipl_match_ptr;

bool isLeapYear(int year) {
    if (year % 4 != 0) return false;
    if (year % 100 != 0) return true;
    return (year % 400 == 0);
}

int daysInMonth(int month, int year) {
    switch(month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: return isLeapYear(year) ? 29 : 28;
        default: return 0;
    }
}

struct Date addDays(struct Date date, int days) {
    while (days > 0) {
        int remainingDays = daysInMonth(date.month, date.year) - date.day;
        if (days > remainingDays) {
            days -= remainingDays + 1;
            date.day = 1;
            if (date.month == 12) {
                date.month = 1;
                date.year++;
            } else {
                date.month++;
            }
        } else {
            date.day += days;
            days = 0;
        }
    }
    return date;
}

int getDayOfWeek(struct Date date) {
    int m = date.month;
    int y = date.year;
    if (m < 3) {
        m += 12;
        y--;
    }
    int k = y % 100;
    int j = y / 100;
    int h = (date.day + (13*(m+1))/5 + k + k/4 + j/4 + 5*j) % 7;
    return (h + 5) % 7 + 1;
}

void dateToString(struct Date date, char* str) {
    sprintf(str, "%02d/%02d/%04d", date.day, date.month, date.year);
}

void rotate(int *arr, int n) {
    int x = arr[n-1];
    for(int i = n-1; i > 1; i--) {
        arr[i] = arr[i-1];
    }
    arr[1] = x;
}

void schedule_days(struct Date *dates, int *days, struct Date start_date, int total) {
    struct Date current_date = start_date;
    int current_day = getDayOfWeek(start_date);

    for(int i = 0; i < total; ) {
        days[i] = current_day;
        dates[i] = current_date;

        if (current_day == 6 || current_day == 7) {
            if (i + 1 < total) {
                days[i+1] = current_day;
                dates[i+1] = current_date;
                i += 2;
            } else {
                i += 1;
            }
        } else {
            i += 1;
        }

        current_date = addDays(current_date, 1);
        current_day = getDayOfWeek(current_date);
    }
}


void schedule_time(char time[][100], int total, int *days) {
    for(int i = 0; i < total; i++) {
        if (days[i] == 6 || days[i] == 7) {
            if (i > 0 && days[i] == days[i-1]) {
                strcpy(time[i], "7:30 pm");
            } else {
                strcpy(time[i], "3:30 pm");
            }
        } else {
            strcpy(time[i], "7:30 pm");
        }
    }
}

int isPriorityTeam(int team) {
    return (team == 1 || team == 2 || team == 3 || team == 4);
}

void map_days(char day[][100], int *days, int total) {
    for(int i = 0; i < total; i++) {
        switch(days[i]) {
            case 1: strcpy(day[i], "Monday"); break;
            case 2: strcpy(day[i], "Tuesday"); break;
            case 3: strcpy(day[i], "Wednesday"); break;
            case 4: strcpy(day[i], "Thursday"); break;
            case 5: strcpy(day[i], "Friday"); break;
            case 6: strcpy(day[i], "Saturday"); break;
            case 7: strcpy(day[i], "Sunday"); break;
        }
    }
}

char day_str[500][100], a_final[500][100], b_final[500][100], venue_final[500][100];
bool isTeamPlayingOnDayOrNext(int team, int current_idx, int a[], int b[], struct Date dates[], int total) {
    if (current_idx == 0) return false;

    for (int i = 0; i < current_idx; i++) {
        if (dates[i].day == dates[current_idx].day &&
            dates[i].month == dates[current_idx].month &&
            dates[i].year == dates[current_idx].year) {
            if (a[i] == team || b[i] == team) {
                return true;
            }
            }
    }

    struct Date prev_day = addDays(dates[current_idx], -1);
    for (int i = 0; i < current_idx; i++) {
        if (dates[i].day == prev_day.day &&
            dates[i].month == prev_day.month &&
            dates[i].year == prev_day.year) {
            if (a[i] == team || b[i] == team) {
                return true;
            }
            }
    }

    return false;
}

int getNextBestVenue(int current_venue, int team, int current_idx, int a[], int b[], struct Date dates[], int total) {
    int *distances = dijkstra(dis, current_venue);
    int best_venue = -1;
    int min_distance = INT_MAX;

    for (int i = 0; i < 10; i++) {
        if (i == current_venue) continue;

        bool has_consecutive = false;
        for (int j = current_idx + 1; j < total; j++) {
            if (a[j] == team || b[j] == team) {
                if (j + 1 < total && (a[j+1] == team || b[j+1] == team)) {
                    has_consecutive = true;
                    break;
                }
            }
        }

        if (distances[i] < min_distance && !has_consecutive) {
            min_distance = distances[i];
            best_venue = i;
        }
    }
    
    free(distances);
    return best_venue;
}

bool isTeamPlayingSoon(int team, int current_idx, int a[], int b[], struct Date dates[], int total, int days_ahead) {
    for (int i = current_idx + 1; i < total && i < current_idx + days_ahead; i++) {
        if (a[i] == team || b[i] == team) {
            return true;
        }
    }
    return false;
}

int getLastMatchType(int team, int current_idx, int a[], int b[]) {
    for (int i = current_idx - 1; i >= 0; i--) {
        if (a[i] == team) {
            return 1;
        }
        if (b[i] == team) {
            return -1;
        }
    }
    return 0;
}

bool isValidHomeAwaySwap(int team, int current_idx, int target_idx, int is_home_team, int a[], int b[]) {
    int last_match_type = getLastMatchType(team, current_idx, a, b);

    if (last_match_type == 0) return true;

    return (last_match_type == 1 && !is_home_team) || (last_match_type == -1 && is_home_team);
}

void optimize_schedule(int a[], int b[], int days[], struct Date dates[], int total) {
    for(int i = 0; i < total; i++) {
        if (isTeamPlayingOnDayOrNext(a[i], i, a, b, dates, total) ||
            isTeamPlayingOnDayOrNext(b[i], i, a, b, dates, total)) {

            for(int j = i+1; j < total; j++) {
                if (!isTeamPlayingOnDayOrNext(a[j], i, a, b, dates, total) &&
                    !isTeamPlayingOnDayOrNext(b[j], i, a, b, dates, total)) {

                    if (isValidHomeAwaySwap(a[j], j, i, true, a, b) &&
                        isValidHomeAwaySwap(b[j], j, i, false, a, b)) {
                        int temp_a = a[i];
                        int temp_b = b[i];
                        a[i] = a[j];
                        b[i] = b[j];
                        a[j] = temp_a;
                        b[j] = temp_b;
                        break;
                    }
                }
            }
        }
        if (i > 0) {
            int current_venue = a[i-1] - 1;
            int next_best_venue = getNextBestVenue(current_venue, a[i], i, a, b, dates, total);
            
            if (next_best_venue != -1) {
                for (int j = i; j < total; j++) {
                    if (a[j] - 1 == next_best_venue && 
                        !isTeamPlayingSoon(a[j], j, a, b, dates, total, 3) &&
                        isValidHomeAwaySwap(a[j], j, i, true, a, b) &&
                        isValidHomeAwaySwap(b[j], j, i, false, a, b)) {
                        
                        int temp_a = a[i];
                        int temp_b = b[i];
                        a[i] = a[j];
                        b[i] = b[j];
                        a[j] = temp_a;
                        b[j] = temp_b;
                        break;
                    }
                }
            }
        }
    }
}


Match* print_schedule(int a[], int b[], struct Date dates[], int days[], char time[][100], int total, int n) {
    Match* matches = (Match*)malloc(total * sizeof(Match));
    if (!matches) {
        printf("Error: Failed to allocate memory for matches\n");
        return NULL;
    }

    Team *team = initializeTeams();
    if (!team) {
        printf("Error: Failed to initialize teams\n");
        free(matches);
        return NULL;
    }

    optimize_schedule(a, b, days, dates, total);

    ipl_match_ptr = (struct ipl_schedule*) malloc(total * sizeof(struct ipl_schedule));
    if (!ipl_match_ptr) {
        printf("Error: Failed to allocate memory for IPL schedule\n");
        free(matches);
        return NULL;
    }

    for(int i = 0; i < total; i++) {
        strcpy(venue_final[i], VENUES[a[i]-1]);
        strcpy(a_final[i], team[a[i]-1].name);
        strcpy(b_final[i], team[b[i]-1].name);
    }

    map_days(day_str, days, total);

    for(int i = 0; i < total; i++) {
        char date_str[20];
        dateToString(dates[i], date_str);
        strcpy((ipl_match_ptr+i)->date, date_str);
        strcpy((ipl_match_ptr+i)->day, day_str[i]);
        strcpy((ipl_match_ptr+i)->time, time[i]);
        strcpy((ipl_match_ptr+i)->a_final, a_final[i]);
        strcpy(matches[i].team1, a_final[i]);
        strcpy(matches[i].team2, b_final[i]);
        strcpy(matches[i].date, date_str);
        strcpy(matches[i].time, time[i]);
        strcpy((ipl_match_ptr+i)->b_final, b_final[i]);
        strcpy((ipl_match_ptr+i)->venue_final, venue_final[i]);
        strcpy(matches[i].venue, venue_final[i]);

        WeatherData weather = getWeatherForStadium(a[i]-1, dates[i]);
        if (weather.rain_probability > 8.0) {
            printf("\nWARNING: Match %d at %s has high rain probability (%.1f%%). Match abandoned - both teams get 1 point.\n",
                   i+1, venue_final[i], weather.rain_probability);
            
            team[a[i]-1].points += 1;
            team[b[i]-1].points += 1;
            team[a[i]-1].matches_played++;
            team[b[i]-1].matches_played++;
            team[a[i]-1].ties++;
            team[b[i]-1].ties++;
            
            strcpy(matches[i].winner, "No Result (Rain)");
            strcpy(matches[i].losser_team, "No Result (Rain)");
        }
    }

    printf("\n+-------+------------+------------+-----------+------------------------------------------------------------------------------+\n");
    printf("| Match |    Date    |    Day     |   Time    |                              Match Details                                  |\n");
    printf("+-------+------------+------------+-----------+------------------------------------------------------------------------------+\n");

    for(int i = 0; i < total; i++) {
        WeatherData weather = getWeatherForStadium(a[i]-1, dates[i]);
        printf("| %-5d | %-10s | %-10s | %-9s | %-30s vs %-30s |\n",
               i+1, (ipl_match_ptr+i)->date, (ipl_match_ptr+i)->day, (ipl_match_ptr+i)->time,
               (ipl_match_ptr+i)->a_final, (ipl_match_ptr+i)->b_final);
        printf("|       |            |            |           | Venue: %-62s |\n", (ipl_match_ptr+i)->venue_final);
        printf("|       |            |            |           | Weather: %-59s |\n", weather.condition);
        if (i < total - 1) {
            printf("+-------+------------+------------+-----------+------------------------------------------------------------------------------+\n");
        }
    }
    printf("+-------+------------+------------+-----------+------------------------------------------------------------------------------+\n");
    
    return matches;
}

float generateRandomNRR(float lower, float upper) {
    return lower + ((float)rand() / RAND_MAX) * (upper - lower);
}
void determineWinner(Match *match, Team *teams) {
    float winningTeamNRR,losingTeamNRR;
    int total_matches = 90;
    srand(time(NULL));
    for (int i = 0; i < total_matches; i++) {
        int indexA = -1, indexB = -1;

        for (int j = 0; j < TOTAL_TEAMS; j++) {
            if (strcmp(match[i].team1, teams[j].name) == 0) {
                indexA = j;
            }
            if (strcmp(match[i].team2, teams[j].name) == 0) {
                indexB = j;
            }
        }

        if (indexA == -1 || indexB == -1) {
            printf("Error: Could not find team for match %d\n", i+1);
            continue;
        }

        teams[indexA].matches_played++;
        teams[indexB].matches_played++;

        int result = rand() % 10;
        if (result < 5) {
            strcpy(match[i].winner, teams[indexA].name);
            strcpy(match[i].losser_team, teams[indexB].name);
            teams[indexA].wins++;
            teams[indexB].losses++;
            teams[indexA].points += 2;
            winningTeamNRR = generateRandomNRR(0.0, 1.0);
            losingTeamNRR = generateRandomNRR(-1.0, 0.0);
            teams[indexA].nrr+=winningTeamNRR;
            teams[indexB].nrr-=losingTeamNRR;
        }
        else if (result < 9) {
            strcpy(match[i].winner, teams[indexB].name);
            strcpy(match[i].losser_team, teams[indexA].name);
            teams[indexB].wins++;
            teams[indexA].losses++;
            teams[indexB].points += 2;
            winningTeamNRR = generateRandomNRR(0.0, 1.0);
            losingTeamNRR = generateRandomNRR(-1.0, 0.0);
            teams[indexB].nrr+=winningTeamNRR;
            teams[indexA].nrr-=losingTeamNRR;
        }
        else {
            strcpy(match[i].winner, "Draw");
            strcpy(match[i].losser_team, "None");
            teams[indexA].ties++;
            teams[indexB].ties++;
            teams[indexA].points += 1;
            teams[indexB].points += 1;
        }
    }
}

void displayResults(Match *match) {
    if (!match) {
        printf("Error: Invalid match data\n");
        return;
    }
    
    int total_matches = 90;

    printf("\n===== MATCH RESULTS =====\n");
    for (int i = 0; i < total_matches; i++) {
        printf("Match %d: %s vs %s | Winner: %s\n", 
               i+1, match[i].team1, match[i].team2, match[i].winner);
    }
    printf("========================\n\n");
}

void saveResultsToFile(Match* matches, int total_matches) {
    FILE* file = fopen("ipl_results.txt", "a");
    
    time_t now;
    time(&now);
    char* timestamp = ctime(&now);
    timestamp[strlen(timestamp)-1] = '\0';
    
    fprintf(file, "\n\nIPL MATCH RESULTS - %s\n", timestamp);
    fprintf(file, "===================================\n\n");

    for (int i = 0; i < total_matches; i++) {
        fprintf(file, "Match %d: %s vs %s\n", i+1, matches[i].team1, matches[i].team2);
        fprintf(file, "Date: %s | Time: %s | Venue: %s\n", matches[i].date, matches[i].time, matches[i].venue);
        fprintf(file, "Winner: %s\n", matches[i].winner);
        fprintf(file, "----------------------------------------\n");
    }

    fclose(file);
    printf("Results have been appended to 'ipl_results.txt'\n");
}

void viewHeadToHeadStats(Match* matches, int total_matches, Team* teams) {
    char team1[50], team2[50];
    printf("\nEnter first team name: ");
    scanf(" %[^\n]", team1);
    printf("Enter second team name: ");
    scanf(" %[^\n]", team2);

    int matches_played = 0;
    int team1_wins = 0;
    int team2_wins = 0;
    int draws = 0;

    printf("\nHead-to-Head Statistics: %s vs %s\n", team1, team2);
    printf("===================================\n");

    // Search in current matches
    for (int i = 0; i < total_matches; i++) {
        if ((strcmp(matches[i].team1, team1) == 0 && strcmp(matches[i].team2, team2) == 0) ||
            (strcmp(matches[i].team1, team2) == 0 && strcmp(matches[i].team2, team1) == 0)) {
            
            matches_played++;
            printf("\nMatch %d: %s vs %s\n", i+1, matches[i].team1, matches[i].team2);
            printf("Date: %s | Venue: %s\n", matches[i].date, matches[i].venue);
            
            if (strcmp(matches[i].winner, team1) == 0) {
                team1_wins++;
                printf("Winner: %s\n", team1);
            } else if (strcmp(matches[i].winner, team2) == 0) {
                team2_wins++;
                printf("Winner: %s\n", team2);
            } else {
                draws++;
                printf("Result: Draw\n");
            }
        }
    }

    // Search in historical results file
    FILE* file = fopen("ipl_results.txt", "r");
    if (file) {
        char line[256];
        char current_match[256] = "";
        char current_winner[256] = "";
        char current_date[256] = "";
        char current_venue[256] = "";
        int match_number = 0;

        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, "Match ") && strstr(line, " vs ")) {
                strcpy(current_match, line);
                sscanf(line, "Match %d:", &match_number);
            } else if (strstr(line, "Date: ")) {
                strcpy(current_date, line);
            } else if (strstr(line, "Venue: ")) {
                strcpy(current_venue, line);
            } else if (strstr(line, "Winner: ")) {
                strcpy(current_winner, line);
                
                // Check if this match involves our teams
                if ((strstr(current_match, team1) && strstr(current_match, team2)) ||
                    (strstr(current_match, team2) && strstr(current_match, team1))) {
                    
                    matches_played++;
                    printf("\nMatch %d: %s", match_number, current_match);
                    printf("%s%s", current_date, current_venue);
                    
                    if (strstr(current_winner, team1)) {
                        team1_wins++;
                        printf("Winner: %s\n", team1);
                    } else if (strstr(current_winner, team2)) {
                        team2_wins++;
                        printf("Winner: %s\n", team2);
                    } else if (strstr(current_winner, "Draw")) {
                        draws++;
                        printf("Result: Draw\n");
                    }
                }
            }
        }
        fclose(file);
    }

    printf("\nSummary:\n");
    printf("Total Matches Played: %d\n", matches_played);
    printf("%s Wins: %d\n", team1, team1_wins);
    printf("%s Wins: %d\n", team2, team2_wins);
    printf("Draws: %d\n", draws);
}

int main() {
    int choice;
    char team_name[50];
    int n = 10;
    int total = n*(n-1);
    struct Date start_date;
    Team *team = initializeTeams();
    printf("Welcome to the IPL SCHEDULER for 10 teams.\n");
    printf("Teams participating this year:\n");
    for(int i = 0; i < 10; i++) {
        printf("%d. %s\n", i+1, team[i].name);
    }
    printf("\nVenues for this season:\n");
    for(int i = 0; i < 10; i++) {
        printf("%d. %s\n", i+1, VENUES[i]);
    }
    printf("\nEnter the starting date of the tournament (DD MM YYYY): ");
    scanf("%d %d %d", &start_date.day, &start_date.month, &start_date.year);
    printf("\n");

    int team_A[500], team_B[500];
    struct Date dates[500];
    int days[500];
    char time[500][100];
    int a[500], b[500];

    int arr[10];
    for(int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    int match_count_A = 0;
    int match_count_B = 0;

    for(int i = 0; i < n / 2; i++) {
        team_A[match_count_A] = arr[i];
        match_count_A++;
    }
    for(int i = n-1; i > n/2-1; i--) {
        team_B[match_count_B] = arr[i];
        match_count_B++;
    }

    for(int j = 0; j < n-2; j++) {
        rotate(arr, n);
        for(int i = 0; i < n / 2; i++) {
            team_A[match_count_A] = arr[i];
            match_count_A++;
        }
        for(int i = n-1; i > n/2-1; i--) {
            team_B[match_count_B] = arr[i];
            match_count_B++;
        }
    }

    int count = 0;
    for(int i = 0; i < n*(n-1)/2; i++) {
        a[count] = team_A[i];
        b[count] = team_B[i];
        count++;
    }

    for(int i = n*(n-1)/2, j = 0; i < n*(n-1); i++, j++) {
        a[count] = team_B[j];
        b[count] = team_A[j];
        count++;
    }
    schedule_days(dates, days, start_date, total);
    schedule_time(time, total, days);

    Match* matches = print_schedule(a, b, dates, days, time, total, n);
    determineWinner(matches, team);
    
    choice = 0;
    while (choice != 7) {  // Changed from 5 to 7
        printf("\n1. To display the match results\n");
        printf("2. To Display Points Tables\n");
        printf("3. To see the specific team Schedule\n");
        printf("4. To see PlayOffs Schedule\n");
        printf("5. Save Results to File\n");
        printf("6. View Head-to-Head Statistics\n");
        printf("7. To exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        sort(team, 10);
        
        if (choice == 1) {
            displayResults(matches);
        }
        else if (choice == 2) {
            PrintPointsTable(team, 10);
        }
        else if (choice == 3) {
            while (getchar() != '\n');
            printf("Enter the team name you want to search: ");
            fgets(team_name, 50, stdin);
            int len = strlen(team_name);
            if (len > 0 && team_name[len-1] == '\n') {
                team_name[len-1] = '\0';
            }
            searchByTeamName(matches, 90, team_name);
        }
        else if (choice == 4) {
            conductPlayoffs(team);
        }
        else if (choice == 5) {
            saveResultsToFile(matches, total);
        }
        else if (choice == 6) {
            viewHeadToHeadStats(matches, total, team);
        }
        else if (choice == 7) {
            printf("---------------Exiting-------------\n");
        }
        else {
            printf("Invalid choice\n");
        }
    }
    
    free(matches);
    free(ipl_match_ptr);
    return 0;
}






