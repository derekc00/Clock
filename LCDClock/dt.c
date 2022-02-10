/*
 * dt.c
 *
 * Created: 2/10/2022 10:30:27 AM
 *  Author: derek
 */ 

#include "dt.h"
#include "lcd.h"
#include <stdio.h>
#include <stdbool.h>

char DAYSINMONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void init_dt(struct dt *dt) {
	dt->year = 2001;
	dt->month = 2;
	dt->day = 28;
	dt->hour = 23;
	dt->minute = 59;
	dt->second = 57;
}
bool isLeapYear(int year) { return (year%4==0) && ((year%400==0) || (year%100 != 0)); }

void advance_dt(struct dt *dt) {
	++dt->second;
	if (60 == dt->second) {
		dt->second = 0;
		++dt->minute;
		if (60 == dt->minute) {
			dt->minute = 0;
			++dt->hour;
			if (24 == dt->hour) {
				dt->hour = 0;
				++dt->day;
				if ( (isLeapYear(dt->year) && dt->month==2 && dt->day == 30) || (!isLeapYear(dt->year) && (DAYSINMONTH[dt->month - 1] + 1 == dt->day))) {
					dt->day = 1;
					++dt->month;
					if (13 == dt->month) {
						dt->month = 1;
						++dt->year;
					}
				}
			}
		}
	}
}

void print_dt(const struct dt *dt) {
	char buf[17];
	lcd_pos(0,0);
	sprintf(buf, "%02d / %02d / %04d ",
	dt->month,
	dt->day,
	dt->year);
	lcd_puts2(buf);
	lcd_pos(1,0);
	sprintf(buf, "%02d : %02d : %02d ",
	dt->hour,
	dt->minute,
	dt->second);
	lcd_puts2(buf);
}

void blink_dt(const struct dt *dt, int editField) {
	char buf[17];
	switch (editField) {
		case 1:
		lcd_pos(0,0);
		sprintf(buf, "   / %02d / %04d ",
		dt->day,
		dt->year);
		break;
		case 2:
		lcd_pos(0,0);
		sprintf(buf, "%02d /    / %04d ",
		dt->month,
		dt->year);
		break;
		case 3:
		lcd_pos(0,0);
		sprintf(buf, "%02d / %02d /     ",
		dt->month,
		dt->day);
		break;
		case 4:
		lcd_pos(1,0);
		sprintf(buf, "   : %02d : %02d",
		dt->minute,
		dt->second);
		break;
		case 5:
		lcd_pos(1,0);
		sprintf(buf, "%02d :    : %02d",
		dt->hour,
		dt->second);
		break;
		case 6:
		lcd_pos(1,0);
		sprintf(buf, "%02d : %02d :   ",
		dt->hour,
		dt->minute);
		break;
		default:
		break;
	}
	lcd_puts2(buf);
}

void copy_dt(struct dt *origin, struct dt *dest) {
	dest->year	 = origin->year;
	dest->month	 = origin->month;
	dest->day	 = origin->day;
	dest->hour	 = origin->hour;
	dest->minute = origin->minute;
	dest->second = origin->second;
}



bool isValid(struct dt *dt) {
	
	if (dt->year > 9999 || dt->year < 0) {
		return false;
		} else if (dt->month > 12 || dt->month < 1){
		return false;
		} else if (isLeapYear(dt->year) && dt->month == 2 && dt->day > 29) {
		return false;
		} else if ( !(isLeapYear(dt->year)&&dt->month==2) && (dt->day > DAYSINMONTH[dt->month-1] || dt->day < 1)) {
		return false;
		} else if (dt->hour > 24 || dt->hour < 0) {
		return false;
		} else if (dt->minute > 59 || dt->minute < 0) {
		return false;
		} else if (dt->second > 59 || dt->second < 0) {
		return false;
		} else {
		return true;
	}
	
}

void editDate(struct dt *dt, int editField, int value) {
	
	switch(editField) {
		case 1: // month
		dt->month = (dt->month % 10) * 10 + value;
		break;
		case 2: // day
		dt->day = (dt->day % 10) * 10 + value;
		break;
		case 3: // year
		dt->year = (dt->year % ((dt->year / 1000) * 1000)) * 10 + value;
		break;
		case 4: // hour
		dt->hour = (dt->hour % 10) * 10 + value;
		break;
		case 5: // minute
		dt->minute = (dt->minute % 10) * 10 + value;
		break;
		case 6:	// second
		dt->second = (dt->second % 10 ) * 10 + value;
		break;
	}
}

void convertMilitaryToAMPM(struct dt *dt, char *indicator) {
	sprintf(indicator, (dt->hour < 12 ? " AM" : " PM"));

	if (dt->hour == 0) { dt->hour = 12; }
	if (dt->hour > 12) { dt->hour = dt->hour - 12; }

}