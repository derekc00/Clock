/*
 * dt.h
 *
 * Created: 2/10/2022 10:29:56 AM
 *  Author: derek and david
 */ 

#ifndef _DT_H
#define _DT_H
#include <stdbool.h>



struct dt {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

void init_dt(struct dt *dt);
bool isLeapYear(int year);
void advance_dt(struct dt *dt);
void print_dt(const struct dt *dt);
void blink_dt(const struct dt *dt, int editField);
void copy_dt(struct dt *origin, struct dt *dest);
bool isValid(struct dt *dt);
void editDate(struct dt *dt, int editField, int value);
void convertMilitaryToAMPM(struct dt *dt, char *indicator);
#endif