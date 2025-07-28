#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <stdbool.h>
#include "shared.h"
struct Date {
    int day;
    int month;
    int year;
}Date;
//void generateSchedule(Match matches[]);
// displaySchedule(Match matches[], Team teams[]);
int isWeekend(int day);
void getDateString(char *date, int matchDay); 
bool isTeamPlayingOnDayOrNext(int team, int current_idx, int a[], int b[], struct Date dates[], int total);
void map_days(char day[][100], int *days, int total);
int isPriorityTeam(int team);
void schedule_time(char time[][100], int total, int *days);
void schedule_days(struct Date *dates, int *days, struct Date start_date, int total);
void rotate(int *arr, int n);
void dateToString(struct Date date, char* str);
int getDayOfWeek(struct Date date);
struct Date addDays(struct Date date, int days);
int daysInMonth(int month, int year);
bool isLeapYear(int year);
void optimize_schedule(int a[], int b[], int days[], struct Date dates[], int total);
void print_schedule(int a[], int b[], struct Date dates[], int days[], char time[][100], int total, int n);
#endif

